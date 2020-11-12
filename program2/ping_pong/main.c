#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char * argv[])
{
   int num_tasks = 0;
   int name_len = 0;
   int rank = 0;
   char host_name[MPI_MAX_PROCESSOR_NAME];
   char rec_host_name[2][MPI_MAX_PROCESSOR_NAME];



   MPI_Init(&argc, &argv);

   MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   MPI_Get_processor_name(host_name, &name_len);
   MPI_Gather(&host_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, &rec_host_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);

   if(rank == 0)
   {
      printf("Local Name: %s\n", host_name);
      printf("Remote name: %s\n", rec_host_name[1]);

   }

   MPI_Finalize();


}
