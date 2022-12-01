#include "operations.h"

void ld_op(reg_t* g_regs, uint32_t* g_mem_arr, int dst, int imm) {
	g_regs[dst].value.uint32_val = g_mem_arr[imm];
}

void st_op(reg_t* g_regs, uint32_t* g_mem_arr, int dst, int imm) {
	g_mem_arr[imm] = g_regs[dst].value.uint32_val;
}

void add_op(reg_t* g_regs, int dst, int src0, int src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val + g_regs[src1].value.float_val;
}

void sub_op(reg_t* g_regs, int dst, int src0, int src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val - g_regs[src1].value.float_val;
}

void mult_op(reg_t* g_regs, int dst, int src0, int src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val * g_regs[src1].value.float_val;
}

void div_op(reg_t* g_regs, int dst, int src0, int src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val / g_regs[src1].value.float_val;
}

void halt_op() {
	return;
}