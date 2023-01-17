#pragma once
#include "global.h"

/* gets an instruction to preform and calls the relevant operation method according to its opcode. */
void perform_instruction(reg_val_status* g_regs, inst_t* instruction, float_uint* res_ptr, uint32_t* g_mem_arr, bool* halted, float_uint src0, float_uint src1);

/* if it’s a LD operation updates the destination register float value,
and otherwise updates the destination register integer value. */
void update_reg(inst_t* inst, reg_val_status* g_regs, float_uint* res_ptr, uint32_t* g_mem_arr);