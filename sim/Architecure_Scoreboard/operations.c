#include <stdio.h>
#include <stdlib.h>
#include "operations.h"

static void ld_op(float_uint* res_ptr, uint32_t* g_mem_arr, int imm) {
	res_ptr->uint32_val = g_mem_arr[imm];
}

static void st_op(uint32_t* g_mem_arr, float_uint src1, int imm) {
	g_mem_arr[imm] = src1.uint32_val;
}

static void add_op(float_uint src0, float_uint src1, float_uint* res_ptr) {
	res_ptr->float_val = src0.float_val + src1.float_val;
}

static void sub_op(float_uint src0, float_uint src1, float_uint* res_ptr) {
	res_ptr->float_val = src0.float_val - src1.float_val;
}

static void mult_op(float_uint src0, float_uint src1, float_uint* res_ptr) {
	res_ptr->float_val = src0.float_val * src1.float_val;
}

static void div_op(float_uint src0, float_uint src1, float_uint* res_ptr) {
	res_ptr->float_val = src0.float_val / src1.float_val;
}

static void halt_op(bool* halted) {
	*halted = true;
}

void update_reg(inst_t* inst, reg_val_status* g_regs, float_uint* res_ptr, uint32_t* g_mem_arr) {
	opcode_e operation = inst->opcode;
	reg_e dst = inst->dst;

	if (operation != LD) {
		g_regs[dst].value.new_val.float_val = res_ptr->float_val;
	}
	else {
		g_regs[dst].value.new_val.uint32_val = res_ptr->uint32_val;
	}
	
}

void perform_instruction(reg_val_status* g_regs, inst_t* instruction, float_uint* res_ptr, uint32_t* g_mem_arr, bool* halted) {
	opcode_e operation = instruction->opcode;
	int imm = instruction->imm;
	float_uint src0 = g_regs[instruction->src0].value.old_val;
	float_uint src1 = g_regs[instruction->src1].value.old_val;

	switch (operation) {
		case LD:
			ld_op(res_ptr, g_mem_arr, imm);
			break;
		case ST:
			st_op(g_mem_arr, src1, imm);
			break;
		case ADD:
			add_op(src0, src1, res_ptr);
			break;
		case SUB:
			sub_op(src0, src1, res_ptr);
			break;
		case MULT:
			mult_op(src0, src1, res_ptr);
			break;
		case DIV:
			div_op(src0, src1, res_ptr);
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