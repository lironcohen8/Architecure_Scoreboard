#include "cfg.h"

/* gets a parameter string and returns the relevant parameter type
it defines (unit num, unit delay or trace unit). */
static param_type_e cfg_str_param_type(char* param_str) {
    char* ret;
    ret = strstr(param_str, "units");
    if (ret != NULL) {
        /* this is a number of units paramater */
        return UNITS_NUM;
    }
    ret = strstr(param_str, "delay");
    if (ret != NULL) {
        /* this is a unit delay paramater */
        return UNIT_DELAY;
    }
    ret = strstr(param_str, "trace_unit");
    if (ret != NULL) {
        /* this is a trace unit paramater */
        return TRACE_UNIT;
    }
    
    printf("Ilegal configuration type %s\n", param_str);
    exit(1);
}

/* gets a parameter string and returns the relevant operation
it defines (add, sub, mult…). */
static opcode_e cfg_str_operation(char* param_str) {
    char* ret;
    ret = strstr(param_str, "add");
    if (ret != NULL) {
        return ADD;
    }
    ret = strstr(param_str, "sub");
    if (ret != NULL) {
        return SUB;
    }
    ret = strstr(param_str, "mul");
    if (ret != NULL) {
        return MULT;
    }
    ret = strstr(param_str, "div");
    if (ret != NULL) {
        return DIV;
    }
    ret = strstr(param_str, "ld");
    if (ret != NULL) {
        return LD;
    }
    ret = strstr(param_str, "st");
    if (ret != NULL) {
        return ST;
    }

    printf("Ilegal configuration type %s\n", param_str);
    exit(1);
}

/* gets a parameter string and takes the relevant number from the string
(without the definition and '=' sign). */
static int cfg_str_num_param(char* param_str) {
    int num_param;
    /* Configuration str in format <some_string> = <value> 
    We ignore the first string and the "=" string and take the value */
    char* num_param_str = strchr(param_str, '=') + 1;
    sscanf_s(num_param_str, " %d", &num_param);
    return num_param;
}

/* gets a parameter string and parses the trace unit operation
and index into the configuration. */
static void cfg_str_set_trace_unit(char* param_str, config_t* config) {
    char trace_unit_str[CONFIGURATION_MAX_OPCODE_LEN] = { 0 };
    char* num_param_str = strchr(param_str, '=') + 1;

    sscanf_s(num_param_str, " %s", trace_unit_str, CONFIGURATION_MAX_OPCODE_LEN);
    int l = strlen(trace_unit_str);
    for (int i = 0; i < l-1; i++)
    {
        trace_unit_str[i] = tolower(trace_unit_str[i]);
    }

    int unit_index = trace_unit_str[l - 1] - '0';

    config->trace_unit.index = unit_index;
    config->trace_unit.operation = cfg_str_operation(trace_unit_str);
}

/* gets a configuration file and a configuration struct.
Reads the lines of the files, and parses them into 
the configuration struct using the methods above. */
void load_configuration(FILE* cfg_file, config_t* config) {
    char line_buffer[CONFIGURATION_MAX_LINE_LEN];
    int instructions_count = 0;
    param_type_e curr_cfg_type;
    opcode_e curr_cfg_op;
    int cfg_num_param;

    while (fgets(line_buffer, CONFIGURATION_MAX_LINE_LEN, cfg_file) != NULL) {
        curr_cfg_type = cfg_str_param_type(line_buffer);
        if (curr_cfg_type != TRACE_UNIT) {
            curr_cfg_op = cfg_str_operation(line_buffer);
            cfg_num_param = cfg_str_num_param(line_buffer);
            if (curr_cfg_type == UNITS_NUM) {
                config->units[curr_cfg_op].num_units = cfg_num_param;
            }
            else {
                if (curr_cfg_type == UNIT_DELAY) {
                    config->units[curr_cfg_op].unit_delay_cycles = cfg_num_param;
                }
                // No else here since cfg_str_param_type would exit for illegal configuration param
            }
        }
        else {
            cfg_str_set_trace_unit(line_buffer, config);
        }
        
    }
}