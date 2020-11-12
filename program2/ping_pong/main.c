#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>


int main(int argc, char * argv[])
{
   int rec_count = atoi(argv[1]);
   int num_tasks = 0;
   int name_len = 0;
   int rank = 0;

   struct timeval start;
   struct timeval stop;

   char host_name[MPI_MAX_PROCESSOR_NAME];
   char rec_host_name[2][MPI_MAX_PROCESSOR_NAME];

   char message[10000];
   MPI_Status status;
   MPI_Request request;



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
         MPI_Send(&message, 1, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
         MPI_Recv(&message, 1, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
      }
      gettimeofday(&stop, NULL);
      printf("%lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

      // Non Blocking
      gettimeofday(&start, NULL);
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Isend(&message, 1, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &request);
         MPI_Irecv(&message, 1, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &request);
      }
      gettimeofday(&stop, NULL);
      printf("%lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

      // Send/Recv
      gettimeofday(&start, NULL);
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Sendrecv(&message, 1, MPI_CHAR, 1, 1, &message, 1, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
      }
      gettimeofday(&stop, NULL);
      printf("%lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

   }

   else{
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Recv(&message, 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
         MPI_Send(&message, 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
      }
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Irecv(&message, 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &request);
         MPI_Isend(&message, 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &request);
      }
      for(int i = 0; i < rec_count; i++)
      {
         MPI_Sendrecv(&message, 1, MPI_CHAR, 0, 1, &message, 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
      }
   }

   MPI_Finalize();


}
