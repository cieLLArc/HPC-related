#include <stdio.h>

#define size 10

void compute_cpu(int *A, int *B, int *C)
{
    for (int i = 0; i < size; i++)
    {
        C[i] = A[i] + B[i];
    }
}

__global__ void compute_gpu(int *gpuA, int *gpuB, int *gpuC)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    gpuC[tid] = gpuA[tid] + gpuB[tid];
}

void print_list(int *C)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", C[i]);
    }
    printf("\n");
}

int main()
{
    int *A = (int *)malloc(size * sizeof(int));
    int *B = (int *)malloc(size * sizeof(int));
    int *C = (int *)malloc(size * sizeof(int));

    if (A && B && C)
    {
        memset(A, 0, size * sizeof(int));
        memset(B, 0, size * sizeof(int));
        memset(C, 0, size * sizeof(int));
    }
    else
    {
        free(A);
        free(B);
        free(C);
        printf("MEMORY failed to allocate\n");
        exit(-1);
    }
    for (int i = 0; i < size; i++)
    {
        A[i] = i;
        B[i] = i;
    }

    printf("computed on cpu\n");
    compute_cpu(A, B, C);
    print_list(C);

    // 在gpu global memory中开辟数组空间

    int *gpuA, *gpuB, *gpuC;
    cudaMalloc((int **)&gpuA, size * sizeof(int));
    cudaMalloc((int **)&gpuB, size * sizeof(int));
    cudaMalloc((int **)&gpuC, size * sizeof(int));

    if (gpuA && gpuB && gpuC)
    {
        cudaMemset(gpuA, 0, size * sizeof(int));
        cudaMemset(gpuB, 0, size * sizeof(int));
        cudaMemset(gpuC, 0, size * sizeof(int));
    }
    else
    {
        cudaFree(gpuA);
        cudaFree(gpuB);
        cudaFree(gpuC);
        printf("GMEMORY failed to allocate\n");
        exit(-1);
    }

    // 将数据从主存cpu(host)传输到显存gpu(device)
    cudaMemcpy(gpuA, A, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(gpuB, B, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(gpuC, C, size * sizeof(int), cudaMemcpyHostToDevice);

    compute_gpu<<<1, size>>>(gpuA, gpuB, gpuC);

    cudaDeviceSynchronize();

    // 将数据从device传回host
    int *D = (int *)malloc(size * sizeof(int));
    cudaMemcpy(D, gpuC, size * sizeof(int), cudaMemcpyDeviceToHost);
    printf("computed on gpu\n");
    print_list(D);

    free(A);
    free(B);
    free(C);
    free(D);
    cudaFree(gpuA);
    cudaFree(gpuB);
    cudaFree(gpuC);

    return 0;
}