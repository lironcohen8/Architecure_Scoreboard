#pragma once
#include "global.h"

void perform_instruction(reg_val_status* g_regs, inst_t* instruction, float_uint* res_ptr, uint32_t* g_mem_arr, bool* halted);

void update_reg(inst_t* inst, reg_val_status* g_regs, float_uint* res_ptr, uint32_t* g_mem_arr);