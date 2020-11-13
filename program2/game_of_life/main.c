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
      board[0] = row;
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
      for( int i = 0; i < arguments.row_size; i++)
      {
         board[i] = malloc(sizeof(int) * arguments.col_size);
         for(int j = 0; j < arguments.col_size; j++)
         {
            board[i][j] = 0;
         }
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

      for( int i = 0; i < arguments.col_size; i++)
      {
         row[i] = (board[0][i] == 1) ? 1 : 0;
      }

      for(int index = 1; index < arguments.row_size; index++)
      {
         MPI_Send(board[index], arguments.col_size, MPI_INT, index, 0, MPI_COMM_WORLD);
      }
   }

   else
   {
      MPI_Recv(row, arguments.col_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
   }

   board = get_board(rank, row, board, arguments);
   MPI_Barrier(MPI_COMM_WORLD);

   if(rank == 0)
   {
      printf("Starting Board:");
      print_board(board, arguments);
   }
   MPI_Barrier(MPI_COMM_WORLD);

   int * ignore_me = malloc(sizeof(int) * arguments.col_size);
   int * upper_row = malloc(sizeof(int) * arguments.col_size);
   int * lower_row = malloc(sizeof(int) * arguments.col_size);
   int * new_row   = malloc(sizeof(int) * arguments.col_size);

   for(int iter = 0; iter < arguments.epochs; iter++)
   {
      for( int i = 0; i < arguments.col_size; i++)
      {
         ignore_me[i] = 0;
         upper_row[i] = 0;
         lower_row[i] = 0;
         new_row[i] = 0;
      }


      if(rank != 0 && rank != arguments.row_size - 1)
      {
         MPI_Sendrecv(row, arguments.col_size, MPI_INT, rank - 1, 1, lower_row, arguments.col_size, MPI_INT, rank + 1, 1, MPI_COMM_WORLD, &status);
         MPI_Sendrecv(row, arguments.col_size, MPI_INT, rank + 1, 1, upper_row, arguments.col_size, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
      }
      else if (rank == 0)
      {
         MPI_Send(row, arguments.col_size, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
         MPI_Recv(lower_row, arguments.col_size, MPI_INT, rank + 1, 1, MPI_COMM_WORLD, &status);
         for( int i = 0; i < arguments.col_size; i++)
         {
   //         printf("Upper %d: %d\n", i,upper_row[i]);
    //        printf("Row %d: %d\n", i, row[i]);
     //       printf("Lower %d: %d\n", i,lower_row[i]);
         }
      }

      else if (rank == arguments.row_size - 1)
      {
         MPI_Send(row, arguments.col_size, MPI_INT, rank - 1, 1, MPI_COMM_WORLD);
         MPI_Recv(upper_row, arguments.col_size, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
      }



      for(int cell_iter = 0; cell_iter < arguments.col_size; cell_iter++)
      {
         int sum = 0;
         for(int cell_col = cell_iter - 1; cell_col <= cell_iter + 1; cell_col++)
         {
            if(cell_col >= 0 && cell_col < arguments.col_size)
            {
               sum += upper_row[cell_col];
               sum += lower_row[cell_col];
               if(cell_col != cell_iter)
               {
                  sum += row[cell_col];
               }
            }
         }

         //printf("%d, %d: %d\n", rank, cell_iter, sum);
         if(row[cell_iter] == 1 && (sum == 2 || sum == 3))
         {
            new_row[cell_iter] = 1;
         }
         else if(row[cell_iter] == 0 && sum == 3)
         {
            new_row[cell_iter] = 1;
         }
         else
         {
            new_row[cell_iter] = 0;
         }



      }

      for( int i = 0; i < arguments.col_size; i++)
      {
      row[i] = new_row[i];
      }


      board = get_board(rank, row, board, arguments);
      MPI_Barrier(MPI_COMM_WORLD);

      if(rank == 0 && (iter + 1) % arguments.print_generation == 0)
      {
         printf("Board: %d", iter + 1);
         print_board(board, arguments);
      }
      MPI_Barrier(MPI_COMM_WORLD);


   }

   MPI_Finalize();

}
