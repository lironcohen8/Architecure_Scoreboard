#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "parsing.h"
#include "init.h"
#include "scoreboard.h"
#include "output_files.h"
#include "free_memory.h"

int main(int argc, char* argv[]) {
    // Open the input and output files and validate them
    FILE* cfg_file = open_and_validate_file(argv[1], "r");
    FILE* memin_file = open_and_validate_file(argv[2], "r");
    FILE* memout_file = open_and_validate_file(argv[3], "w");
    FILE* regout_file = open_and_validate_file(argv[4], "w");
    FILE* traceinst_file = open_and_validate_file(argv[5], "w");
    FILE* traceunit_file = open_and_validate_file(argv[6], "w");
    
    simulation_t* simulation = get_simulation();

    init_simulation(simulation, memin_file, cfg_file);

    // First cycle is only first fetch
    if (fetch()) {
        // Advance PC after succesfull fetch
        advance_pc();
        simulation->clock_cycle++;
    }

    // The main loop
    while (!is_halted() || (num_of_active_instructions() > 0)) {
        if (fetch()) {
            // Advance PC after succesfull fetch
            advance_pc();
        }
        execute_all(simulation);
        issue();

        if (simulation->trace_unit->busy.old_val) {
            write_traceunit_file(traceunit_file, simulation->clock_cycle, simulation->trace_unit);
        }

        cycle_end();
        simulation->clock_cycle++;
    }

    // Write output files
    write_memout_file(memout_file, simulation->memory);
    write_regout_file(regout_file, simulation->regs);
    write_traceinst_file(traceinst_file, simulation->issued_inst, simulation->issued_cnt);

    // Free memory
    free_units_memory(simulation->op_units, &simulation->config);
    free_address_buff(simulation->active_st_addresses);

    // Close files
    close_file(cfg_file, argv[1]);
    close_file(memin_file, argv[2]);
    close_file(memout_file, argv[3]);
    close_file(regout_file, argv[4]);
    close_file(traceinst_file, argv[5]);
    close_file(traceunit_file, argv[6]);
}