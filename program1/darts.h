#ifndef DARTS_H
#define DARTS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

/**
 * @brief Approximates PI in parallel using the Monte Carlo process
 * 
 * @param n The number of darts to use
 */
void darts(long long int n);

#endif