#include "operations.h"
#include "main.c"

void ld(int dst, int imm) {
	g_regs[dst].value.uint32_val = g_mem_arr[imm];
}

void st(int dst, int imm) {
	g_mem_arr[imm] = g_regs[dst].value.uint32_val;
}

void add(int dst, int src0, int src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val + g_regs[src1].value.float_val;
}

void sub(int dst, int src0, int src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val - g_regs[src1].value.float_val;
}

void mult(int dst, int src0, int src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val * g_regs[src1].value.float_val;
}

void div(int dst, int src0, int src1) {
	g_regs[dst].value.float_val = g_regs[src0].value.float_val / g_regs[src1].value.float_val;
}

void halt() {
	return;
}