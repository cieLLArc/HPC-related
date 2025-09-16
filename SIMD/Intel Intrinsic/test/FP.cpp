#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <intrin.h>
#include <iostream>

double rand_double()
{
    return rand() / (RAND_MAX + 1.0);
}

void print_matrix(double *m, int dim1, int dim2)
{
    for (int i = 0; i < dim1; i++)
    {
        for (int j = 0; j < dim2; j++)
        {
            printf("%lf ", m[i * dim2 + j]);
        }
        printf("\n");
    }
}

void compute_naive(double *A, double *B, double *C, int m, int n, int k)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int p = 0; p < k; p++)
            {
                // C[i][j] += A[i][k] * B[k][j];
                // C在第i行j列，一行n个元素，故i*n+j
                // A在第i行k列，一行k个元素，故i*k+p，+p是因为本循环内A中p自加代表取一行中的下一个元素
                // B在第k行j列，一行n个元素，故p*n+j，p*是因为本循环内B中p自加代表取一列中的下一个元素
                C[i * n + j] += A[i * k + p] * B[p * n + j];
            }
        }
        printf("row %d done\n", i);
    }
}

// A,B,C矩阵均按行优先存储在动态开辟的一维数组中
void compute_fma_ymm(double *A, double *B, double *C, int m, int n, int k)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j += 4)
        {
            // 一个ymm寄存器可存储4个double型变量，即一个有4个元素的C行向量分量，据此分配计算的粒度
            __m256d c_vreg = _mm256_setzero_pd();

            // 存储一个标量A[][]，并将其复制扩展4份填满ymm寄存器
            __m256d a_extended_vreg = _mm256_setzero_pd();

            // 存储一个有4个元素的B行向量分量
            __m256d b_vreg = _mm256_setzero_pd();

            for (int p = 0; p < k; p++)
            {
                // p在A中用于遍历A一行中的元素
                // p在B中用于遍历B中的行
                a_extended_vreg = _mm256_set1_pd(A[i * k + p]);

                b_vreg = _mm256_loadu_pd((double *)&B[p * n + j]);

                c_vreg = _mm256_fmadd_pd(a_extended_vreg, b_vreg, c_vreg);
            }
            double val[4];
            _mm256_storeu_pd(val, c_vreg);
            C[i * n + j + 0] += val[0];
            C[i * n + j + 1] += val[1];
            C[i * n + j + 2] += val[2];
            C[i * n + j + 3] += val[3];
        }
        printf("row %d done\n", i);
    }
}

// A,B,C矩阵均按行优先存储在动态开辟的一维数组中
void compute_fma_ymm_alignment(double *A, double *B, double *C, int m, int n, int k)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j += 4)
        {
            __m256d c_vreg = _mm256_setzero_pd();
            __m256d a_extended_vreg;
            __m256d b_vreg;

            for (int p = 0; p < k; p++)
            {
                a_extended_vreg = _mm256_set1_pd(A[i * k + p]);
                b_vreg = _mm256_load_pd((double *)&B[p * n + j]);
                c_vreg = _mm256_fmadd_pd(a_extended_vreg, b_vreg, c_vreg);
            }
            _mm256_store_pd(&C[i * n + j], c_vreg); // 存回C，需对齐
        }
        printf("row %d done\n", i);
    }
}

void compute_fma_ymm_alignment_blocking(double *A, double *B, double *C, int m, int n, int k)
{

    for (int i = 0; i < m; i += 4)
    {
        // 现在是i++算出一行中4个C的元素
        // 改为i+=4，使每次迭代计算出4行4个C元素
        for (int j = 0; j < n; j += 4)
        {
            __m256d c_0_vreg = _mm256_setzero_pd();
            __m256d c_1_vreg = _mm256_setzero_pd();
            __m256d c_2_vreg = _mm256_setzero_pd();
            __m256d c_3_vreg = _mm256_setzero_pd();

            for (int p = 0; p < k; p++)
            {
                __m256d a_0_extended_vreg = _mm256_set1_pd(A[(i + 0) * k + p]);
                __m256d a_1_extended_vreg = _mm256_set1_pd(A[(i + 1) * k + p]);
                __m256d a_2_extended_vreg = _mm256_set1_pd(A[(i + 2) * k + p]);
                __m256d a_3_extended_vreg = _mm256_set1_pd(A[(i + 3) * k + p]);

                __m256d b_vreg = _mm256_load_pd(&B[p * n + j]);

                c_0_vreg = _mm256_fmadd_pd(a_0_extended_vreg, b_vreg, c_0_vreg);
                c_1_vreg = _mm256_fmadd_pd(a_1_extended_vreg, b_vreg, c_1_vreg);
                c_2_vreg = _mm256_fmadd_pd(a_2_extended_vreg, b_vreg, c_2_vreg);
                c_3_vreg = _mm256_fmadd_pd(a_3_extended_vreg, b_vreg, c_3_vreg);
            }
            _mm256_store_pd(&C[(i + 0) * n + j], c_0_vreg);
            _mm256_store_pd(&C[(i + 1) * n + j], c_1_vreg);
            _mm256_store_pd(&C[(i + 2) * n + j], c_2_vreg);
            _mm256_store_pd(&C[(i + 3) * n + j], c_3_vreg);
        }
        printf("row %d done\n", i);
    }
}

int main()
{
    // int m = 8;
    // int n = 8;
    // int k = 8;

    int m = 5000;
    int n = 5000;
    int k = 5000;

    double *A = (double *)malloc(sizeof(double) * m * k);
    double *B = (double *)malloc(sizeof(double) * k * n);
    double *C = (double *)malloc(sizeof(double) * m * n);

    double *A_align = (double *)_aligned_malloc(sizeof(double) * m * k, 32);
    double *B_align = (double *)_aligned_malloc(sizeof(double) * k * n, 32);
    double *C_align = (double *)_aligned_malloc(sizeof(double) * m * n, 32);

    memset(A, 0, sizeof(double) * m * k);
    memset(B, 0, sizeof(double) * k * n);
    memset(C, 0, sizeof(double) * m * n);

    memset(A_align, 0, sizeof(double) * m * k);
    memset(B_align, 0, sizeof(double) * k * n);
    memset(C_align, 0, sizeof(double) * m * n);

    srand(0); // 为了验证优化后的结果是否正确，故使用相同的一组随机数

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            A[i * k + j] = rand_double();
            A_align[i * k + j] = A[i * k + j];
        }
    }

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            B[i * n + j] = rand_double();
            B_align[i * n + j] = B[i * n + j];
        }
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i * n + j] = rand_double();
            C_align[i * n + j] = C[i * n + j];
        }
    }

    // print_matrix(A, m, k);
    // print_matrix(B, k, n);
    // print_matrix(C, m, n);

    // compute_fma_ymm(A, B, C, m, n, k);
    // compute_fma_ymm_alignment(A_align, B_align, C_align, m, n, k);
    compute_fma_ymm_alignment_blocking(A_align, B_align, C_align, m, n, k);

    free(A);
    free(B);
    free(C);

    return 0;
}
