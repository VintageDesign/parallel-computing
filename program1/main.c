#include "sift.h"
#include "darts.h"



int main(int argc, char ** argv)
{
    long long int n = 0; 

    if(argc < 2 || argc > 2)
    {
        printf("Incorrect args. \nCorrect Usage:\n    ./program1 <number>\n");
        exit(1);
    }

    n = atoi(argv[1]);
    //printf("%d\n", n );
    
    //sift_par(n);
    //sift(n);

    darts(n);
    return 0;
}