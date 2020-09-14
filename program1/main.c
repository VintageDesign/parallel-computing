#include <string.h>
#include "sift.h"
#include "darts.h"



int main(int argc, char ** argv)
{
    long long int  n         = 0;
    long long int  n_darts   = 0;
    char         * args      = NULL;
    bool           run_sift  = false;
    bool           run_darts = false;

    if(argc < 3 || argc > 4)
    {
        printf("Incorrect args. \nCorrect Usage:\n    ./program1 -ds <sift_number> <darts_number>\n");
        exit(1);
    }

    args = argv[1];

    if(strcmp(args, "-d") == 0)
    {
       run_darts = true;
    }
    else if(strcmp(args, "-s") == 0)
    {
       run_sift = true;
    }
    else if(strcmp(args, "-ds") == 0 || strcmp(args, "-sd") == 0)
    {
       run_sift = true;
       run_darts = true;
    }
    else
    {
       printf("Invalid arguments, correct options are: -d, -s, -ds, or -sd\n");
       exit(1);
    }

    if (run_sift && run_darts)
    {
       n = atoll(argv[2]);
       n_darts = atoll(argv[3]);
    }
    else if(run_sift)
    {
       n = atoll(argv[2]);
    }
    else
    {
       n_darts = atoll(argv[2]);
    }

    if(run_sift)
    {
       printf("Sifting.....\n");
       sift_par(n);
       sift(n);
    }

    if(run_darts)
    {
       printf("Throwing Darts...\n");
       darts(n_darts);
    }

    return 0;
}
