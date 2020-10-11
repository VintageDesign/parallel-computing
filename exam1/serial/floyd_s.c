#include "floyd_s.h"
#include "utils.h"

void floyd_serial(uint8_t node_count, uint8_t *** adj_matrix)
{
    uint8_t k = 0;
    uint8_t i = 0;
    uint8_t j = 0;

    for(; k < node_count; k++)
    {
        for(; i < node_count; i++)
        {
            for(; j < node_count; j++)
            {
                (*adj_matrix)[i][j] = minimum((*adj_matrix)[i][j]
                                         , (*adj_matrix)[i][k] + (*adj_matrix)[k][j]);
            }
        }
    }

    return;
}
