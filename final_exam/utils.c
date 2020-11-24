#include "utils.h"

cmd_arguments_t parse_cmd_args(int argc, char ** argv)
{
   cmd_arguments_t retval;

   if( argc == 4)
   {
      retval.k = atoi(argv[1]);
      retval.csv_filename = argv[2];
      retval.query_filename = argv[3];
   }
   else
   {
      printf("Error parsing command line arguments.\n");
      printf("Example Run:\n");
      printf("> mpiexec knn_classify <k> <dataset filename> <query filename>\n");
      exit(1);
   }
   return retval;
}


dataset_t parse_csv(char * csv_filename)
{
   FILE * file_h = fopen((const char *) csv_filename, "r");

   printf("Counting Number of Lines\n");
   int line_count = count_datapoints(file_h);
   printf("Line Count: %d\n", line_count);
   printf("Counting Line Size\n");
   int line_size = count_line_size(file_h);
   printf("Line Size: %d\n", line_size);

   dataset_t data_set;
   data_set.data = malloc(line_count * sizeof(datapoint_t));
   data_set.count = line_count;
   data_set.feature_count = line_size;

   for(int i = 0; i < line_count; i++)
   {
      data_set.data[i].features = malloc(line_size * sizeof(double));
   }
   printf("Allocated Memory...\n");

   char * line;
   size_t size;
   char * feature;

   for(int row = 0; row < line_count; row++)
   {
      getline(&line, &size, file_h);
      feature = strtok(line, ",");
      for(int feature_idx = 0; feature_idx < line_size; feature_idx++)
      {
         data_set.data[row].features[feature_idx] = atof(feature);
         //printf("%lf,", data_set.data[row].features[feature_idx]);
         feature = strtok(NULL, ",");
      }
      //printf("\n");
   }

   fclose(file_h);

   return data_set;

}

int count_datapoints(FILE * file_handle)
{
   int line_count = 0;
   char * line;
   size_t size;

   // I hate this but read the whole file and count the number of data entries.
   while(getline(&line, &size, file_handle) != -1)
   {
      line_count++;
   }
   rewind(file_handle);
   return line_count;
}

int count_line_size(FILE * file_handle)
{
   char * line;
   size_t size;
   int data_count = 0;

   getline(&line, &size, file_handle);
   rewind(file_handle);

   for(int i = 0; i < (int) size; i++)
   {
      if(line[i] == ',' || line[i] == '\n')
      {
         data_count++;
      }
   }
   return data_count;
}

void send_partition(int dest,int send_size, int partition_size, dataset_t data_set)
{

   MPI_Send((const void *) &send_size, 1,  MPI_INT, dest, 0, MPI_COMM_WORLD);
   MPI_Send((const void *) &data_set.feature_count, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

   for(int i = dest * partition_size; i < dest * partition_size + send_size; i++)
   {
      MPI_Send((const void *) data_set.data[i].features, data_set.feature_count, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
   }
}
