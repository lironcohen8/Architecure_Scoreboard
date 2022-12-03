#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CONFIGURED_UNITS            (6)
#define UNIT_ID_STR_LEN             (6)

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
    F15,
    REGS_NUM
} reg_e;

typedef enum {
    IDLE,
    READ_OPERANDS,
    EXEC,
    WRITE_RESULT
} unit_state_e;

typedef struct {
    uint32_t    num_units;
    uint32_t    unit_delay_cycles;
} op_config_t;

typedef struct {
    opcode_e    operation;
    uint32_t    index;
    char        unit_id_str[UNIT_ID_STR_LEN];
} unit_id_t;

typedef struct {
    unit_id_t     unit_id;
    struct unit_t*     unit;
    int         cycle_issued;
    int         cycle_read_operands;
    int         cycle_execute_end;
    int         cycle_write_result;
} inst_trace_t;

typedef struct {
    opcode_e        opcode;
    reg_e           dst;
    reg_e           src0;
    reg_e           src1;
    int             imm;
    inst_trace_t    inst_trace;
    uint32_t        raw_inst;
} inst_t;

typedef struct unit_t {
    unit_id_t       unit_id;
    reg_e           Fi;
    reg_e           Fj;
    reg_e           Fk;
    struct unit_t* Qj;
    struct unit_t* Qk;
    bool            Rj;
    bool            Rk;
    bool            busy;
    unit_state_e    unit_state;
    int             exec_cnt;
    inst_t*         active_instruction;
} unit_t;

typedef struct {
    op_config_t     units[CONFIGURED_UNITS];
    unit_id_t       trace_unit;
} config_t;

typedef union {
    uint32_t uint32_val;
    float    float_val;
} float_uint;

typedef struct {
    /* value representation both as uint32 and as float 
    when reading from memory values are represented as uint32 but when performing ALU 
    operation we will use the float representation */
    float_uint   value;
    /* If not null, the relevant unit for updating this register */
    unit_t*     status;
} reg_val_status;

static const char* regs_str[] = {
    "F0",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
};

static const char* opcode_str[] = {
    "LD",
    "ST",
    "ADD",
    "SUB",
    "MULT",
    "DIV",
    "HALT"
};