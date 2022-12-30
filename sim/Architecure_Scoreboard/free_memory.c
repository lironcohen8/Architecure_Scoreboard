#include "free_memory.h"

void free_units_memory(unit_t** op_units, config_t* config) {
    for (opcode_e operation = 0; operation < CONFIGURED_UNITS; operation++) {
        free(op_units[operation]);
    }
    free(op_units);
}

void free_address_buff(address_entry* buff) {
    free(buff);
}