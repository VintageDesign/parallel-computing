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

int ** generate_board(command_args_t config)
{
    int ** retval = malloc(sizeof(int *) * config.row_size);
    for(int index = 0; index < config.row_size; index++)
    {
        retval[index] = malloc(sizeof(int) * config.col_size);
    }
    return retval;
}

void print_board(int ** board, command_args_t config)
{
    printf("\n");
    char character = ' ';
    for(int row_index = 0; row_index < config.row_size; row_index++)
    {
        for(int col_index = 0; col_index < config.col_size; col_index++)
        {
            character = (board[row_index][col_index] == 1) ? 'O' : '-';
            printf("| %c ", character);
        }
        printf("|\n");
    }
}
