#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "floyd_s.h"
#include "floyd_cuda.h"
#include "floyd_omp.h"
#include "utils.h"

#define INF 99999

typedef struct command_args {
    bool run_serial;
    bool run_omp;
    bool run_cuda;
    char * file_name;
} command_args_t;

command_args_t parse_args(uint8_t argc, char ** argv);
void print_usage(void);

void parse_adj(FILE * file_handle_p, int * size, int *** adj, int *** adj_answer);


int main(int argc, char ** argv)
{
    FILE * file_handle_p;

    int ** adj        = NULL;
    int ** adj_answer = NULL;
    int ** solution   = NULL;
    int    adj_size   = 0;


    command_args_t parsed_args = parse_args(argc, argv);

    printf("Opening File: %s...", parsed_args.file_name);
    file_handle_p = fopen(parsed_args.file_name, "r");
    if(file_handle_p == NULL)
    {
        printf("\nERROR! Could not open file %s\n", parsed_args.file_name);
        exit(1);
    }
    printf("Done\n");


    printf("Parsing File....");
    parse_adj(file_handle_p, &adj_size, &adj, &adj_answer);
    printf("Done!\n");

    printf("Graph size: %d nodes\n", adj_size);
    

    if(parsed_args.run_serial)
    {
        printf("Serial: ");
        copy_adj(adj_size, adj, &solution);
        floyd_serial(adj_size, &solution);
        if (!check_answer(adj_size, adj_answer, solution))
        {
            printf("Fail\n");
            printf("Solution:\n");
            print_matrix(adj_size, adj_answer);
            printf("Calculated:\n");
            print_matrix(adj_size, solution);
            printf("Starting:\n");
            print_matrix(adj_size, adj);
        }
        else
        {
            printf("Pass\n");
        }

    }

    if(parsed_args.run_omp)
    {
        printf("OpenMP: ");
        copy_adj(adj_size, adj, &solution);
        floyd_omp(adj_size, &solution);
        if (!check_answer(adj_size, adj_answer, solution))
        {
            printf("Fail\n");
        }
        else
        {
            printf("Pass\n");
        }

    }

    if(parsed_args.run_cuda)
    {
        printf("Cuda: ");
        copy_adj(adj_size, adj, &solution);
        floyd_cuda(adj_size, &solution);
        if (!check_answer(adj_size, adj_answer, solution))
        {
            printf("Fail\n");
            printf("Solution:\n");
            print_matrix(adj_size, adj_answer);
            printf("Calculated:\n");
            print_matrix(adj_size, solution);
            printf("Starting:\n");
            print_matrix(adj_size, adj);
        }
        else
        {
            printf("Pass\n");
        }
    }
   

    // Clean up...
    fclose(file_handle_p);
    free(adj);
    free(adj_answer);

    return 0;
}



command_args_t parse_args(uint8_t argc, char ** argv)
{
    command_args_t retval;

    if(argc < 2)
    {
        printf("ERROR! Exam 1 requires at least one command line argument!\n");
        print_usage();
        exit(1);
    }

    if(argc == 2)
    {
        // Assume that the second arg is the file name and we want to run all
        // possible functions.
        retval.run_serial = true;
        retval.run_omp = true;
        retval.run_cuda = true;
        retval.file_name = argv[1];
    }

    else if(argc == 3)
    {
        retval.file_name = argv[2];

        if(strcmp("-s", argv[1]) == 0)
        {
            retval.run_serial = true;
            retval.run_omp = false;
            retval.run_cuda = false;
        }
        else if(strcmp("-o", argv[1]) == 0)
        {
            retval.run_serial = false;
            retval.run_omp = true;
            retval.run_cuda = false;
        }
        else if(strcmp("-c", argv[1]) == 0)
        {
            retval.run_serial = false;
            retval.run_omp = false;
            retval.run_cuda = true;
        }
        else
        {
            printf("ERROR! Could not parse argument \"%s\"\n", argv[1]);
            print_usage();
            exit(1);
        }
    }

    return retval;
}

void print_usage(void)
{
    // TODO
}



void parse_adj( FILE     * file_handle_p
        , int  *  size
        , int *** adj
        , int *** adj_answer)
{
    char      buffer[255];
    char    * token = NULL;
    int   row   = 0;
    int   col   = 0;
    

    fgets(buffer, 255, file_handle_p);
    *size = atoi(buffer);

    (*adj)        = (int **) calloc(*size, sizeof(int *));
    (*adj_answer) = (int **) calloc(*size, sizeof(int *));
    for(int index = 0; index < *size; index++)
    {
        (*adj)[index]        = (int *) calloc(*size, sizeof(int));
        (*adj_answer)[index] = (int *) calloc(*size, sizeof(int));
    }


    while(fgets(buffer, 255, file_handle_p) != NULL)
    {
        col = 0;
        token = strtok(buffer, " ");
        while(token != NULL)
        {
            if(strcmp(token, "inf") == 0 || strcmp(token, "inf\n") == 0)
            {
                (*adj)[row][col] = INF;
            }
            else
            {
                (*adj)[row][col] = atoi(token);  
            }
            token = strtok(NULL, " ");
            col += 1;
        }
        row += 1;
        if(row == *size)
        break;
        
    }
    row = 0;

    while(fgets(buffer, 255, file_handle_p) != NULL)
    {
        col = 0;
        token = strtok(buffer, " ");
        while(token != NULL)
        {
            if(strcmp(token, "inf") == 0 || strcmp(token, "inf\n") == 0)
            {
                (*adj_answer)[row][col] = INF;
            }
            else
            {
                (*adj_answer)[row][col] = atoi(token);  
            }
            token = strtok(NULL, " ");
            col += 1;
        }
        row += 1;
        if(row == *size)
        break;
        
    }
    row = 0;
}

