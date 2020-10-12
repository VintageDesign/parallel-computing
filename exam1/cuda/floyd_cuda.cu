#include "floyd_cuda.h"
#include "utils.h"


__global__ void shortest_path_kernel(int k, int node_count, int * matrix_cuda)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;

    for(int i = 0; i < node_count; i++)
    {
        if(matrix_cuda[i * node_count + index] > matrix_cuda[i * node_count + k] + matrix_cuda[k * node_count + index])
        {
            matrix_cuda[i * node_count + index] = matrix_cuda[i * node_count + k] + matrix_cuda[k * node_count + index];
        }
    }

}


void floyd_cuda(int node_count, int *** adj_matrix)
{
    int * matrix_cuda;
    
    // Copy to GPU
    cudaMalloc((void **) &matrix_cuda,node_count *  node_count * sizeof(int));
    
    for(int i = 0; i < node_count; i++)
    {
        cudaMemcpy(&matrix_cuda[i * node_count], (*adj_matrix)[i], (size_t) node_count * sizeof(int), cudaMemcpyHostToDevice);
    }


    // do the thing
    for(int k = 0; k < node_count; k++)
    {
        shortest_path_kernel <<<node_count, 1>>>(k,node_count, matrix_cuda);
        cudaDeviceSynchronize();
    }
   



    // Copy back to Host
    for(int i = 0; i < node_count; i++)
    {
        cudaMemcpy((*adj_matrix)[i], &matrix_cuda[i * node_count], (size_t) node_count * sizeof(int), cudaMemcpyDeviceToHost);
    }

}

