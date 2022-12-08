#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "parsing.h"
#include "init.h"
#include "scoreboard.h"

simulation_t    g_simulation;

int main(int argc, char* argv[]) {
    FILE* mem_file = open_and_validate_file(argv[2], "r");
    
    FILE* cfg_file = open_and_validate_file(argv[1], "r");
    
    init_simulation(&g_simulation, mem_file, cfg_file);

    while(1);
}