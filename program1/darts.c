#include "darts.h"


void darts(long long int n)
{
    long long int        index         = 0;
    long long int        hit_count     = 0;
    float                x             = 0;
    float                y             = 0;
    int                * random_array;

    random_array = malloc(n * sizeof(int) + 1);
    printf("darts: %llu\n", n);

    if(random_array == NULL)
    {
       printf("Could not allocate random numbers\n");
    }

    srandom(100);

    printf("Pregenerating random numbers...\n");
    for(; index < n + 1; index++)
    {
       random_array[index] = random() ;
    }

    printf("Yeeting darts...\n");
    #pragma omp parallel for num_threads(omp_get_num_procs()) default(none) shared( n, random_array) private(x, y, index) schedule(guided) reduction(+:hit_count)
    for(index = 0; index < n; index++)
    {
        x = random_array[index] / (float) RAND_MAX;
        y = random_array[index + 1 ] / (float) RAND_MAX;
        if(x * x + y * y <= 1)
        {
          hit_count += 1;
        }

    }

    free(random_array);
    printf("Approximated Value: %lf\n", 4 * ((float)hit_count / (float) n));

}
