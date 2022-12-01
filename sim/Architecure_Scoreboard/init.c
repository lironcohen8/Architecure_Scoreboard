#include "init.h"


unit_t** init_units(config_t* config) {
    unit_t** op_units = (unit_t**)malloc(CONFIGURED_UNITS * sizeof(unit_t*));
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
            }
        }
    }

    return op_units;

exit:
    printf("Error when malloc");
    exit(0);
}

void init_regs_status_values(reg_val_status* regs) {
    for (reg_e reg_index = F0; reg_index < REGS_NUM; reg_index++) {
        regs[reg_index].value.float_val = (float)reg_index;
        regs[reg_index].status = NULL;
    }
}