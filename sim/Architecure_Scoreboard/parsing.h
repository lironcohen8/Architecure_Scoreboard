#pragma once

#include <stdlib.h>
#include <stdio.h>


/* File opening and validation used for all files */
FILE* open_and_validate_file(char const* file_name, char const* perms);