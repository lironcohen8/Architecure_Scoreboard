#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "parsing.h"
#include "init.h"
#include "scoreboard.h"

int main(int argc, char* argv[]) {
    FILE* mem_file = open_and_validate_file(argv[2], "r");
    
    FILE* cfg_file = open_and_validate_file(argv[1], "r");
    
    init_simulation(get_simulation(), mem_file, cfg_file);

    while (!is_halted() || (num_of_active_instructions() > 0)) {
        if (fetch()) {
            // Advance PC after succesfull fetch
            advance_pc();
        }

        // TODO...


        cycle_end();
    }
}