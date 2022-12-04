#include <stdio.h>
#include <stdlib.h>
#include "scoreboard.h"
#include "operations.h"


bool fetch(simulation_t* scoreboard) {
	if (!is_full(&scoreboard->inst_queue)) {
		enqueue(&scoreboard->inst_queue, scoreboard->memory[scoreboard->pc]);
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


bool issue(simulation_t* scoreboard) {
	inst_queue_t* inst_queue = &scoreboard->inst_queue;
	reg_val_status* regs = scoreboard->regs;
	inst_t* issued_inst_buff = scoreboard->issued_inst;

	if (is_empty(inst_queue)) {
		return false;
	}

	// retrieving instructions queue's top
	uint32_t raw_inst = top(inst_queue);
	
	// decoding the instruction
	inst_t inst;
	parse_line_to_inst(raw_inst, &inst);

	// if the dst register is busy, waiting
	if (regs[inst.dst].status->busy = true) {
		return false;
	}

	// if there is no free unit, waiting
	unit_t* assigned_unit = find_free_unit(&scoreboard->config, scoreboard->op_units, inst.opcode);
	if (assigned_unit == NULL) {
		return false;
	}

	// everything is checked, issuing
	dequeue(inst_queue); // TODO method can be void because it is called after top

	// Mark dest reg status as taken by the unit
	assign_unit_to_inst(regs, &inst, assigned_unit);

	// Update assigned unit parameters
	update_scoreboard_after_issue(regs, &scoreboard->config, &inst, assigned_unit);

	// Update issued instructions array
	inst.inst_trace.cycle_issued = scoreboard->clock_cycle;
	insert_inst_into_inst_arr(issued_inst_buff, &inst, scoreboard->issued_cnt);
	// Update active instruction pointer in the assigned unit
	assigned_unit->active_instruction = &issued_inst_buff[scoreboard->issued_cnt];
	scoreboard->issued_cnt++;

	// After succesfull issue next state is read_operands
	assigned_unit->unit_state = READ_OPERANDS;
	
	return true;
}

bool read_operands(unit_t* assigned_unit, simulation_t* scoreboard) {
	// checks if both src0 and src1 registers are ready
	if (!assigned_unit->Rj || !assigned_unit->Rk) {
		return false;
	}

	// If both are ready, set them to not ready
	assigned_unit->Rj = false;
	assigned_unit->Rk = false;

	// After succesfull read next state is exec
	assigned_unit->unit_state = EXEC;

	// Update instruction trace
	assigned_unit->active_instruction->inst_trace.cycle_read_operands = scoreboard->clock_cycle;

	return true;
}

bool exec(unit_t* assigned_unit, simulation_t* scoreboard);

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

bool write_result(unit_t* assigned_unit, simulation_t* scoreboard) {
	reg_e dest_reg = assigned_unit->Fi;
	unit_t** op_units = scoreboard->op_units;
	config_t* config = &scoreboard->config;

	// Check for write after read - avoid writing to dest reg if any unit didn't read it yet
	if (is_there_unit_pending_read_operand(op_units, config, dest_reg)) {
		return false;
	}

	// Resolve read-after-write by updating the R fields to true in the units waiting for this write and clearing the Q fields
	update_pending_units(op_units, config, dest_reg, assigned_unit);

	// TODO hande read/write or write/write to same memory address

	// Update the value in the regs array according to the instruction or load/store
	perform_instruction(assigned_unit->active_instruction, scoreboard->regs, scoreboard->memory, &scoreboard->halted);

	// Update instruction trace
	assigned_unit->active_instruction->inst_trace.cycle_write_result = scoreboard->clock_cycle;

	// Mark unit as not busy, and dest reg as not pending for any unit
	assigned_unit->busy = false;
	scoreboard->regs[dest_reg].status = NULL;
	assigned_unit->unit_state = IDLE;

	return true;
}