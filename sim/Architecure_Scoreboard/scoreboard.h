#pragma once
#include "global.h";
#include "inst_queue.h"
#include "memin.h"

bool fetch(int pc);

bool issue();

bool read_operands(unit_t* assigned_unit);

bool exec();

bool write_result();