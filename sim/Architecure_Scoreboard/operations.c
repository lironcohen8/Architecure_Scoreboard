#include <stdio.h>
#include <stdlib.h>
#include "operations.h"

static void ld_op(reg_val_status* g_regs, uint32_t* g_mem_arr, reg_e dst, int imm) {
	g_regs[dst].value.uint32_val = g_mem_arr[imm];
}

static void st_op(reg_val_status* g_regs, uint32_t* g_mem_arr, reg_e dst, int imm) {
	g_mem_arr[imm] = g_regs[dst].value.uint32_val;
}

static void add_op(reg_val_status* g_regs, reg_e dst, reg_e src0, reg_e src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val + g_regs[src1].value.float_val;
}

static void sub_op(reg_val_status* g_regs, reg_e dst, reg_e src0, reg_e src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val - g_regs[src1].value.float_val;
}

static void mult_op(reg_val_status* g_regs, reg_e dst, reg_e src0, reg_e src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val * g_regs[src1].value.float_val;
}

static void div_op(reg_val_status* g_regs, reg_e dst, reg_e src0, reg_e src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val / g_regs[src1].value.float_val;
}

static void halt_op(bool* halted) {
	*halted = true;
}

void perform_instruction(inst_t* inst, reg_val_status* g_regs, uint32_t* g_mem_arr, bool* halted) {
	opcode_e operation = inst->opcode;
	reg_e dst = inst->dst;
	reg_e src0 = inst->src0;
	reg_e src1 = inst->src1;
	int imm = inst->imm;

	switch (operation) {
		case LD:
			ld_op(g_regs, g_mem_arr, dst, imm);
			break;
		case ST:
			st_op(g_regs, g_mem_arr, dst, imm);
			break;
		case ADD:
			add_op(g_regs, dst, src0, src1);
			break;
		case SUB:
			sub_op(g_regs, dst, src0, src1);
			break;
		case MULT:
			mult_op(g_regs, dst, src0, src1);
			break;
		case DIV:
			div_op(g_regs, dst, src0, src1);
			break;
		case HALT:
			halt_op(halted);
			break;
		default:
			printf("Illegal operation\n");
			exit(1);
			break;
	}
}