#include "sift.h"



int main(int argc, char ** argv)
{
    uint32_t n = 0; 

    if(argc < 2 || argc > 2)
    {
        printf("Incorrect args. \nCorrect Usage:\n    ./program1 <number>\n");
        exit(1);
    }

    n = atoi(argv[1]);
    printf("%d\n", n );
    
    sift_par(n);
    sift(n);

    return 0;
}