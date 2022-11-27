#pragma once

#include "global.h"
#include <stdio.h>

#define MAX_ASSEMBLY_LINES          (4096)
#define INSTRUCTION_LINE_LEN		(8)


void load_instructions(FILE* instr_file, asm_cmd_t* imem_buff);