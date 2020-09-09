#include "sift.h"

void init_sift(uint32_t n)
{
    A = malloc(n * sizeof(uint32_t));
}

void clean_sift(void)
{
    free(A);
}

uint32_t calculate_j_value(uint32_t index, uint32_t multiplier)
{
    return (index * index + index * multiplier);
}

void print_values(uint32_t n)
{
    uint32_t print_count = 0; 
    for( uint32_t index = 0; index < n; index++)
    {
        if(A[index])
        {
            printf("%d, ", index);
            print_count++;

            if(print_count == 10)
            {
                print_count = 0;
                printf("\n");
            }

        }
    }
    
}

void sift(uint32_t n)
{
    uint32_t sifting_val = 0;

    init_sift(n);

    for(uint32_t index = 2; index < sqrt(n); index++)
    {
        if(A[index])
        {
            for(uint32_t multiplier = 0; calculate_j_value(index, multiplier) < n; multiplier++)
            {
                sifting_val = calculate_j_value(index, multiplier); 
                A[sifting_val] = false;
            }
        }

    }

    print_values(n);

    clean_sift();
}

void sift_par(uint32_t n)
{
    uint32_t sifting_val = 0;

    init_sift(n);

    #pragma omp parallel for num_threads(thread_count) default(none) shared(A) private(index)
    for(uint32_t index = 2; index < sqrt(n); index++)
    {
        if(A[index])
        {
            for(uint32_t multiplier = 0; calculate_j_value(index, multiplier) < n; multiplier++)
            {
                sifting_val = calculate_j_value(index, multiplier); 
                A[sifting_val] = false;
            }
        }

    }

    print_values(n);

    clean_sift();
}