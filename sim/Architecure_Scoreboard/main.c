#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "parsing.h"
#include "memin.h"
#include "cfg.h"
#include "init.h"
#include "inst_queue.h"
#include "scoreboard.h"

uint32_t        g_mem_arr[MEMORY_SIZE];     /* memory array contains both imem and dmem */
inst_t          g_inst_arr[MEMORY_SIZE];    /* issued instructions array */
config_t        g_config;                   /* Configuration paramaters */
scoreboard_t    g_scoreboard;

int main(int argc, char* argv[]) {

    FILE* mem_file = open_and_validate_file(argv[2], "r");
    load_memin(mem_file, g_mem_arr);

    FILE* cfg_file = open_and_validate_file(argv[1], "r");
    load_configuration(cfg_file, &g_config);

    init_scoreboard(&g_scoreboard, &g_config, g_mem_arr);

    while(1);
}