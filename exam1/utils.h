#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define  minimum(left, right) ((left < right) ? left : right)

void copy_adj(const int size, int ** adj, int *** copy); 
bool check_answer(int size, int ** key, int ** attempt);
void print_matrix(int size, int ** matrix);

#endif
