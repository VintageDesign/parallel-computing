#include "utils.h"

uint8_t minimum(uint8_t left, uint8_t right)
{
    return (left < right) ? left : right;
}

void copy_adj(const uint8_t size, uint8_t ** adj, uint8_t *** copy)
{
    (*copy)        = (uint8_t **) calloc(size, sizeof(uint8_t *));
    for(uint8_t index = 0; index < size; index++)
    {
        (*copy)[index]        = (uint8_t *) calloc(size, sizeof(uint8_t));
    }

    for(int row = 0; row < size; row++)
    {
        for(int col = 0; col < size; col++)
        {
            (*copy)[row][col] = adj[row][col];
        }
    }

}


bool check_answer(uint8_t size, uint8_t ** key, uint8_t ** attempt)
{
    bool retval = true;

    for(int row = 0; row < size; row++)
    {
        for(int col = 0; col < size; col++)
        {
            if(key[row][col] != attempt[row][col])
            {
                retval = false;
            }
        }
    }
    
    return retval;
}

void print_matrix(uint8_t size, uint8_t ** matrix)
{
    for(int row = 0; row < size; row++)
    {
        for(int col = 0; col < size; col++)
        {
            printf("%d, ", matrix[row][col]);
        }
        printf("\n");
    }
}
