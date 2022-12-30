#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "scoreboard.h"

/* init everything for simulation struct from memory input file and config unput file. */
void init_simulation(simulation_t* simulation, FILE* memin_file, FILE* cfg_file);