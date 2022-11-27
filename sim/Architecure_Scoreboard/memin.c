#include "global.h"
#include "memin.h"

static void parse_line_to_cmd(char* line, asm_cmd_t* cmd) {
    /* Each command is 32 bits*/
    uint32_t raw;
    sscanf_s(line, "%lX", &raw);

    /* construct the command object */
    cmd->imm = raw & 0xFFF;
    cmd->src1 = (raw >> 12) & 0xF;
    cmd->src0 = (raw >> 16) & 0xF;
    cmd->dst = (raw >> 20) & 0xF;
    cmd->opcode = (raw >> 24) & 0xF;

    cmd->raw_cmd = raw;
}


void load_instructions(FILE* instr_file, asm_cmd_t* imem_buff) {
    char line_buffer[INSTRUCTION_LINE_LEN + 2];
    int instructions_count = 0;
    asm_cmd_t curr_cmd;
    /* stops when either (n-1) characters are read, or /n is read
    We want to read the /n char so it won't get in to the next line */
    while (fgets(line_buffer, INSTRUCTION_LINE_LEN + 2, instr_file) != NULL) {
        parse_line_to_cmd(line_buffer, &curr_cmd);
        imem_buff[instructions_count++] = curr_cmd;
    }
}