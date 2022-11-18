#include <stdint.h>

#define MAX_ASSEMBLY_LINES          (4096)
#define INSTRUCTION_LINE_LEN		(8)
#define CONFIGURED_UNITS            (6)

typedef enum {
    LD      = 0,
    ST      = 1,
    ADD     = 2,
    SUB     = 3,
    MULT    = 4,
    DIV     = 5,
    HALT    = 6,
    OPCODES_NUM
} opcode_e;

typedef enum {
    F0,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15
} cpu_reg_e;

typedef struct {
    opcode_e opcode;
    cpu_reg_e dst;
    cpu_reg_e src0;
    cpu_reg_e src1;
    int imm;

    uint32_t raw_cmd;
} asm_cmd_t;

typedef struct {
    opcode_e    operation;
    uint32_t    num_units;
    uint32_t    unit_delay_cycles;
} unit_config_t;

typedef struct {
    opcode_e    operation;
    uint32_t    index;
} op_unit_t;

typedef struct {
    unit_config_t   units[CONFIGURED_UNITS];
    op_unit_t       trace_unit;
} config_t;