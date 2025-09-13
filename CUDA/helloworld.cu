#include <stdio.h>
__global__ void helloFromGPU()
{
    printf("Hello World from GPU!\n");
    printf("%d %d\n", blockIdx.x, threadIdx.x);
}
int main()
{
    // Launch kernel
    helloFromGPU<<<3, 2>>>();
    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();
    printf("Hello World from CPU!\n");
    return 0;
}
