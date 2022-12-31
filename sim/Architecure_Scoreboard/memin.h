#pragma once

#include "global.h"
#include "parsing.h"
#include <stdio.h>

/* loads the memin input file to the memory struct. */
void load_memin(FILE* instr_file, uint32_t* imem_buff);
