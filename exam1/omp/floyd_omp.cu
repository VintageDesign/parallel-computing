#include <omp.h>

#include "floyd_omp.h"
#include "utils.h"

void floyd_omp(int node_count, int *** adj_matrix)
{
    int k = 0;
    int i = 0;
    int j = 0;

#   pragma omp parallel for num_threads(omp_get_num_procs()) default(none) shared(adj_matrix, node_count) private(i, k, j)
    for(k = 0; k < node_count; k++)
    {
        for(i = 0; i < node_count; i++)
        {
            for(j = 0; j < node_count; j++)
            {
                (*adj_matrix)[i][j] = minimum((*adj_matrix)[i][j]
                                         , (*adj_matrix)[i][k] + (*adj_matrix)[k][j]);
            }
        }
    }

    return;
}
