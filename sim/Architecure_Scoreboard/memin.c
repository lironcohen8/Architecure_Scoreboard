#include "global.h"
#include "memin.h"

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

// THIS IS THE DECODE FUNCTION - SHOULD BE CALLED WITH ISSUE STAGE
static void parse_line_to_inst(uint32_t raw, inst_t* inst) {
    /* construct the command object */
    inst->imm = raw & 0xFFF;
    inst->src1 = (raw >> 12) & 0xF;
    inst->src0 = (raw >> 16) & 0xF;
    inst->dst = (raw >> 20) & 0xF;
    inst->opcode = (raw >> 24) & 0xF;

    inst->raw_inst = raw;
}