#pragma once

#include <stdint.h>
#include <stdbool.h>

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
} reg_e;

typedef enum {
    READ_OPERANDS,
    EXEC,
    WRITE_RESULT
} unit_state_e;

typedef struct {
    unit_id_t   unit_id;
    unit_t*     unit;
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

typedef struct {
    uint32_t    num_units;
    uint32_t    unit_delay_cycles;
} op_config_t;

typedef struct {
    opcode_e    operation;
    uint32_t    index;
} unit_id_t;

typedef struct unit_t {
    unit_id_t       unit_id;
    reg_e           Fi;
    reg_e           Fj;
    reg_e           Fk;
    struct unit_t*  Qj;
    struct unit_t*  Qk;
    bool            Rj;
    bool            Rk;
    bool            busy;
    unit_state_e    unit_state;
    int             exec_cnt;
} unit_t;

typedef struct {
    op_config_t     units[CONFIGURED_UNITS];
    unit_id_t       trace_unit;
} config_t;