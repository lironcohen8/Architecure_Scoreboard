#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "parsing.h"
#include "memin.h"
#include "cfg.h"

inst_t q_mem_arr[MAX_ASSEMBLY_LINES]; /* Commands array */
config_t  g_config;                   /* Configuration paramaters */



int main(int argc, char* argv[]) {
	printf("hello\n");
    printf(argv[0]);
    FILE* file = open_and_validate_file(argv[1], "r");
    load_instructions(file, q_mem_arr);
    printf("bi1\n");

    file = open_and_validate_file(argv[2], "r");
    load_configuration(file, &g_config);
    printf("bi2\n");
}