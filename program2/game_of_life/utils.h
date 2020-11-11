#include <stdio.h>
#include <stdlib.h>

typedef struct command_args
{
    int row_size;
    int col_size;
    int epochs;
    int starting_cells;
    int print_generation;
} command_args_t;


command_args_t parse_args(int argc, char * argv[]);
