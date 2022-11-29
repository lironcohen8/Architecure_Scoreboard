#pragma once

#include "global.h"
#include <stdio.h>

#define MAX_ASSEMBLY_LINES          (4096)
#define INSTRUCTION_LINE_LEN		(8)


void load_memin(FILE* instr_file, inst_t* imem_buff);