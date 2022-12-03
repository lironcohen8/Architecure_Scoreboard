#pragma once
#include "global.h"

void perform_instruction(inst_t* inst, reg_val_status* g_regs, uint32_t* g_mem_arr, bool* halted);