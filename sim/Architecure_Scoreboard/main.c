#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "parsing.h"
#include "memin.h"
#include "cfg.h"
#include "init.h"

inst_t   q_mem_arr[MAX_ASSEMBLY_LINES]; /* Commands array */
config_t g_config;                   /* Configuration paramaters */
unit_t** g_op_units;
reg_val_status g_regs[REGS_NUM];

int main(int argc, char* argv[]) {
	printf("hello\n");
    printf(argv[0]);
    FILE* mem_file = open_and_validate_file(argv[2], "r");
    load_memin(mem_file, q_mem_arr);
    printf("bi1\n");

    FILE* cfg_file = open_and_validate_file(argv[1], "r");
    load_configuration(cfg_file, &g_config);
    printf("bi2\n");

    g_op_units = init_units(&g_config);
    printf("bi3\n");
}