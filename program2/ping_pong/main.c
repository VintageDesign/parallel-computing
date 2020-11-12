#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>


int main(int argc, char * argv[])
{
   int rec_count = atoi(argv[1]);
   int byte_size = atoi(argv[2]);
   int num_tasks = 0;
   int name_len = 0;
   int rank = 0;

   struct timeval start;
   struct timeval stop;

   char host_name[MPI_MAX_PROCESSOR_NAME];
   char rec_host_name[2][MPI_MAX_PROCESSOR_NAME];

   char * message = malloc(sizeof(char) * byte_size);
   MPI_Status status;
   MPI_Request request;
   MPI_Request send_request;

   MPI_Init(&argc, &argv);

   MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   MPI_Get_processor_name(host_name, &name_len);
   MPI_Gather(&host_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, &rec_host_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);

   if(rank == 0)
   {
      // Blocking
      gettimeofday(&start, NULL);
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Send(message, byte_size, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
         MPI_Recv(message, byte_size, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
      }
      gettimeofday(&stop, NULL);
      printf("%lu\n", ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)/(2 *rec_count));

      // Non Blocking
      gettimeofday(&start, NULL);
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Isend(message, byte_size, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &send_request);
         MPI_Irecv(message, byte_size, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &request);
         MPI_Wait(&send_request, &status);
         MPI_Wait(&request, &status);
      }
      gettimeofday(&stop, NULL);
      printf("%lu\n", ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)/(2 * rec_count));

      // Send/Recv
      gettimeofday(&start, NULL);
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Sendrecv(message, byte_size, MPI_CHAR, 1, 1, message, byte_size, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
      }
      gettimeofday(&stop, NULL);

      printf("%lu\n", ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)/(2 * rec_count));
   }

   else{
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Recv(message, byte_size, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
         MPI_Send(message, byte_size, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
      }
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Irecv(message, byte_size, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &send_request);
         MPI_Isend(message, byte_size, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &request);
         MPI_Wait(&send_request, &status);
         MPI_Wait(&request, &status);
      }
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Sendrecv(message, byte_size, MPI_CHAR, 0, 1, message, byte_size, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
      }
   }

   MPI_Finalize();


}
