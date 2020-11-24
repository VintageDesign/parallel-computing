#include <sys/time.h>
#include <time.h>

#include "utils.h"





int main(int argc, char ** argv)
{
   int num_tasks = 0;
   int rank = 0;
   int query_size;
   double * query_features;

   dataset_t personal_dataset;
   MPI_Status status;

   MPI_Init(&argc, &argv);

   MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   if(rank == 0)
   {
      cmd_arguments_t args = parse_cmd_args(argc, argv);

      printf("Parsing Dataset...\n");
      dataset_t data_set = parse_csv(args.csv_filename);
      printf("Parsing Query...\n");
      dataset_t tmp = parse_csv(args.query_filename);

      int task_size = data_set.count / num_tasks;
      int send_size = task_size;
      int rem = data_set.count % num_tasks;

      for(int i = 0; i < num_tasks; i++)
      {

         printf("Sending Data for task: %d\n", i);
         if( i == num_tasks - 1)
            send_size += rem; // Add the remainder of the data to the last task.
         send_partition(i, send_size, task_size, data_set);
         MPI_Send((const void *) &tmp.feature_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
         MPI_Send((const void *) tmp.data[0].features, tmp.feature_count, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);

      }

   }

   MPI_Recv(&personal_dataset.count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
   //printf("Rank %d's personal datasize: %d\n", rank, personal_dataset.count);

   MPI_Recv(&personal_dataset.feature_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
   //printf("Rank %d's feature count: %d\n", rank, personal_dataset.feature_count);

   personal_dataset.data = malloc(personal_dataset.count * sizeof(datapoint_t));
   for(int i = 0; i < personal_dataset.count; i++)
   {
      personal_dataset.data[i].features =
         malloc(personal_dataset.feature_count * sizeof(double));
   }
   for(int i = 0; i < personal_dataset.count; i++)
   {
      MPI_Recv(personal_dataset.data[i].features
             , personal_dataset.feature_count
             , MPI_DOUBLE
             , 0
             , 0
             , MPI_COMM_WORLD
             , &status);
   }
   MPI_Recv(&query_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
   query_features = malloc(query_size * sizeof(double));
   MPI_Recv(query_features, query_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);


   for( int point = 0; point < personal_dataset.count; point++)
   {
      for(int i = 0; i < query_size; i++)
      {
        personal_dataset.data[point].distance +=
           (personal_dataset.data[point].features[i] - query_features[i])
         * (personal_dataset.data[point].features[i] - query_features[i]);
      }

     personal_dataset.data[point].distance = sqrt(personal_dataset.data[point].distance);

   }

   // Odd Even sort in OMP

   // Reduce the data using a binary tree.
   // Run the Merge algorithm from merge sort
   //

   // Once at root, calculate the mode of the top K datapoints and print out the
   // class.
   MPI_Finalize();
}

