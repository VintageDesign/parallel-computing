#include "mpi.h"
#include <stdio.h>
#include "utils.h"

int ** get_board(int rank, int * row, int ** board, command_args_t config)
{
   MPI_Status status;

   if(rank == 0)
   {
      for(int i = 1; i < config.row_size; i++)
      {
         MPI_Recv(board[i], config.col_size, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
      }
   }
   else
   {
      MPI_Send(row, config.col_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }

   return board;
}

int main(int argc, char * argv[]){
   command_args_t arguments = parse_args(argc, argv);
   int num_tasks = 0;
   int rank = 0;
   MPI_Status status;
   MPI_Init(&argc, &argv);

   MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   int * row;
   int ** board;

   if(rank == 0)
   {
      board = malloc(sizeof(int) * arguments.row_size );
      for( int i = 0; i < arguments.row_size ; i++)
      {
         board[i] = malloc(sizeof(int) * arguments.col_size);
      }
   }

   row = malloc(sizeof(int) * arguments.col_size);
   MPI_Barrier(MPI_COMM_WORLD);

   // Init board
   if(rank == 0)
   {
      int rand_x;
      int rand_y;
      for(int index = 0; index < arguments.starting_cells; index++)
      {
         do {
            rand_x = rand() % arguments.row_size;
            rand_y = rand() % arguments.col_size;
         }
         while(board[rand_x][rand_y] != 0);

         board[rand_x][rand_y] = 1;
      }

      row = board[0];

      for(int index = 1; index < arguments.row_size; index++)
      {
         MPI_Send(board[index], arguments.col_size, MPI_INT, index, 0, MPI_COMM_WORLD);
      }
   }

   else
   {
      MPI_Recv(row, arguments.col_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      printf("%d got Row\n",rank);
   }

   board = get_board(rank, row, board, arguments);
   MPI_Barrier(MPI_COMM_WORLD);

   if(rank == 0)
   {
      print_board(board, arguments);
   }
   MPI_Barrier(MPI_COMM_WORLD);


   MPI_Finalize();

}
