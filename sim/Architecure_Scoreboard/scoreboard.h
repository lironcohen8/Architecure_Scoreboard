#pragma once
#include "global.h"
#include "uint32_queue.h"
#include "memin.h"
#include "parsing.h"

#define ADDRESS_INVALID		0xFFFFFFFF

/* Defines the simulation. */
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
	uint32_queue_t	inst_queue;

	/* Registers values and status */
	reg_val_status  regs[REGS_NUM];

	/* Configuration form the cfg.txt file */
	config_t		config;

	/* Operational units as defined in config */
	unit_t**		op_units;

	/* Clock cycle counter. */
	uint32_t		clock_cycle;

	/* Pointer to the trace unit. */
	unit_t*			trace_unit;

	/* The active store addresses array. */
	address_entry*	active_st_addresses;

	/* The active store addresses array size. */
	uint32_t		active_st_addresses_size;

	/* Current pc, updated after succesful fetch */
	uint32_t		pc;

	/* Is halted. */
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

void execute_all(simulation_t* simulation);

void cycle_end();