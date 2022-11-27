#include "parsing.h"


FILE* open_and_validate_file(char const* file_name, char const* perms) {
    FILE* file;
    fopen_s(&file, file_name, perms);
    if (file == NULL) {
        printf("Error opening file %s\n", file_name);
        exit(0);
    }
    else {
        return file;
    }
}