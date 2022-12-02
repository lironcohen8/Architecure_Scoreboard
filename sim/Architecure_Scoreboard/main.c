#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "parsing.h"
#include "memin.h"
#include "cfg.h"
#include "init.h"
#include "inst_queue.h"

uint32_t        g_mem_arr[MEMORY_SIZE];     /* memory array contains both imem and dmem */
inst_t          g_inst_arr[MEMORY_SIZE];    /* issued instructions array */
config_t        g_config;                   /* Configuration paramaters */
unit_t**        g_op_units;
reg_val_status  g_regs[REGS_NUM];
inst_queue_t    g_inst_queue;
int             clock_cycle;

int main(int argc, char* argv[]) {

    FILE* mem_file = open_and_validate_file(argv[2], "r");
    load_memin(mem_file, g_mem_arr);

    FILE* cfg_file = open_and_validate_file(argv[1], "r");
    load_configuration(cfg_file, &g_config);

    g_op_units = init_units(&g_config);

    init_regs_status_values(g_regs);

    init_instruction_queue(&g_inst_queue);

    while(1);
}