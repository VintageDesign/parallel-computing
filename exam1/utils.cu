#include "utils.h"

int minimum(int left, int right)
{
    return (left < right) ? left : right;
}


void copy_adj(const int size, int ** adj, int *** copy)
{
    (*copy)        = (int **) calloc(size, sizeof(int *));
    for(int index = 0; index < size; index++)
    {
        (*copy)[index]        = (int *) calloc(size, sizeof(int));
    }

    for(int row = 0; row < size; row++)
    {
        for(int col = 0; col < size; col++)
        {
            (*copy)[row][col] = adj[row][col];
        }
    }

}


bool check_answer(int size, int ** key, int ** attempt)
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

void print_matrix(int size, int ** matrix)
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
