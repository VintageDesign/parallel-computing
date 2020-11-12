#include "serial.h"

int ** generate_game(command_args_t config){
    int rand_x = 0;
    int rand_y = 0;

    int ** retval = generate_board(config);

    for(int index = 0; index < config.starting_cells; index++)
    {
        do {
            rand_x = rand() % config.row_size;
            rand_y = rand() % config.col_size;
        }
        while(retval[rand_x][rand_y] != 0);

        retval[rand_x][rand_y] = 1;
    }
    return retval;
}

int ** run_game(int** board, command_args_t config)
{
    int sum = 0;
    int ** new_board = generate_board(config);
    for(int row_index = 0; row_index < config.row_size; row_index++)
    {
        for(int col_index = 0; col_index < config.col_size; col_index++)
        {
            // Cell Wise checking
            sum = 0;
            for(int cell_row = row_index-1; cell_row <= row_index + 1; cell_row++)
            {
                if(cell_row < config.row_size && cell_row >= 0)
                {
                    for(int cell_col = col_index -1; cell_col <= col_index + 1; cell_col++)
                    {
                        if(cell_col < config.col_size && cell_col >= 0)
                        {
                            if(!(cell_col == col_index && cell_row == row_index))
                            {
                                sum += board[cell_row][cell_col];
                            }
                        }
                    }
                }
            }

            //if(sum != 0)
            //printf("Cell (%d, %d): %d\n", row_index, col_index, sum);

            // Set the next board up
            if (board[row_index][col_index] == 1 && (sum == 2 || sum == 3))
            {
                new_board[row_index][col_index] = 1;
            }
            else if(board[row_index][col_index] == 0 && sum == 3)
            {
                new_board[row_index][col_index] = 1;
            }

        }
    }

    return new_board;

}