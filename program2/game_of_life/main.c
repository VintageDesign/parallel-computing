#include "mpi.h"
#include <stdio.h>
#include "utils.h"

int main(int argc, char * argv[]){
   command_args_t arguments = parse_args(argc, argv);
   int num_tasks = 0;
   int rank = 0;
   int tile_value = 0;
   MPI_Status status;

   int ** board;

   // One bigass vector for the game of life
   board = malloc(sizeof(int) * arguments.row_size * arguments.col_size);
   for( int i = 0; i <= arguments.row_size * arguments.col_size; i++)
   {
      board[i] = malloc(sizeof(int));
   }

   MPI_Init(&argc, &argv);

   MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
   printf("%d\n", num_tasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   // Initialize the board
   if( rank == 0)
   {
      int live_cell_count = 0;
      int value = 0;
      for( int i = 1; i <= arguments.row_size * arguments.col_size; i++)
      {
         if(live_cell_count <= arguments.starting_cells)
         {
            value = rand() % 2;
            live_cell_count += value;
         }
         MPI_Send(&value, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      }

   }
   else
   {
      MPI_Recv(&tile_value, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
   }

   MPI_Barrier(MPI_COMM_WORLD);


   MPI_Gather(&tile_value, 1, MPI_INT, &board, (arguments.row_size * arguments.col_size), MPI_INT, 0, MPI_COMM_WORLD);



   //    int ** board = generate_game(arguments);


   if( rank == 0)
   {
      printf("Starting Board");
      print_board(board, arguments);
   }

   MPI_Barrier(MPI_COMM_WORLD);
   /*
   for(int generation = 0; generation < arguments.epochs; generation ++)
   {
      board = run_game(board, arguments);

      if(generation % arguments.print_generation == 0)
      {
         printf("Generation: %d", generation);
         print_board(board, arguments);
      }

   }

   */

   MPI_Finalize();

}
