#pragma once
#include "global.h"
#include "inst_queue.h"
#include "memin.h"

bool fetch(inst_queue_t* g_inst_queue, uint32_t* g_mem_arr, int pc);

bool issue(inst_queue_t* g_inst_queue, reg_val_status* g_regs, config_t g_config, unit_t** g_op_units);

bool read_operands(unit_t* assigned_unit);

bool exec();

bool write_result();