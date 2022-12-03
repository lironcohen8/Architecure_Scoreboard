#pragma once
#include "global.h"
#include "inst_queue.h"
#include "memin.h"
#include "parsing.h"

typedef struct {
	/* Memory pointer */
	uint32_t*		memory;

	/* Issued instructions array */
	inst_t          issued_inst[MEMORY_SIZE];
	/* Number of issued instructions */
	uint32_t		issued_cnt;

	/* Instruction queue */
	inst_queue_t	inst_queue;

	/* Registes values and status */
	reg_val_status  regs[REGS_NUM];

	/* Configuration form the cfg.txt file */
	config_t		config;

	/* Operational units as defined in config */
	unit_t**		op_units;

	uint32_t		clock_cycle;

	/* Current pc, updated after succesful fetch */
	uint32_t		pc;

	bool			halted;
} scoreboard_t;

bool fetch(scoreboard_t* scoreboard);

bool issue(scoreboard_t* scoreboard);

bool read_operands(unit_t* assigned_unit, scoreboard_t* scoreboard);

bool exec(unit_t* assigned_unit, scoreboard_t* scoreboard);

bool write_result(unit_t* assigned_unit, scoreboard_t* scoreboard);

void init_scoreboard(scoreboard_t* scoreboard, config_t* config, uint32_t* memory_ptr);