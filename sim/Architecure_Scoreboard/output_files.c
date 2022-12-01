#include "output_files.h"

// will be written in the end of execution
void write_memout_file(FILE* memout_file, uint32_t* g_mem_arr) {
    char mem_buffer[INSTRUCTION_LINE_LEN + 2];

    for (int i = 0; i < MAX_ASSEMBLY_LINES; i++) { // TODO consider changing to memory_count
        sprintf(mem_buffer, "%lX", g_mem_arr[i]);
        fputs(mem_buffer, memout_file);
    }
}

// will be written in the end of execution
void write_regout_file(FILE* regout_file, reg_val_status* g_regs) {
    char val_buffer[sizeof(float)];

    for (int i = 0; i < REGS_NUM; i++) {
        sprintf(val_buffer, "%f", g_regs[i].value.float_val);
        fputs(val_buffer, regout_file);
    }
}

// will be written in the end of execution
void write_traceinst_file(FILE* traceinst_file, uint32_t* g_mem_arr) {
    for (int i = 0; i < MAX_ASSEMBLY_LINES; i++) { // TODO change to number of instructions
        inst_t* inst = g_mem_arr[i];
        inst_trace_t inst_trace = inst->inst_trace;
        fprintf(traceinst_file, "%08X %d %s%d %d %d %d %d\n",
            inst->raw_inst,
            i,
            inst_trace.unit_id.operation,
            inst_trace.unit_id.index,
            inst_trace.cycle_issued,
            inst_trace.cycle_read_operands,
            inst_trace.cycle_execute_end,
            inst_trace.cycle_write_result);
    }
}

// will be written in every cycle
void write_traceunit_file(FILE* traceunit_file, int cycle, unit_t* trace_unit) {
    fprintf(traceunit_file, "%d %s %s %s %s %s %s %s %s\n",
        cycle,
        trace_unit->unit_id.operation,
        trace_unit->unit_id.index,
        trace_unit->Fi,
        trace_unit->Fj,
        trace_unit->Fk,
        trace_unit->Qj != NULL ? get_unit_id_str(trace_unit->Qj->unit_id) : "-",
        trace_unit->Qj != NULL ? get_unit_id_str(trace_unit->Qk->unit_id) : "-",
        trace_unit->Rj ? "Yes" : "No",
        trace_unit->Rj ? "Yes" : "No");
}

char* get_unit_id_str(unit_id_t unit_id) {
    char unit_index[8];
    sprintf(unit_index, "%lu", unit_id.index);
    return unit_id.operation + unit_index;
}