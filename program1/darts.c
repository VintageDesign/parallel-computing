#include "darts.h"

void darts(long long int n)
{
    srandom(0);
    long long int index     = 0;
    long long int hit_count = 0;
    double        x         = 0;
    double        y         = 0;


    #pragma omp parallel for num_threads(omp_get_num_procs()) default(none) shared(hit_count, n) private(x, y, index) 
    for(index = 0; index < n; index++)
    {
        x = random() / (double) RAND_MAX;
        y = random() / (double) RAND_MAX;

        if(x * x + y * y < 1)
        {
            #pragma omp critical
            hit_count++;
        }

    }

    printf("Approximated Value: %f\n", 4 * ((double)hit_count / (double) n));

}