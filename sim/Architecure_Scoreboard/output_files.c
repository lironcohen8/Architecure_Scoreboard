#include "output_files.h"

// will be written in the end of execution
void write_memout_file(FILE* memout_file, uint32_t* g_mem_arr) {
    char mem_buffer[INSTRUCTION_LINE_LEN + 2];

    for (int i = 0; i < MAX_ASSEMBLY_LINES; i++) { // TODO consider changing to memory_count
        sprintf_s(mem_buffer, "%lX", g_mem_arr[i]);
        fputs(mem_buffer, memout_file);
    }
}

// will be written in the end of execution
void write_regout_file(FILE* regout_file, reg_t* g_regs) {
    char val_buffer[NUMBER_OF_DIGITS_OF_FLOAT + 2];

    for (int i = 0; i < REGS_NUM; i++) {
        _gcvt_s(val_buffer, NUMBER_OF_DIGITS_OF_FLOAT + 2, g_regs[i].value.float_val, NUMBER_OF_DIGITS_OF_FLOAT);
        fputs(val_buffer, regout_file);
    }
}

// will be written in the end of execution
void write_traceinst_file(FILE* traceinst_file, inst_t* inst) {
    for (int i = 0; i < MAX_ASSEMBLY_LINES; i++) { // TODO change to number of instructions
        inst_trace_t inst_trace = inst->inst_trace;
        fprintf(traceinst_file, "%08X %d %s%" PRIu32 "%d %d %d %d\n",
            inst->raw_inst,
            i,
            opcode_str[inst_trace.unit_id.operation],
            inst_trace.unit_id.index,
            inst_trace.cycle_issued,
            inst_trace.cycle_read_operands,
            inst_trace.cycle_execute_end,
            inst_trace.cycle_write_result);
    }
}


char* get_op_name(opcode_e op_index) {
    return regs_str[op_index];
}

// will be written in every cycle
void write_traceunit_file(FILE* traceunit_file, reg_t* g_regs, int cycle, unit_t* trace_unit) {
    fprintf_s(traceunit_file, "%d %s%" PRIu32 "%s %s %s %s%" PRIu32 "%s" PRIu32 "%s %s\n",
        cycle,
        trace_unit->unit_id.operation,
        trace_unit->unit_id.index,
        regs_str[trace_unit->Fi],
        regs_str[trace_unit->Fj],
        regs_str[trace_unit->Fk],
        trace_unit->Qj != NULL ? opcode_str[trace_unit->Qj->unit_id.operation] : "-",
        trace_unit->Qj != NULL ? trace_unit->Qj->unit_id.index : "",
        trace_unit->Qk != NULL ? opcode_str[trace_unit->Qk->unit_id.operation] : "-",
        trace_unit->Qk != NULL ? trace_unit->Qk->unit_id.index : "",
        trace_unit->Rj ? "Yes" : "No",
        trace_unit->Rj ? "Yes" : "No");
}