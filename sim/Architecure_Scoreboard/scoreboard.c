#include <stdio.h>
#include <stdlib.h>
#include "scoreboard.h"

static int issued_cnt = 0;

bool fetch(inst_queue_t* g_inst_queue, uint32_t* g_mem_arr, int pc) {
	if (!is_full(g_inst_queue)) {
		enqueue(g_inst_queue, g_mem_arr[pc]);
		return true;
	}
	return false;
}

void parse_line_to_inst(uint32_t raw_inst, inst_t* inst) {
	/* construct the command object */
	inst->imm = raw_inst & 0xFFF;
	inst->src1 = (raw_inst >> 12) & 0xF;
	inst->src0 = (raw_inst >> 16) & 0xF;
	inst->dst = (raw_inst >> 20) & 0xF;
	inst->opcode = (raw_inst >> 24) & 0xF;

	inst->raw_inst = raw_inst;
}

unit_t* find_free_unit(config_t g_config, unit_t** g_op_units, opcode_e operation) {
	int unit_cnt = g_config.units[operation].num_units;
	for (int i = 0; i < unit_cnt; i++) {
		unit_t unit = g_op_units[operation][i];
		if (!unit.busy) {
			return &g_op_units[operation][i];
		}
	}
	return NULL;
}

void assign_unit_to_inst(reg_val_status* g_regs, inst_t* inst, unit_t* assigned_unit) {
	g_regs[inst->dst].status = assigned_unit;
}

void update_scoreboard_after_issue(reg_val_status* g_regs, config_t g_config, inst_t* inst, unit_t* assigned_unit) {
	assigned_unit->busy = true;
	assigned_unit->exec_cnt = g_config.units[inst->opcode].unit_delay_cycles;
	assigned_unit->Fi = inst->dst;
	assigned_unit->Fj = inst->src0;
	assigned_unit->Fk = inst->src1;
	assigned_unit->Qj = g_regs[inst->src0].status;
	assigned_unit->Qk = g_regs[inst->src1].status;
	assigned_unit->Rj = assigned_unit->Qj == NULL;
	assigned_unit->Rk = assigned_unit->Qk == NULL;
}

void insert_inst_into_inst_arr(inst_t* g_inst_arr, inst_t* inst) {
	
	g_inst_arr[issued_cnt++] = *inst;
}


bool issue(inst_queue_t* g_inst_queue, reg_val_status* g_regs, inst_t* g_inst_arr, config_t g_config, unit_t** g_op_units, int clock_cycle) {
	if (is_empty(g_inst_queue)) {
		return false;
	}

	// retrieving instructions queue's top
	uint32_t raw_inst = top(g_inst_queue);
	
	// decoding the instruction
	inst_t inst;
	parse_line_to_inst(raw_inst, &inst);

	// if the dst register is busy, waiting
	if (g_regs[inst.dst].status->busy = true) {
		return false;
	}

	// if there is no free unit, waiting
	unit_t* assigned_unit = find_free_unit(g_config, g_op_units, inst.opcode);
	if (assigned_unit == NULL) {
		return false;
	}

	// everything is checked, issuing
	dequeue(g_inst_queue); // TODO method can be void because it is called after top
	// Mark dest reg status as taken by the unit
	assign_unit_to_inst(g_regs, &inst, assigned_unit);
	update_scoreboard_after_issue(g_regs, g_config, &inst, assigned_unit);
	insert_inst_into_inst_arr(g_inst_arr, &inst);

	// After succesfull issue next state is read_operands
	assigned_unit->unit_state = READ_OPERANDS;
	inst.inst_trace.cycle_issued = clock_cycle;
	return true;
}

bool read_operands(unit_t* assigned_unit, inst_t* inst, int clock_cycle) {
	// checks if both src0 and src1 registers are ready
	if (!assigned_unit->Rj || !assigned_unit->Rk) {
		return false;
	}

	// If both are ready, set them to not ready
	assigned_unit->Rj = false;
	assigned_unit->Rk = false;

	// After succesfull read next state is exec
	assigned_unit->unit_state = EXEC;
	inst->inst_trace.cycle_read_operands = clock_cycle;

	return true;
}

bool exec();

bool write_result();