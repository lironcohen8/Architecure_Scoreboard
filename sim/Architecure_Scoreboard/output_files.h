#pragma once

#include "global.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_ASSEMBLY_LINES				(4096)
#define INSTRUCTION_LINE_LEN			(8)
#define NUMBER_OF_DIGITS_OF_FLOAT		(10)

void write_memout_file(FILE* memout_file, uint32_t* g_mem_arr);

void write_regout_file(FILE* regout_file, reg_val_status* g_regs);

void write_traceinst_file(FILE* traceinst_file, uint32_t* g_mem_arr);

void write_traceunit_file(FILE* traceunit_file, int cycle, unit_t* trace_unit);