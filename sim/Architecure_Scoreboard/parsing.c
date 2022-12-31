#include "parsing.h"

/* opens the file and makes sure it succeeds. */
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

/* closes the file and makes sure it succeeds. */
void close_file(FILE* file, char const* file_name) {
    int result;
    result = fclose(file);
    if (result != 0) {
        printf("Error closing file %s\n", file_name);
        exit(0);
    }
}