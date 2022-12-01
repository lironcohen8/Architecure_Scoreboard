#include "scoreboard.h";
#include "main.c";

bool fetch(int pc) {
	if (!g_inst_queue.is_full) {
		enqueue(&g_inst_queue, g_mem_arr[pc]);
		return true;
	}
	return false;
}

bool issue() {
	if (g_inst_queue.is_empty) {
		return false;
	}

	// retrieving instructions queue's top
	uint32_t inst_int = top(&g_inst_queue);
	inst_t* inst = (inst_t*)malloc(sizeof(inst_t*));
	
	// decoding the instruction
	parse_line_to_inst(inst_int, inst);

	// if the dst register is busy, waiting
	if (g_regs[inst->dst].status->busy = true) {
		return false;
	}

	// if there is no free unit, waiting
	unit_t* assigned_unit = find_free_unit(inst->opcode);
	if (assigned_unit == NULL) {
		return false;
	}
	dequeue(&g_inst_queue); // TODO method can be void because it is called after top
	assign_unit_to_inst(inst, assigned_unit);
	update_scoreboard_after_issue(inst, assigned_unit);
}

unit_t* find_free_unit(opcode_e operation) {
	int unit_cnt = g_config.units[operation].num_units;
	for (int i = 0; i < unit_cnt; i++) {
		unit_t unit = g_op_units[operation][i];
		if (!unit.busy) {
			return &unit;
		}
	}
	return NULL;
}

void assign_unit_to_inst(inst_t* inst, unit_t* assigned_unit) {
	g_regs[inst->dst].status = assigned_unit;
}

void update_scoreboard_after_issue(inst_t* inst, unit_t *assigned_unit) {
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

bool read_operands(unit_t* assigned_unit) {
	// checks if both src0 and src1 registers are ready
	if (!assigned_unit->Rj || !assigned_unit->Rk) {
		return false;
	}

	// If both are ready, set them to not ready
	assigned_unit->Rj = false;
	assigned_unit->Rk = false;
	return true;
}

bool exec();

bool write_result();