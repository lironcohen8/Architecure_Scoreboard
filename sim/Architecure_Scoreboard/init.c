#include "init.h"
#include "scoreboard.h"
#include "cfg.h"

static void create_unit_id_str(unit_id_t* id) {
    sprintf_s(id->unit_id_str, UNIT_ID_STR_LEN, "%s%u", opcode_str[id->operation], id->index);
}

/* for every operation, allocate the required memory according to num of units in configuration. 
set up the unit values (index, operation, initial state as IDLE…). */
static unit_t** init_units(config_t* config) {
    unit_t** op_units = (unit_t**)malloc(CONFIGURED_UNITS * sizeof(unit_t*));
    unit_id_t* trace_unit = &config->trace_unit;

    if (op_units == NULL) {
        goto exit;
    }

    for (opcode_e operation = 0; operation < CONFIGURED_UNITS; operation++) {
        uint32_t num_units = config->units[operation].num_units;
        op_units[operation] = (unit_t*)calloc(num_units, sizeof(unit_t));
        if (op_units[operation] == NULL) {
            goto exit;
        }
        else {
            for (uint32_t i = 0; i < num_units; i++) {
                op_units[operation][i].unit_id.index = i;
                op_units[operation][i].unit_id.operation = operation;
                op_units[operation][i].unit_state = IDLE;
                op_units[operation][i].busy.new_val = false;
                op_units[operation][i].executed = false;
                create_unit_id_str(&op_units[operation][i].unit_id);
                if (operation == trace_unit->operation && i == trace_unit->index) {
                    create_unit_id_str(trace_unit);
                }
            }
        }
    }

    return op_units;

exit:
    printf("Error when malloc");
    exit(0);
}

/* set up the initial registers values to be as their index.
set up their status to be NULL (not waiting to any unit to write the value). */
static void init_regs_status_values(reg_val_status* regs) {
    for (reg_e reg_index = F0; reg_index < REGS_NUM; reg_index++) {
        regs[reg_index].value.new_val.float_val = (float)reg_index;
        regs[reg_index].status.new_val = NULL;
    }
}

/* allocating the array of the current writing address (updated every cycle) as the total number of LD and ST units */

static void create_current_writing_addresses(simulation_t* simulation, int num_of_st_units) {
    simulation->active_st_addresses =  (address_entry*)malloc(num_of_st_units * sizeof(address_entry));
    if (simulation->active_st_addresses == NULL) {
        printf("Error when malloc");
        exit(0);
    }
    for (int i = 0; i < num_of_st_units; i++) {
        simulation->active_st_addresses[i].addr = ADDRESS_INVALID;
    }
    simulation->active_st_addresses_size = num_of_st_units;
}


/* init everything for simulation struct from memory input file and config unput file. */
void init_simulation(simulation_t* simulation, FILE* memin_file, FILE* cfg_file) {
    // memory is loaded from memin.txt
    load_memin(memin_file, simulation->memory);

    simulation->clock_cycle = 0;
    // Configuration is loaded from cfg.txt

    load_configuration(cfg_file, &simulation->config);
    simulation->halted = false;
    simulation->pc = 0;

    simulation->issued_cnt = 0;
    simulation->finished_cnt = 0;
    // Malloc memory for units according to config
    simulation->op_units = init_units(&simulation->config);
    simulation->trace_unit = &simulation->op_units[simulation->config.trace_unit.operation][simulation->config.trace_unit.index];

    create_current_writing_addresses(simulation, simulation->config.units[ST].num_units);
    init_regs_status_values(simulation->regs);
    init_queue(&simulation->inst_queue);
}