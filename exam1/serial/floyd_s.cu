#include "floyd_s.h"
#include "utils.h"

void floyd_serial(int node_count, int *** adj_matrix)
{
    int k = 0;
    int i = 0;
    int j = 0;

    for(k = 0; k < node_count; k++)
    {
        for(i = 0; i < node_count; i++)
        {
            for(j = 0; j < node_count; j++)
            {
                if ((*adj_matrix)[i][k] + (*adj_matrix)[k][j] < (*adj_matrix)[i][j])  
                    (*adj_matrix)[i][j] = (*adj_matrix)[i][k] + (*adj_matrix)[k][j];  
            }
        }
    }


    return;
}
