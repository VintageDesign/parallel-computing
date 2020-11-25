#ifndef UTILS_H
#define UTILS_H
#include "mpi.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct cmd_arguments
{
   int k;
   char * csv_filename;
   char * query_filename;
} cmd_arguments_t;

typedef struct datapoint
{
   double * features;
   double distance;
   double classification;
} datapoint_t;

typedef struct dataset
{
   datapoint_t * data;
   int count;
   int feature_count;
} dataset_t;


cmd_arguments_t parse_cmd_args(int argc, char ** argv);

dataset_t parse_csv(char * csv_filename);

int count_datapoints(FILE * file_handle);

int count_line_size(FILE * file_handle);

void send_partition(int dest,int send_size, int partition_size, dataset_t data_set);

#endif
