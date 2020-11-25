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
   cmd_arguments_t args;

   if(rank == 0)
   {
      printf("Parsing Cmd Args...");
      args = parse_cmd_args(argc, argv);
      printf("Done!\n");


      printf("Parsing dataset...");
      dataset_t data_set = parse_csv(args.csv_filename);
      printf("Done!\n");
      printf("Parsing query...");
      dataset_t tmp = parse_csv(args.query_filename);
      printf("Done!\n");

      int task_size = data_set.count / num_tasks;
      int send_size = task_size;
      int rem = data_set.count % num_tasks;

      printf("Partitioning Data...");
      for(int i = 0; i < num_tasks; i++)
      {

         if( i == num_tasks - 1)
            send_size += rem; // Add the remainder of the data to the last task.
         send_partition(i, send_size, task_size, data_set);
         MPI_Send((const void *) &tmp.feature_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
         MPI_Send((const void *) tmp.data[0].features, tmp.feature_count, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);

      }
      printf("Done!\n");
      printf("Calculating Norms...");


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



#pragma omp paralllel for num_threads(omp_get_num_procs()) default(none)
   for( int point = 0; point < personal_dataset.count; point++)
   {

      personal_dataset.data[point].distance = 0;
      for(int i = 0; i < query_size; i++)
      {
        personal_dataset.data[point].distance +=
         ((personal_dataset.data[point].features[i] - query_features[i])
         * (personal_dataset.data[point].features[i] - query_features[i]));
      }

     personal_dataset.data[point].distance = sqrt(personal_dataset.data[point].distance);

   }

   // Odd Even sort in OMP
   // Borrowed from Karlsson's Example code
    int phase, i;
    datapoint_t tmp;

#  pragma omp parallel num_threads(omp_get_num_procs()) \
      default(none) shared(personal_dataset) private(i, tmp, phase)
   for (phase = 0; phase < personal_dataset.count; phase++) {
      if (phase % 2 == 0)
#        pragma omp for
         for (i = 1; i < personal_dataset.count; i += 2) {
            if (personal_dataset.data[i-1].distance > personal_dataset.data[i].distance) {
               tmp = personal_dataset.data[i-1];
               personal_dataset.data[i-1] = personal_dataset.data[i];
               personal_dataset.data[i] = tmp;
            }
         }
      else
#        pragma omp for
         for (i = 1; i < personal_dataset.count-1; i += 2) {
            if (personal_dataset.data[i].distance > personal_dataset.data[i+1].distance) {
               tmp = personal_dataset.data[i+1];
               personal_dataset.data[i+1] = personal_dataset.data[i];
               personal_dataset.data[i] = tmp;
            }
         }
   }

   for(int i = 0; i < personal_dataset.count; i++)
   {
      personal_dataset.data[i].classification = personal_dataset.data[i].features[personal_dataset.feature_count -1];
   }
   MPI_Barrier(MPI_COMM_WORLD);
   if(rank == 0)
   {
      printf("Done!\n");
      printf("Reducing...");
   }

   // Reduce the data using a binary tree.
   int reduction = floor(num_tasks / 2);
   int sent = 0;
   dataset_t reduction_set;
   dataset_t combined;
   int round = 0;
   while(reduction > 0)
   {
      if(rank >= reduction && sent == 0)
      {
            MPI_Send(&personal_dataset.count
                   , 1
                   , MPI_INT
                   , rank - reduction
                   , 0
                   , MPI_COMM_WORLD);
            for(int i = 0; i < personal_dataset.count; i++)
            {
               MPI_Send(&personal_dataset.data[i].distance
                      , 1
                      , MPI_DOUBLE
                      , rank - reduction
                      , 0
                      , MPI_COMM_WORLD);
               MPI_Send(&personal_dataset.data[i].classification
                      , 1
                      , MPI_DOUBLE
                      , rank - reduction
                      , 0
                      , MPI_COMM_WORLD);
            }
         sent = 1;
      }
      else if(rank < reduction)
      {
         MPI_Recv(&reduction_set.count
                 , 1
                 , MPI_INT
                 , rank + reduction
                 , 0
                 , MPI_COMM_WORLD
                 , &status);
         reduction_set.data = malloc(reduction_set.count * sizeof(datapoint_t));
         for(int i = 0; i < reduction_set.count; i++)
         {
            MPI_Recv(&reduction_set.data[i].distance
                   , 1
                   , MPI_DOUBLE
                   , rank + reduction
                   , 0
                   , MPI_COMM_WORLD
                   , &status);
            MPI_Recv(&reduction_set.data[i].classification
                   , 1
                   , MPI_DOUBLE
                   , rank + reduction
                   , 0
                   , MPI_COMM_WORLD
                   , &status);
         }

         combined.count = reduction_set.count + personal_dataset.count;

         combined.data = malloc(combined.count * sizeof(datapoint_t));

         // Do the merge from mergesort
         int personal_index = 0;
         int reduction_index = 0;
         int combined_index = 0;

         while(personal_index < personal_dataset.count
            && reduction_index < reduction_set.count)
         {
            if(personal_dataset.data[personal_index].distance <= reduction_set.data[reduction_index].distance)
            {
               combined.data[combined_index].distance = personal_dataset.data[personal_index].distance;
               combined.data[combined_index].classification = personal_dataset.data[personal_index].classification;
               personal_index += 1;
            }
            else
            {
               combined.data[combined_index].distance = reduction_set.data[reduction_index].distance;
               combined.data[combined_index].classification = reduction_set.data[reduction_index].classification;
               reduction_index += 1;
            }
            combined_index += 1;

         }

         while(personal_index < personal_dataset.count)
         {
            combined.data[combined_index].distance = personal_dataset.data[personal_index].distance;
            combined.data[combined_index].classification = personal_dataset.data[personal_index].classification;
            personal_index += 1;
            combined_index += 1;
         }
         while(reduction_index < reduction_set.count)
         {
            combined.data[combined_index].distance = reduction_set.data[reduction_index].distance;
            combined.data[combined_index].classification = reduction_set.data[reduction_index].classification;
            reduction_index += 1;
            combined_index += 1;
         }

        /* personal_dataset.data = malloc(combined.count * sizeof(datapoint_t));
         for(int i = 0; i < combined.count; i++)
         {
            personal_dataset.data[i].distance = combined.data[i].distance;
            personal_dataset.data[i].classification = combined.data[i].classification;
         }
         personal_dataset.count = combined.count;
         */
      }
      personal_dataset = combined;
      reduction = floor(reduction / 2);
      MPI_Barrier(MPI_COMM_WORLD);
      round += 1;

   }

   if(rank == 0)
   {
      printf("Done!\n");
      int classified = -1;
      int count = 0;
      int max_count = 0;
      for(int i = 0; i < args.k; i++)
      {
         for(int j = 0; j < args.k; j++)
         {
            if(personal_dataset.data[i].classification == personal_dataset.data[j].classification)
            {
               count += 1;
            }
         }
         if(count > max_count)
         {
            classified = (int) personal_dataset.data[i].classification;
            max_count = count;
         }
      }
      printf("Class: %d\n", classified);
   }


   // Once at root, calculate the mode of the top K datapoints and print out the
   // class.
   MPI_Finalize();
}

