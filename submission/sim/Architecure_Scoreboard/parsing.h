#pragma once

#include <stdlib.h>
#include <stdio.h>

#define MEMORY_SIZE         (4096)
#define MEMORY_LINE_LEN		(8)

/* opens the file and makes sure it succeeds. */
FILE* open_and_validate_file(char const* file_name, char const* perms);


/* closes the file and makes sure it succeeds. */
void close_file(FILE* file, char const* file_name);