#include "output_files.h"

// will be written in the end of execution
void write_memout_file(FILE* memout_file, uint32_t* g_mem_arr) {
    for (int i = 0; i < MEMORY_SIZE; i++) { // TODO consider changing to memory_count
        fprintf_s(memout_file, "%08x\n", g_mem_arr[i]);
    }
}

// will be written in the end of execution
void write_regout_file(FILE* regout_file, reg_val_status* g_regs) {
    for (int i = 0; i < REGS_NUM; i++) {
        fprintf_s(regout_file, "%f\n", g_regs[i].value.new_val.float_val);
    }
}

// will be written in the end of execution
void write_traceinst_file(FILE* traceinst_file, inst_t* issued_inst_arr, uint32_t issued_count) {
    for (uint32_t i = 0; i < issued_count; i++) {
        inst_trace_t inst_trace = issued_inst_arr[i].inst_trace;
        fprintf_s(traceinst_file, "%08x %d %s %d %d %d %d\n",
            issued_inst_arr[i].raw_inst,
            i,
            inst_trace.unit_id.unit_id_str,
            inst_trace.cycle_issued,
            inst_trace.cycle_read_operands,
            inst_trace.cycle_execute_end,
            inst_trace.cycle_write_result);
    }
}

// will be written in every cycle
void write_traceunit_file(FILE* traceunit_file, int cycle, unit_t* trace_unit) {
    fprintf_s(traceunit_file, "%d %s %s %s %s %s %s %s %s\n",
        cycle,
        trace_unit->unit_id.unit_id_str,
        regs_str[trace_unit->Fi.old_val],
        regs_str[trace_unit->Fj.old_val],
        regs_str[trace_unit->Fk.old_val],
        trace_unit->Qj.old_val != NULL ? trace_unit->Qj.old_val->unit_id.unit_id_str : "-",
        trace_unit->Qk.old_val != NULL ? trace_unit->Qk.old_val->unit_id.unit_id_str : "-",
        trace_unit->Rj.old_val ? "Yes" : "No",
        trace_unit->Rk.old_val ? "Yes" : "No");
}