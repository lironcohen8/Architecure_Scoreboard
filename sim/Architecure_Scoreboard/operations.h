#pragma once
#include "global.h"

void ld_op(reg_t* g_regs, uint32_t* g_mem_arr, int dst, int imm);

void st_op(reg_t* g_regs, uint32_t* g_mem_arr, int dst, int imm);

void add_op(reg_t* g_regs, int dst, int src0, int src1);

void sub_op(reg_t* g_regs, int dst, int src0, int src1);

void mult_op(reg_t* g_regs, int dst, int src0, int src1);

void div_op(reg_t* g_regs, int dst, int src0, int src1);

void halt_op();