#pragma once
#include "global.h"
#include "inst_queue.h"
#include "memin.h"
#include "parsing.h"

typedef struct {
	/* Memory loaded from memin.txt */
	uint32_t		memory[MEMORY_SIZE];

	/* Issued instructions array */
	inst_t          issued_inst[MEMORY_SIZE];
	/* Number of issued instructions */
	uint32_t		issued_cnt;

	/* Number of finished instructions */
	uint32_t		finished_cnt;

	/* Instruction queue */
	inst_queue_t	inst_queue;

	/* Registers values and status */
	reg_val_status  regs[REGS_NUM];

	/* Configuration form the cfg.txt file */
	config_t		config;

	/* Operational units as defined in config */
	unit_t**		op_units;

	uint32_t		clock_cycle;

	/* Current pc, updated after succesful fetch */
	uint32_t		pc;

	bool			halted;
} simulation_t;

bool fetch();

bool issue();

bool read_operands(unit_t* assigned_unit);

bool exec(unit_t* assigned_unit);

bool write_result(unit_t* assigned_unit);

uint32_t num_of_active_instructions();

bool is_halted();

void advance_pc();

simulation_t* get_simulation();