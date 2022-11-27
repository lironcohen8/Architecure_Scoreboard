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

void load_configuration(FILE* cfg_file, config_t* config);