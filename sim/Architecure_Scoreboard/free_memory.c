#include "free_memory.h"

void free_units_memory(unit_t** op_units, config_t* config) {
    for (opcode_e operation = 0; operation < CONFIGURED_UNITS; operation++) {
        uint32_t num_units = config->units[operation].num_units;
        for (uint32_t i = 0; i < num_units; i++) {
            free(&op_units[operation][i]);
        }
        free(op_units[operation]);
    }
}