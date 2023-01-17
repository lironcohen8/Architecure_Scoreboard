#include <stdio.h>
#include <stdlib.h>
#include "scoreboard.h"
#include "operations.h"

simulation_t    g_simulation;


static bool insert_address_to_buff(uint32_t addr, unit_t* req_unit);
static void remove_address_from_buff(uint32_t addr);
static bool is_address_collide(opcode_e opcode, uint32_t dst_address, uint32_t st_id, unit_t* req_unit);

uint32_t num_of_active_instructions() {
	return g_simulation.issued_cnt - g_simulation.finished_cnt;
}

bool is_halted() {
	return g_simulation.halted;
}

void advance_pc() {
	g_simulation.pc++;
}

/* If the program is halted returns false (not fetched).
If the instructions queue is not full, enqueues the instruction in the memory in index pc. */
bool fetch() {
	if (g_simulation.halted) {
		return false;
	}

	if (!is_full(&g_simulation.inst_queue)) {
		enqueue(&g_simulation.inst_queue, g_simulation.memory[g_simulation.pc]);
		return true;
	}
	return false;
}

static void parse_line_to_inst(uint32_t raw_inst, inst_t* inst) {
	/* construct the command object */
	inst->imm = raw_inst & 0xFFF;
	inst->src1 = (raw_inst >> 12) & 0xF;
	inst->src0 = (raw_inst >> 16) & 0xF;
	inst->dst = (raw_inst >> 20) & 0xF;
	inst->opcode = (raw_inst >> 24) & 0xF;

	if (inst->opcode == LD) {
		inst->src1 = REG_INVALID;
		inst->src0 = REG_INVALID;
	}

	if (inst->opcode == ST) {
		inst->dst = REG_INVALID;
		inst->src0 = REG_INVALID;
	}

	inst->raw_inst = raw_inst;
}

static unit_t* find_free_unit(config_t* g_config, unit_t** g_op_units, opcode_e operation) {
	int unit_cnt = g_config->units[operation].num_units;
	for (int i = 0; i < unit_cnt; i++) {
		unit_t unit = g_op_units[operation][i];
		if (!unit.busy.old_val) {
			return &g_op_units[operation][i];
		}
	}
	return NULL;
}

static void assign_unit_to_inst(reg_val_status* regs, inst_t* inst, unit_t* assigned_unit) {
	if (inst->opcode != ST) {
		regs[inst->dst].status.new_val = assigned_unit;
	}
	inst->inst_trace.unit = assigned_unit;
	inst->inst_trace.unit_id = assigned_unit->unit_id;
}

static void update_scoreboard_after_issue(reg_val_status* regs, config_t* config, inst_t* inst, unit_t* assigned_unit) {
	assigned_unit->busy.new_val = true;
	assigned_unit->exec_cnt = config->units[inst->opcode].unit_delay_cycles;
	assigned_unit->Fi.new_val = inst->dst;
	assigned_unit->Fj.new_val = inst->src0;
	assigned_unit->Fk.new_val = inst->src1;
	/* Special case being handled here by taking the new_val from the status of the src registers
	This is for the case when a write-result for register X and issue to instruction which uses register X in the same cycle.
	We want the issued instruction in this function to look at the new_val of status to avoid a deadlock 
	(i.e to avoid a case where the write-result clears the status and all pending units to this reg X and then the issue will look at the
	old value and see the reg X as pending and will wait for the write-result to set R to true and Q to NULL - 
	but this won't happen becouse we will miss thewrite in this case) -
	To complete the handling of this case, need to make sure that every cycle write-result is done before issue */
	
	if (inst->opcode == LD) { // LD doesn't depend on the values of SRC0, SRC1
		assigned_unit->Qj.new_val = NULL;
		assigned_unit->Qk.new_val = NULL;
		assigned_unit->Rj.new_val = true;
		assigned_unit->Rk.new_val = true;
	}
	else if (inst->opcode == ST) { // ST only depends on the value of SRC1
		assigned_unit->Qj.new_val = NULL;
		assigned_unit->Qk.new_val = regs[inst->src1].status.new_val;
		assigned_unit->Rj.new_val = true;
		assigned_unit->Rk.new_val = (assigned_unit->Qk.new_val == NULL);
	}
	else {
		assigned_unit->Qj.new_val = regs[inst->src0].status.new_val;
		assigned_unit->Qk.new_val = regs[inst->src1].status.new_val;
		assigned_unit->Rj.new_val = (assigned_unit->Qj.new_val == NULL);
		assigned_unit->Rk.new_val = (assigned_unit->Qk.new_val == NULL);
	}
}

static void insert_inst_into_inst_arr(inst_t* inst_arr, inst_t* inst, uint32_t index) {
	inst_arr[index] = *inst;
}

/* Issuing an instruction. */
bool issue() {
	uint32_queue_t* inst_queue = &g_simulation.inst_queue;
	reg_val_status* regs = g_simulation.regs;
	inst_t* issued_inst_buff = g_simulation.issued_inst;

	if (is_empty(inst_queue)) {
		return false;
	}

	// retrieving instructions queue's top
	uint32_t raw_inst = top(inst_queue);
	
	// decoding the instruction
	inst_t inst;
	parse_line_to_inst(raw_inst, &inst);

	if (inst.opcode == HALT) {
		g_simulation.halted = true;
		return true;
	}

	/* if the dst register is busy, waiting(handling write after write) */
	if (inst.opcode != ST && regs[inst.dst].status.old_val != NULL) {
		return false;
	}

	// if there is no free unit, waiting
	unit_t* assigned_unit = find_free_unit(&g_simulation.config, g_simulation.op_units, inst.opcode);
	if (assigned_unit == NULL) {
		return false;
	}

	if (inst.opcode == ST || inst.opcode == LD) {
		// Insert new address to active addresses buffer
		if (!insert_address_to_buff(inst.imm, assigned_unit)) {
			return false;
		}
	}

	// everything is checked, issuing
	dequeue(inst_queue);

	// Update assigned unit parameters
	update_scoreboard_after_issue(regs, &g_simulation.config, &inst, assigned_unit);

	// Mark dest reg status as taken by the unit
	assign_unit_to_inst(regs, &inst, assigned_unit);

	// Update issued instructions array
	inst.inst_trace.cycle_issued = g_simulation.clock_cycle;
	insert_inst_into_inst_arr(issued_inst_buff, &inst, g_simulation.issued_cnt);
	// Update active instruction pointer in the assigned unit
	assigned_unit->active_instruction = &issued_inst_buff[g_simulation.issued_cnt];
	g_simulation.issued_cnt++;


	// After a successful issue next state is read_operands
	assigned_unit->unit_state = READ_OPERANDS;
	
	return true;
}

static bool try_perfrom_instruction(unit_t* assigned_unit) {
	inst_t* instruction = assigned_unit->active_instruction;

	if (is_address_collide(instruction->opcode, instruction->imm, assigned_unit->unit_id.index, assigned_unit)) {
		// We stall the execution of a memory instruction that will cause a collision or is dependent on earlier memory access not yet finished
		return false;
	}

	perform_instruction(g_simulation.regs, assigned_unit->active_instruction, &assigned_unit->exec_result, g_simulation.memory, &g_simulation.halted, assigned_unit->src0, assigned_unit->src1);

	if (instruction->opcode == ST || instruction->opcode == LD) {
		// In case of a successfull store - remove its address from the active buffer
		remove_address_from_buff(instruction->imm);
	}

	return true;
}
/* Read operands. */
bool read_operands(unit_t* assigned_unit) {
	if (!assigned_unit->Rj.old_val || !assigned_unit->Rk.old_val) {
		return false;
	}

	// If both are ready, set them to not ready
	assigned_unit->Rj.new_val = false;
	assigned_unit->Rk.new_val = false;

	// After a successful read next state is exec
	assigned_unit->unit_state = EXEC;

	// Update instruction trace
	assigned_unit->active_instruction->inst_trace.cycle_read_operands = g_simulation.clock_cycle;

	// save the correct values for instruction operation
	assigned_unit->src0 =  g_simulation.regs[assigned_unit->active_instruction->src0].value.old_val;
	assigned_unit->src1 = g_simulation.regs[assigned_unit->active_instruction->src1].value.old_val;

	exec(assigned_unit);

	return true;
}

/* execution. */
bool exec(unit_t* assigned_unit) {
	assigned_unit->exec_cnt--;

	if (assigned_unit->exec_cnt <= 0) {
		if (!assigned_unit->executed) {
			if (try_perfrom_instruction(assigned_unit)) {
				assigned_unit->active_instruction->inst_trace.cycle_execute_end = g_simulation.clock_cycle;
				assigned_unit->unit_state = WRITE_RESULT;
				assigned_unit->executed = true;
			}
			else {
				return false;
			}
		}
	}

	return false;
}

static bool is_there_unit_pending_read_operand(unit_t** op_units, config_t* config, reg_e dest_reg) {
	if (dest_reg == REG_INVALID) {
		return false;
	}
	unit_t current_unit;
	for (opcode_e operation = 0; operation < CONFIGURED_UNITS; operation++) {
		uint32_t num_units = config->units[operation].num_units;
		for (uint32_t i = 0; i < num_units; i++) {
			current_unit = op_units[operation][i];
			if (current_unit.busy.old_val) {
				if (current_unit.Fj.old_val == dest_reg && current_unit.Rj.old_val) {
					return true;
				}
				if (current_unit.Fk.old_val == dest_reg && current_unit.Rk.old_val) {
					return true;
				}
			}
		}
	}

	return false;
}

static void update_pending_units(unit_t** op_units, config_t* config, reg_e dest_reg, unit_t* writing_unit) {
	unit_t current_unit;
	for (opcode_e operation = 0; operation < CONFIGURED_UNITS; operation++) {
		uint32_t num_units = config->units[operation].num_units;
		for (uint32_t i = 0; i < num_units; i++) {
			current_unit = op_units[operation][i];
			if (current_unit.busy.old_val) {
				if (current_unit.Qj.old_val == writing_unit) {
					op_units[operation][i].Rj.new_val = true;
					op_units[operation][i].Qj.new_val = NULL;
				}
				if (current_unit.Qk.old_val == writing_unit) {
					op_units[operation][i].Rk.new_val = true;
					op_units[operation][i].Qk.new_val = NULL;
				}
			}
		}
	}
}

static bool is_address_collide(opcode_e opcode, uint32_t dst_address, uint32_t st_id, unit_t* req_unit) {
	bool is_ld = (opcode == LD);
	bool is_st = (opcode == ST);
	
	if (!is_ld && !is_st) {
		return false;
	}

	// any address present in the active address which came from a different unit
	//		will indicate a collision.
	address_entry* address_buff = g_simulation.active_st_addresses;
	for (uint32_t i = 0; i < g_simulation.active_st_addresses_size; i++) {
		if (address_buff[i].addr == dst_address && (address_buff[i].active_unit != req_unit)) {
			return true;
		}
	}
	return false;
}


static bool insert_address_to_buff(uint32_t addr, unit_t* requesting_unit) {
	for (uint32_t i = 0; i < g_simulation.active_st_addresses_size; i++) {
		if (g_simulation.active_st_addresses[i].addr == addr) {
			return false;
		}
	}
	for (uint32_t i = 0; i < g_simulation.active_st_addresses_size; i++) {
		if (g_simulation.active_st_addresses[i].addr == ADDRESS_INVALID) {
			g_simulation.active_st_addresses[i].addr = addr;
			g_simulation.active_st_addresses[i].active_unit = requesting_unit;
			return true;
		}
	}
	return false;
}

static void remove_address_from_buff(uint32_t addr) {
	for (uint32_t i = 0; i < g_simulation.active_st_addresses_size; i++) {
		if (g_simulation.active_st_addresses[i].addr == addr) {
			g_simulation.active_st_addresses[i].addr = ADDRESS_INVALID;
			g_simulation.active_st_addresses[i].active_unit = NULL;
		}
	}
}


/* Write result. */
bool write_result(unit_t* assigned_unit) {
	reg_e dest_reg = assigned_unit->Fi.old_val;
	unit_t** op_units = g_simulation.op_units;
	config_t* config = &g_simulation.config;

	// Check for write after read - avoid writing to dest reg if any unit didn't read it yet
	if (is_there_unit_pending_read_operand(op_units, config, dest_reg)) {
		return false;
	}

	// Resolve read-after-write by updating the R fields to true in the units waiting for this write and clearing the Q fields
	update_pending_units(op_units, config, dest_reg, assigned_unit);

	// Update the value in the regs array according to the instruction result
	if (assigned_unit->active_instruction->opcode != ST) {
		update_reg(assigned_unit->active_instruction, g_simulation.regs, &assigned_unit->exec_result, g_simulation.memory);
	}

	// Update instruction trace
	assigned_unit->active_instruction->inst_trace.cycle_write_result = g_simulation.clock_cycle;

	// Mark unit as not busy, and dest reg as not pending for any unit
	assigned_unit->busy.new_val = false;
	if (dest_reg != REG_INVALID) {
		g_simulation.regs[dest_reg].status.new_val = NULL;
	}
	assigned_unit->unit_state = IDLE;
	assigned_unit->executed = false;

	g_simulation.finished_cnt++;

	return true;
}

simulation_t* get_simulation() {
	return &g_simulation;
}

/* Going over all of the busy units, and calling the relevant method according to the unit's state. */
void execute_all(simulation_t* simulation) {
	for (opcode_e operation = 0; operation < CONFIGURED_UNITS; operation++) {
		uint32_t num_units = simulation->config.units[operation].num_units;
		for (uint32_t i = 0; i < num_units; i++) {
			unit_t* unit = &simulation->op_units[operation][i];
			if (unit->busy.old_val) {
				switch (unit->unit_state) {
				case WRITE_RESULT:
					write_result(unit);
					break;
				case EXEC:
					exec(unit);
					break;
				case READ_OPERANDS:
					read_operands(unit);
					break;
				}
			}
		}
	}
}

static void update_ff_regs() {
	reg_val_status* regs = g_simulation.regs;
	for (reg_e reg_index = F0; reg_index < REGS_NUM; reg_index++) {
		regs[reg_index].value.old_val.float_val = regs[reg_index].value.new_val.float_val;
		regs[reg_index].status.old_val = regs[reg_index].status.new_val;
	}
}

static void update_ff_units() {
	unit_t** op_units = g_simulation.op_units;
	config_t config = g_simulation.config;

	for (opcode_e operation = 0; operation < CONFIGURED_UNITS; operation++) {
		uint32_t num_units = config.units[operation].num_units;
		for (uint32_t i = 0; i < num_units; i++) {
			op_units[operation][i].busy.old_val = op_units[operation][i].busy.new_val;
			op_units[operation][i].Qj.old_val = op_units[operation][i].Qj.new_val;
			op_units[operation][i].Qk.old_val = op_units[operation][i].Qk.new_val;
			op_units[operation][i].Fi.old_val = op_units[operation][i].Fi.new_val;
			op_units[operation][i].Fj.old_val = op_units[operation][i].Fj.new_val;
			op_units[operation][i].Fk.old_val = op_units[operation][i].Fk.new_val;
			op_units[operation][i].Rj.old_val = op_units[operation][i].Rj.new_val;
			op_units[operation][i].Rk.old_val = op_units[operation][i].Rk.new_val;
		}
	}
}

void cycle_end() {
	update_ff_regs();
	update_ff_units();
}