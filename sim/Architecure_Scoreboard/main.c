#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

asm_cmd_t g_cmd_arr[MAX_ASSEMBLY_LINES]; /* Commands array */
config_t  g_config;                     /* Configuration paramaters */


static const char* g_operation_strs[] = { "ld", "st", "add", "sub", "mul", "div"};

/* File opening and validation used for all files */
static FILE* open_and_validate_file(char const* file_name, char const* perms) {
    FILE* file;
    fopen_s(&file, file_name, perms);
    if (file == NULL) {
        printf("Error opening file %s\n", file_name);
        exit(0);
    }
    else {
        return file;
    }
}

static void parse_line_to_cmd(char* line, asm_cmd_t* cmd) {
    /* Each command is 32 bits*/
    uint32_t raw;
    sscanf_s(line, "%lX", &raw);

    /* construct the command object */
    cmd->imm    =  raw & 0xFFF;
    cmd->src1   = (raw >> 12) & 0xF;
    cmd->src0   = (raw >> 16) & 0xF;
    cmd->dst    =  (raw >> 20) & 0xF;
    cmd->opcode = (raw >> 24) & 0xF;

    cmd->raw_cmd = raw;
}

static void load_instructions(FILE* instr_file) {
    char line_buffer[INSTRUCTION_LINE_LEN + 2];
    int instructions_count = 0;
    asm_cmd_t curr_cmd;
    /* stops when either (n-1) characters are read, or /n is read
    We want to read the /n char so it won't get in to the next line */
    while (fgets(line_buffer, INSTRUCTION_LINE_LEN + 2, instr_file) != NULL) {
        parse_line_to_cmd(line_buffer, &curr_cmd);
        g_cmd_arr[instructions_count++] = curr_cmd;
    }
}

static void update_config_param(char* param_str, char* param_val) {
    char* ret;
    ret = strstr(param_str, "units");
    if (ret != NULL) {
        /* this is a number of untis paramater */
    }
    else {
        ret = strstr(param_str, "delay");
        if (ret != NULL) {
            /* this is a delay paramater */
        }
        else {
            /* */
        }
    }
}


int main(int argc, char* argv[]) {
	printf("hello");
    printf(argv[0]);
    FILE* file = open_and_validate_file(argv[1], "r");
    load_instructions(file);
}