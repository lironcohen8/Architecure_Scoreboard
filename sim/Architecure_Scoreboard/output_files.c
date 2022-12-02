#include "output_files.h"

// will be written in the end of execution
void write_memout_file(FILE* memout_file, uint32_t* g_mem_arr) {
    for (int i = 0; i < MEMORY_SIZE; i++) { // TODO consider changing to memory_count
        fprintf_s(memout_file, "%lX\n", g_mem_arr[i]);
    }
}

// will be written in the end of execution
void write_regout_file(FILE* regout_file, reg_val_status* g_regs) {
    for (int i = 0; i < REGS_NUM; i++) {
        fprintf_s(regout_file, "%f\n", g_regs[i].value.float_val);
    }
}

// will be written in the end of execution
void write_traceinst_file(FILE* traceinst_file, inst_t* inst) {
    for (int i = 0; i < MEMORY_SIZE; i++) { // TODO change to number of instructions
        inst_trace_t inst_trace = inst->inst_trace;
        fprintf_s(traceinst_file, "%08X %d %s %d %d %d %d\n",
            inst->raw_inst,
            i,
            inst_trace.unit_id.unit_id_str,
            inst_trace.cycle_issued,
            inst_trace.cycle_read_operands,
            inst_trace.cycle_execute_end,
            inst_trace.cycle_write_result);
    }
}


const char* get_op_name(opcode_e op_index) {
    return regs_str[op_index];
}

// will be written in every cycle
void write_traceunit_file(FILE* traceunit_file, int cycle, unit_t* trace_unit) {
    fprintf_s(traceunit_file, "%d %s %s %s %s %s %s %s %s\n",
        cycle,
        trace_unit->unit_id.unit_id_str,
        regs_str[trace_unit->Fi],
        regs_str[trace_unit->Fj],
        regs_str[trace_unit->Fk],
        trace_unit->Qj != NULL ? trace_unit->Qj->unit_id.unit_id_str : "-",
        trace_unit->Qk != NULL ? trace_unit->Qk->unit_id.unit_id_str : "-",
        trace_unit->Rj ? "Yes" : "No",
        trace_unit->Rj ? "Yes" : "No");
}