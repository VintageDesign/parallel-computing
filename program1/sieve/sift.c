#include "sift.h"

bool     *  A  = NULL;

void init_sift(uint32_t n)
{
    A = malloc(n * sizeof(uint32_t));
    for(uint32_t index = 0; index < n; index++)
    {
        A[index] = true;
    }
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
    uint32_t total_count = 0;
    uint32_t print_count = 0;
    for( uint32_t index = 2; index < n; index++)
    {
        if(A[index])
        {
            printf("%d, ", index);
            print_count++;
            total_count++;

            if(print_count == 10)
            {
                print_count = 0;
                printf("\n");
            }

        }
    }
    printf("\n");
    printf("Total Primes: %d\n", total_count);

}

void sift(uint32_t n)
{
    uint32_t sifting_val = 0;
    double   start_time;
    double   end_time;

    init_sift(n);


    start_time = omp_get_wtime();
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
    end_time = omp_get_wtime();

    print_values(n);
    printf("Single Core Time Elapsed: %f (s)\n ", (end_time - start_time));

    clean_sift();
}

void sift_par(uint32_t n)
{
    uint32_t sifting_val = 0;
    uint32_t index;
    double   start_time;
    double   end_time;

    double   sqrt_n = ceil(sqrt(n));



    printf("Procs: %d\n", omp_get_num_procs());
    init_sift(n);
    start_time = omp_get_wtime();
#   pragma omp parallel for num_threads(omp_get_num_procs()) default(none) shared(A, n, sqrt_n) private(index, sifting_val)
    for(index = 2; index < (int)sqrt_n; index++)
    {
        if(A[index])
        {
            for(uint32_t multiplier = 0; (index * index + index * multiplier) < n; multiplier++)
            {
                sifting_val = (index * index + index * multiplier);
                A[sifting_val] = false;
            }
        }

    }
    end_time = omp_get_wtime();

    print_values(n);
    printf("Multi-Core Time Elapsed: %f (s)\n ", (end_time - start_time));

    clean_sift();
}
