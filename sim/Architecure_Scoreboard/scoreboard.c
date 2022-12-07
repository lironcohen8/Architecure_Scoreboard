#include <stdio.h>
#include <stdlib.h>
#include "scoreboard.h"
#include "operations.h"

simulation_t    g_simulation;

uint32_t num_of_active_instructions() {
	return g_simulation.issued_cnt - g_simulation.finished_cnt;
}

bool is_halted() {
	return g_simulation.halted;
}

void advance_pc() {
	g_simulation.pc++;
}

bool fetch() {
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

	inst->raw_inst = raw_inst;
}

static unit_t* find_free_unit(config_t* g_config, unit_t** g_op_units, opcode_e operation) {
	int unit_cnt = g_config->units[operation].num_units;
	for (int i = 0; i < unit_cnt; i++) {
		unit_t unit = g_op_units[operation][i];
		if (!unit.busy) {
			return &g_op_units[operation][i];
		}
	}
	return NULL;
}

static void assign_unit_to_inst(reg_val_status* regs, inst_t* inst, unit_t* assigned_unit) {
	regs[inst->dst].status = assigned_unit;
}

static void update_scoreboard_after_issue(reg_val_status* regs, config_t* config, inst_t* inst, unit_t* assigned_unit) {
	assigned_unit->busy = true;
	assigned_unit->exec_cnt = config->units[inst->opcode].unit_delay_cycles;
	assigned_unit->Fi = inst->dst;
	assigned_unit->Fj = inst->src0;
	assigned_unit->Fk = inst->src1;
	assigned_unit->Qj = regs[inst->src0].status;
	assigned_unit->Qk = regs[inst->src1].status;
	assigned_unit->Rj = assigned_unit->Qj == NULL;
	assigned_unit->Rk = assigned_unit->Qk == NULL;
}

static void insert_inst_into_inst_arr(inst_t* inst_arr, inst_t* inst, uint32_t index) {
	inst_arr[index] = *inst;
}


bool issue() {
	inst_queue_t* inst_queue = &g_simulation.inst_queue;
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

	// if the dst register is busy, waiting
	if (regs[inst.dst].status != NULL) {
		return false;
	}

	// if there is no free unit, waiting
	unit_t* assigned_unit = find_free_unit(&g_simulation.config, g_simulation.op_units, inst.opcode);
	if (assigned_unit == NULL) {
		return false;
	}

	// everything is checked, issuing
	dequeue(inst_queue); // TODO method can be void because it is called after top

	// Mark dest reg status as taken by the unit
	assign_unit_to_inst(regs, &inst, assigned_unit);

	// Update assigned unit parameters
	update_scoreboard_after_issue(regs, &g_simulation.config, &inst, assigned_unit);

	// Update issued instructions array
	inst.inst_trace.cycle_issued = g_simulation.clock_cycle;
	insert_inst_into_inst_arr(issued_inst_buff, &inst, g_simulation.issued_cnt);
	// Update active instruction pointer in the assigned unit
	assigned_unit->active_instruction = &issued_inst_buff[g_simulation.issued_cnt];
	g_simulation.issued_cnt++;

	// After succesfull issue next state is read_operands
	assigned_unit->unit_state = READ_OPERANDS;
	
	return true;
}

bool read_operands(unit_t* assigned_unit) {
	if (!assigned_unit->Rj || !assigned_unit->Rk) {
		return false;
	}

	// If both are ready, set them to not ready
	assigned_unit->Rj = false;
	assigned_unit->Rk = false;

	// After succesfull read next state is exec
	assigned_unit->unit_state = EXEC;

	// Update instruction trace
	assigned_unit->active_instruction->inst_trace.cycle_read_operands = g_simulation.clock_cycle;

	// When read operands is done - the first execuation cycle happens
	// TODO: verify what happens when the delay of a unit is 1 ? does the exec and read operands finish in the same cycle ?
	assigned_unit->exec_cnt--;

	return true;
}

bool exec(unit_t* assigned_unit) {
	assigned_unit->exec_cnt--;

	if (assigned_unit->exec_cnt == 0) {
		assigned_unit->active_instruction->inst_trace.cycle_execute_end = g_simulation.clock_cycle;
		assigned_unit->unit_state = WRITE_RESULT;
		return true;
	}

	return false;
}

static bool is_there_unit_pending_read_operand(unit_t** op_units, config_t* config, reg_e dest_reg) {
	unit_t current_unit;
	for (opcode_e operation = 0; operation < CONFIGURED_UNITS; operation++) {
		uint32_t num_units = config->units[operation].num_units;
		for (uint32_t i = 0; i < num_units; i++) {
			current_unit = op_units[operation][i];
			if (current_unit.busy) {
				if (current_unit.Fj == dest_reg && current_unit.Rj) {
					return true;
				}
				if (current_unit.Fk == dest_reg && current_unit.Rk) {
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
			if (current_unit.busy) {
				if (current_unit.Qj == writing_unit) {
					current_unit.Rj = true;
					current_unit.Qj = NULL;
				}
				if (current_unit.Qk == writing_unit) {
					current_unit.Rk = true;
					current_unit.Qk = NULL;
				}
			}
		}
	}

}

bool write_result(unit_t* assigned_unit) {
	reg_e dest_reg = assigned_unit->Fi;
	unit_t** op_units = g_simulation.op_units;
	config_t* config = &g_simulation.config;

	// Check for write after read - avoid writing to dest reg if any unit didn't read it yet
	if (is_there_unit_pending_read_operand(op_units, config, dest_reg)) {
		return false;
	}

	// Resolve read-after-write by updating the R fields to true in the units waiting for this write and clearing the Q fields
	update_pending_units(op_units, config, dest_reg, assigned_unit);

	// TODO hande read/write or write/write to same memory address

	// Update the value in the regs array according to the instruction or load/store
	perform_instruction(assigned_unit->active_instruction, g_simulation.regs, g_simulation.memory, &g_simulation.halted);

	// Update instruction trace
	assigned_unit->active_instruction->inst_trace.cycle_write_result = g_simulation.clock_cycle;

	// Mark unit as not busy, and dest reg as not pending for any unit
	assigned_unit->busy = false;
	g_simulation.regs[dest_reg].status = NULL;
	assigned_unit->unit_state = IDLE;

	g_simulation.finished_cnt++;

	return true;
}

simulation_t* get_simulation() {
	return &g_simulation;
}