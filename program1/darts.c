#include "darts.h"


void darts(long long int n)
{
    srandom(0);
    long long int        index         = 0;
    long long int        hit_count     = 0;
    double               x             = 0;
    double               y             = 0;
    int                  temp_x;
    int                  temp_y;
    struct random_data * data;
           char        * states;

    srandom(0);
    data = (struct random_data*) calloc(omp_get_num_procs(), sizeof(struct random_data));
    states = (char*) calloc(omp_get_num_procs(), 256);

    for (;index < omp_get_num_procs(); index++)
    {
        initstate_r(random(), &states[index], 256, &data[index]);
    }

    printf("%d\n", RAND_MAX);

    #pragma omp parallel for num_threads(omp_get_num_procs()) default(none) shared(hit_count, n, data) private(x, y, index, temp_x, temp_y) schedule(guided) 
    for(index = 0; index < n; index++)
    {
        srandom_r(0, &data[omp_get_thread_num()]);
        random_r(&data[omp_get_thread_num()], &temp_x) ;
        x = temp_x / (double) RAND_MAX;
        random_r(&data[omp_get_thread_num()], &temp_y) ;
        y = temp_y / (double) RAND_MAX;

        if(x * x + y * y <= 1)
        {
            #pragma omp critical
            {
                hit_count++;
                // printf("Hit: %lld\n", hit_count);

            }
        }

    }

    printf("Approximated Value: %lf\n", 4 * ((double)hit_count / (double) n));

}
