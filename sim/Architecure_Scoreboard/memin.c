#include "memin.h"

void load_memin(FILE* memin_file, uint32_t* imem_buff) {
    char line_buffer[MEMORY_LINE_LEN + 2];
    int memory_count = 0;
    uint32_t curr_mem;
    /* stops when either (n-1) characters are read, or /n is read
    We want to read the /n char so it won't get in to the next line */
    while (fgets(line_buffer, MEMORY_LINE_LEN + 2, memin_file) != NULL) {
        sscanf_s(line_buffer, "%lX", &curr_mem);
        imem_buff[memory_count++] = curr_mem;
    }
}