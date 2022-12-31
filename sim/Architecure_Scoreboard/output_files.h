#pragma once

#include "global.h"
#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>

/* write the values for the memout file. will be written at the end of execution. */
void write_memout_file(FILE* memout_file, uint32_t* g_mem_arr);

/* write the values for the regout file. will be written at the end of execution. */
void write_regout_file(FILE* regout_file, reg_val_status* g_regs);

/* write the values for the traceinst file. will be written at the end of execution. */
void write_traceinst_file(FILE* traceinst_file, inst_t* issued_inst_arr, uint32_t issued_count);

/* write the values for the traceunit file. will be written in every cycle. */
void write_traceunit_file(FILE* traceunit_file, int cycle, unit_t* trace_unit);