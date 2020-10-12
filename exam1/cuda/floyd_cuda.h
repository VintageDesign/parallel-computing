#ifndef FLOYD_CUDA_H
#define FLOYD_CUDA_H
#include <stdint.h>
#include <stddef.h>
#define block_Size 256 

void floyd_cuda(int node_count, int *** adj_matrix);

#endif
