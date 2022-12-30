#include <stdlib.h>
#include "global.h"

/* free all of the dynamically allocated memory for the units. */
void free_units_memory(unit_t** op_units, config_t* config);

/* free the dynamically allocated memory for the array of current writing addresses. */
void free_current_writing_addresses_array(uint32_t* current_writing_addresses);