#include "global.h"
#include "memin.h"

// THIS IS THE DECODE FUNCTION - SHOULD BE CALLED WITH ISSUE STAGE
static void parse_line_to_cmd(char* line, inst_t* cmd) {
    /* Each command is 32 bits*/
    uint32_t raw;
    sscanf_s(line, "%lX", &raw);

    /* construct the command object */
    cmd->imm = raw & 0xFFF;
    cmd->src1 = (raw >> 12) & 0xF;
    cmd->src0 = (raw >> 16) & 0xF;
    cmd->dst = (raw >> 20) & 0xF;
    cmd->opcode = (raw >> 24) & 0xF;

    cmd->raw_inst = raw;
}


void load_memin(FILE* instr_file, uint32_t* imem_buff) {
    char line_buffer[INSTRUCTION_LINE_LEN + 2];
    int memory_count = 0;
    uint32_t curr_mem;
    /* stops when either (n-1) characters are read, or /n is read
    We want to read the /n char so it won't get in to the next line */
    while (fgets(line_buffer, INSTRUCTION_LINE_LEN + 2, instr_file) != NULL) {
        sscanf_s(line_buffer, "%lX", &curr_mem);
        imem_buff[memory_count++] = curr_mem;
    }
}