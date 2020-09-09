#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


/*******************************************************************************
 * Globals
 ******************************************************************************/
bool     *  A  = NULL;

/**
 * @brief Initailzes the sifting algorithm with the correct global vars.
 * 
 * @param n The maximum value to sift. 
 */
void init_sift(uint32_t n);

/**
 * @brief Sifts out prime numbers between 2 and n.
 */
void sift(uint32_t n);

/**
 * @brief Sifts out the prime numbers between 2 and n in parallel 
 */
void sift_par(uint32_t n);
