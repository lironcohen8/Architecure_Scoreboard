#pragma once

#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CONFIGURATION_MAX_LINE_LEN		50
#define CONFIGURATION_MAX_OPCODE_LEN	10

typedef enum {
	UNITS_NUM,
	UNIT_DELAY,
	TRACE_UNIT,
} param_type_e;

/* gets a configuration file and a configuration struct.
Reads the lines of the files, and parses them into
the configuration struct. */
void load_configuration(FILE* cfg_file, config_t* config);