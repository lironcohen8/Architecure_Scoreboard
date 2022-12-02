#pragma once

#include <stdlib.h>
#include <stdio.h>

#define MAX_ASSEMBLY_LINES          (4096)
#define INSTRUCTION_LINE_LEN		(8)

/* File opening and validation used for all files */
FILE* open_and_validate_file(char const* file_name, char const* perms);