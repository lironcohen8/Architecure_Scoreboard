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
    unit_id_t           unit_id;
    struct unit_t*      unit;
    int                 cycle_issued;
    int                 cycle_read_operands;
    int                 cycle_execute_end;
    int                 cycle_write_result;
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

typedef struct {
    bool old_val;
    bool new_val;
}bool_ff;

typedef struct {
    struct unit_t* old_val;
    struct unit_t* new_val;
}unit_ptr_ff;

typedef struct {
    reg_e old_val;
    reg_e new_val;
}reg_ff;

typedef struct unit_t {
    unit_id_t       unit_id;
    reg_ff           Fi;
    reg_ff           Fj;
    reg_ff           Fk;
    unit_ptr_ff      Qj;
    unit_ptr_ff      Qk;
    bool_ff          Rj;
    bool_ff          Rk;
    bool_ff          busy;
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

typedef struct float_uint_ff {
    float_uint old_val;
    float_uint new_val;
}float_uint_ff;

typedef struct {
    /* value representation both as uint32 and as float 
    when reading from memory values are represented as uint32 but when performing ALU 
    operation we will use the float representation */
    float_uint_ff   value;
    /* If not null, the relevant unit for updating this register */
    unit_ptr_ff            status;
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