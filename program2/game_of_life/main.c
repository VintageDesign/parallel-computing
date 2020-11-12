#include <stdio.h>
#include "utils.h"
#include "serial.h"

int main(int argc, char * argv[]){
    command_args_t arguments = parse_args(argc, argv);
    //printf("Arguments: %d, %d, %d, %d, %d\n", arguments.row_size, arguments.col_size, arguments.epochs, arguments.starting_cells, arguments.print_generation);

    int ** board = generate_game(arguments);

    printf("Starting Board");
    print_board(board, arguments);

    for(int generation = 0; generation < arguments.epochs; generation ++)
    {
        board = run_game(board, arguments);

        if(generation % arguments.print_generation == 0)
        {
            printf("Generation: %d", generation);
            print_board(board, arguments);
        }

    }




}
