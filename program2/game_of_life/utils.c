#include "utils.h"


command_args_t parse_args(int argc, char * argv[])
{
    command_args_t retval;

    if (argc == 6)
    {
        retval.row_size = atoi(argv[1]);
        retval.col_size = atoi(argv[2]);
        retval.epochs = atoi(argv[3]);
        retval.starting_cells = atoi(argv[4]);
        retval.print_generation = atoi(argv[5]);
    }
    else
    {
        printf("Failed to parse arguments! Arguments should look like:\n");
        printf("./game_of_life <n> <m> <j> <i> <k>\n Exiting...\n");
        exit(1);
    }

    return retval;
}
