#include <omp.h>

#include "floyd_omp.h"
#include "utils.h"

void floyd_omp(uint8_t node_count, uint8_t *** adj_matrix)
{
    uint8_t k = 0;
    uint8_t i = 0;
    uint8_t j = 0;

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
