#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

uint8_t minimum(uint8_t left, uint8_t right);
void copy_adj(const uint8_t size, uint8_t ** adj, uint8_t *** copy); 
bool check_answer(uint8_t size, uint8_t ** key, uint8_t ** attempt);
void print_matrix(uint8_t size, uint8_t ** matrix);

#endif
