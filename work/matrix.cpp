#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <intrin.h>

void compute_naive(double **A, double **B, double **C, int m, int n, int k)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = 0;
            for (int p = 0; p < k; p++)
            {
                C[i][j] += A[i][p] * B[p][j];
            }
        }
        printf("row %d done\n", i);
    }
}

void compute_1_4(double **A, double **B, double **C, int m, int n, int k)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j += 4)
        {
            C[i][j + 0] = 0;
            C[i][j + 1] = 0;
            C[i][j + 2] = 0;
            C[i][j + 3] = 0;

            for (int p = 0; p < k; p++)
            {
                C[i][j + 0] += A[i][p] * B[p][j + 0];
                C[i][j + 1] += A[i][p] * B[p][j + 1];
                C[i][j + 2] += A[i][p] * B[p][j + 2];
                C[i][j + 3] += A[i][p] * B[p][j + 3];
            }
        }
        printf("row %d done\n", i);
    }
}

void compute_1_4_col_priority(double **A, double **B, double **C, int m, int n, int k)
{
    for (int j = 0; j < n; j += 4)
    {
        for (int i = 0; i < m; i++)
        {
            C[i][j + 0] = 0;
            C[i][j + 1] = 0;
            C[i][j + 2] = 0;
            C[i][j + 3] = 0;

            for (int p = 0; p < k; p++)
            {
                C[i][j + 0] += A[i][p] * B[p][j + 0];
                C[i][j + 1] += A[i][p] * B[p][j + 1];
                C[i][j + 2] += A[i][p] * B[p][j + 2];
                C[i][j + 3] += A[i][p] * B[p][j + 3];
            }
        }
        printf("col %d done\n", j);
    }
}

void compute_1_4_reg(double **A, double **B, double **C, int m, int n, int k)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j += 4)
        {
            C[i][j + 0] = 0;
            C[i][j + 1] = 0;
            C[i][j + 2] = 0;
            C[i][j + 3] = 0;

            register double c_00_reg = 0.0;
            register double c_01_reg = 0.0;
            register double c_02_reg = 0.0;
            register double c_03_reg = 0.0;
            register double a_0p_reg = 0.0;

            for (int p = 0; p < k; p++)
            {
                a_0p_reg = A[i][p];
                c_00_reg += a_0p_reg * B[p][j + 0];
                c_01_reg += a_0p_reg * B[p][j + 1];
                c_02_reg += a_0p_reg * B[p][j + 2];
                c_03_reg += a_0p_reg * B[p][j + 3];
            }
            C[i][j + 0] += c_00_reg;
            C[i][j + 1] += c_01_reg;
            C[i][j + 2] += c_02_reg;
            C[i][j + 3] += c_03_reg;
        }
        printf("row %d done\n", i);
    }
}

void compute_1_4_reg_col_priority(double **A, double **B, double **C, int m, int n, int k)
{
    for (int j = 0; j < n; j += 4)
    {
        for (int i = 0; i < m; i++)
        {
            C[i][j + 0] = 0;
            C[i][j + 1] = 0;
            C[i][j + 2] = 0;
            C[i][j + 3] = 0;

            register double c_00_reg = 0.0;
            register double c_01_reg = 0.0;
            register double c_02_reg = 0.0;
            register double c_03_reg = 0.0;
            register double a_0p_reg = 0.0;

            for (int p = 0; p < k; p++)
            {
                a_0p_reg = A[i][p];
                c_00_reg += a_0p_reg * B[p][j + 0];
                c_01_reg += a_0p_reg * B[p][j + 1];
                c_02_reg += a_0p_reg * B[p][j + 2];
                c_03_reg += a_0p_reg * B[p][j + 3];
            }
            C[i][j + 0] += c_00_reg;
            C[i][j + 1] += c_01_reg;
            C[i][j + 2] += c_02_reg;
            C[i][j + 3] += c_03_reg;
        }
        printf("col %d done\n", j);
    }
}

void compute_1_4_reg_col_priority_ptr(double **A, double **B, double **C, int m, int n, int k)
{
    for (int j = 0; j < n; j += 4)
    {

        for (int i = 0; i < m; i++)
        {

            register double c_00_reg = 0.0;
            register double c_01_reg = 0.0;
            register double c_02_reg = 0.0;
            register double c_03_reg = 0.0;
            register double a_0p_reg = 0.0;

            // B矩阵各列的首地址，并可以通过这些地址自加遍历完相应的列
            double *bp0_ptr = B[0];
            double *bp1_ptr = B[1];
            double *bp2_ptr = B[2];
            double *bp3_ptr = B[3];

            for (int p = 0; p < k; p++)
            {
                a_0p_reg = A[i][p];

                c_00_reg += (a_0p_reg * (*bp0_ptr++));
                c_01_reg += (a_0p_reg * (*bp1_ptr++));
                c_02_reg += (a_0p_reg * (*bp2_ptr++));
                c_03_reg += (a_0p_reg * (*bp3_ptr++));
            }
            C[i][j + 0] += c_00_reg;
            C[i][j + 1] += c_01_reg;
            C[i][j + 2] += c_02_reg;
            C[i][j + 3] += c_03_reg;
        }
        printf("col %d done\n", j);
    }
}

void compute_4_4(double **A, double **B, double **C, int m, int n, int k)
{
    for (int i = 0; i < m; i += 4)
    {
        for (int j = 0; j < n; j += 4)
        {
            C[i + 0][j + 0] = 0;
            C[i + 0][j + 1] = 0;
            C[i + 0][j + 2] = 0;
            C[i + 0][j + 3] = 0;

            C[i + 1][j + 0] = 0;
            C[i + 1][j + 1] = 0;
            C[i + 1][j + 2] = 0;
            C[i + 1][j + 3] = 0;

            C[i + 2][j + 0] = 0;
            C[i + 2][j + 1] = 0;
            C[i + 2][j + 2] = 0;
            C[i + 2][j + 3] = 0;

            C[i + 3][j + 0] = 0;
            C[i + 3][j + 1] = 0;
            C[i + 3][j + 2] = 0;
            C[i + 3][j + 3] = 0;

            for (int p = 0; p < k; p++)
            {
                C[i + 0][j + 0] += A[i + 0][p] * B[p][j + 0];
                C[i + 0][j + 1] += A[i + 0][p] * B[p][j + 1];
                C[i + 0][j + 2] += A[i + 0][p] * B[p][j + 2];
                C[i + 0][j + 3] += A[i + 0][p] * B[p][j + 3];

                C[i + 1][j + 0] += A[i + 1][p] * B[p][j + 0];
                C[i + 1][j + 1] += A[i + 1][p] * B[p][j + 1];
                C[i + 1][j + 2] += A[i + 1][p] * B[p][j + 2];
                C[i + 1][j + 3] += A[i + 1][p] * B[p][j + 3];

                C[i + 2][j + 0] += A[i + 2][p] * B[p][j + 0];
                C[i + 2][j + 1] += A[i + 2][p] * B[p][j + 1];
                C[i + 2][j + 2] += A[i + 2][p] * B[p][j + 2];
                C[i + 2][j + 3] += A[i + 2][p] * B[p][j + 3];

                C[i + 3][j + 0] += A[i + 3][p] * B[p][j + 0];
                C[i + 3][j + 1] += A[i + 3][p] * B[p][j + 1];
                C[i + 3][j + 2] += A[i + 3][p] * B[p][j + 2];
                C[i + 3][j + 3] += A[i + 3][p] * B[p][j + 3];
            }
        }
        printf("row %d done\n", i);
    }
}

void compute_4_4_reg(double **A, double **B, double **C, int m, int n, int k)
{
    for (int i = 0; i < m; i += 4)
    {
        for (int j = 0; j < n; j += 4)
        {
            C[i + 0][j + 0] = 0;
            C[i + 0][j + 1] = 0;
            C[i + 0][j + 2] = 0;
            C[i + 0][j + 3] = 0;

            C[i + 1][j + 0] = 0;
            C[i + 1][j + 1] = 0;
            C[i + 1][j + 2] = 0;
            C[i + 1][j + 3] = 0;

            C[i + 2][j + 0] = 0;
            C[i + 2][j + 1] = 0;
            C[i + 2][j + 2] = 0;
            C[i + 2][j + 3] = 0;

            C[i + 3][j + 0] = 0;
            C[i + 3][j + 1] = 0;
            C[i + 3][j + 2] = 0;
            C[i + 3][j + 3] = 0;

            register double c_00_reg = 0.0;
            register double c_01_reg = 0.0;
            register double c_02_reg = 0.0;
            register double c_03_reg = 0.0;

            register double c_10_reg = 0.0;
            register double c_11_reg = 0.0;
            register double c_12_reg = 0.0;
            register double c_13_reg = 0.0;

            register double c_20_reg = 0.0;
            register double c_21_reg = 0.0;
            register double c_22_reg = 0.0;
            register double c_23_reg = 0.0;

            register double c_30_reg = 0.0;
            register double c_31_reg = 0.0;
            register double c_32_reg = 0.0;
            register double c_33_reg = 0.0;

            register double a_0p_reg = 0.0;
            register double a_1p_reg = 0.0;
            register double a_2p_reg = 0.0;
            register double a_3p_reg = 0.0;

            for (int p = 0; p < k; p++)
            {
                a_0p_reg = A[i + 0][p];
                a_1p_reg = A[i + 1][p];
                a_2p_reg = A[i + 2][p];
                a_3p_reg = A[i + 3][p];

                printf("[%lf, %lf, %lf, %lf]\n", a_0p_reg, a_1p_reg, a_2p_reg, a_3p_reg);

                c_00_reg += a_0p_reg * B[p][j + 0];
                c_01_reg += a_0p_reg * B[p][j + 1];
                c_02_reg += a_0p_reg * B[p][j + 2];
                c_03_reg += a_0p_reg * B[p][j + 3];

                c_10_reg += a_1p_reg * B[p][j + 0];
                c_11_reg += a_1p_reg * B[p][j + 1];
                c_12_reg += a_1p_reg * B[p][j + 2];
                c_13_reg += a_1p_reg * B[p][j + 3];

                c_20_reg += a_2p_reg * B[p][j + 0];
                c_21_reg += a_2p_reg * B[p][j + 1];
                c_22_reg += a_2p_reg * B[p][j + 2];
                c_23_reg += a_2p_reg * B[p][j + 3];

                c_30_reg += a_3p_reg * B[p][j + 0];
                c_31_reg += a_3p_reg * B[p][j + 1];
                c_32_reg += a_3p_reg * B[p][j + 2];
                c_33_reg += a_3p_reg * B[p][j + 3];
                // printf("[%lf, %lf, %lf, %lf]\n", B[p][j + 0], B[p][j + 1], B[p][j + 2], B[p][j + 3]);
            }
            C[i + 0][j + 0] += c_00_reg;
            C[i + 0][j + 1] += c_01_reg;
            C[i + 0][j + 2] += c_02_reg;
            C[i + 0][j + 3] += c_03_reg;

            C[i + 1][j + 0] += c_10_reg;
            C[i + 1][j + 1] += c_11_reg;
            C[i + 1][j + 2] += c_12_reg;
            C[i + 1][j + 3] += c_13_reg;

            C[i + 2][j + 0] += c_20_reg;
            C[i + 2][j + 1] += c_21_reg;
            C[i + 2][j + 2] += c_22_reg;
            C[i + 2][j + 3] += c_23_reg;

            C[i + 3][j + 0] += c_30_reg;
            C[i + 3][j + 1] += c_31_reg;
            C[i + 3][j + 2] += c_32_reg;
            C[i + 3][j + 3] += c_33_reg;
        }
        printf("row %d done\n", i);
    }
}

void compute_4_4_loop_reduce(double **A, double **B, double **C, int m, int n, int k)
{

    for (int i = 0; i < m; i += 4)
    {
        for (int j = 0; j < n; j += 4)
        {
            // C[i + 0][j + 0] = 0;
            // C[i + 0][j + 1] = 0;
            // C[i + 0][j + 2] = 0;
            // C[i + 0][j + 3] = 0;

            // C[i + 1][j + 0] = 0;
            // C[i + 1][j + 1] = 0;
            // C[i + 1][j + 2] = 0;
            // C[i + 1][j + 3] = 0;

            // C[i + 2][j + 0] = 0;
            // C[i + 2][j + 1] = 0;
            // C[i + 2][j + 2] = 0;
            // C[i + 2][j + 3] = 0;

            // C[i + 3][j + 0] = 0;
            // C[i + 3][j + 1] = 0;
            // C[i + 3][j + 2] = 0;
            // C[i + 3][j + 3] = 0;

            for (int p = 0; p < k; p += 4)
            {
                C[i + 0][j + 0] += A[i + 0][p + 0] * B[p + 0][j + 0];
                C[i + 0][j + 0] += A[i + 0][p + 1] * B[p + 1][j + 0];
                C[i + 0][j + 0] += A[i + 0][p + 2] * B[p + 2][j + 0];
                C[i + 0][j + 0] += A[i + 0][p + 3] * B[p + 3][j + 0];

                C[i + 0][j + 1] += A[i + 0][p + 0] * B[p + 0][j + 1];
                C[i + 0][j + 1] += A[i + 0][p + 1] * B[p + 1][j + 1];
                C[i + 0][j + 1] += A[i + 0][p + 2] * B[p + 2][j + 1];
                C[i + 0][j + 1] += A[i + 0][p + 3] * B[p + 3][j + 1];

                C[i + 0][j + 2] += A[i + 0][p + 0] * B[p + 0][j + 2];
                C[i + 0][j + 2] += A[i + 0][p + 1] * B[p + 1][j + 2];
                C[i + 0][j + 2] += A[i + 0][p + 2] * B[p + 2][j + 2];
                C[i + 0][j + 2] += A[i + 0][p + 3] * B[p + 3][j + 2];

                C[i + 0][j + 3] += A[i + 0][p + 0] * B[p + 0][j + 3];
                C[i + 0][j + 3] += A[i + 0][p + 1] * B[p + 1][j + 3];
                C[i + 0][j + 3] += A[i + 0][p + 2] * B[p + 2][j + 3];
                C[i + 0][j + 3] += A[i + 0][p + 3] * B[p + 3][j + 3];

                C[i + 1][j + 0] += A[i + 1][p + 0] * B[p + 0][j + 0];
                C[i + 1][j + 0] += A[i + 1][p + 1] * B[p + 1][j + 0];
                C[i + 1][j + 0] += A[i + 1][p + 2] * B[p + 2][j + 0];
                C[i + 1][j + 0] += A[i + 1][p + 3] * B[p + 3][j + 0];

                C[i + 1][j + 1] += A[i + 1][p + 0] * B[p + 0][j + 1];
                C[i + 1][j + 1] += A[i + 1][p + 1] * B[p + 1][j + 1];
                C[i + 1][j + 1] += A[i + 1][p + 2] * B[p + 2][j + 1];
                C[i + 1][j + 1] += A[i + 1][p + 3] * B[p + 3][j + 1];

                C[i + 1][j + 2] += A[i + 1][p + 0] * B[p + 0][j + 2];
                C[i + 1][j + 2] += A[i + 1][p + 1] * B[p + 1][j + 2];
                C[i + 1][j + 2] += A[i + 1][p + 2] * B[p + 2][j + 2];
                C[i + 1][j + 2] += A[i + 1][p + 3] * B[p + 3][j + 2];

                C[i + 1][j + 3] += A[i + 1][p + 0] * B[p + 0][j + 3];
                C[i + 1][j + 3] += A[i + 1][p + 1] * B[p + 1][j + 3];
                C[i + 1][j + 3] += A[i + 1][p + 2] * B[p + 2][j + 3];
                C[i + 1][j + 3] += A[i + 1][p + 3] * B[p + 3][j + 3];

                C[i + 2][j + 0] += A[i + 2][p + 0] * B[p + 0][j + 0];
                C[i + 2][j + 0] += A[i + 2][p + 1] * B[p + 1][j + 0];
                C[i + 2][j + 0] += A[i + 2][p + 2] * B[p + 2][j + 0];
                C[i + 2][j + 0] += A[i + 2][p + 3] * B[p + 3][j + 0];

                C[i + 2][j + 1] += A[i + 2][p + 0] * B[p + 0][j + 1];
                C[i + 2][j + 1] += A[i + 2][p + 1] * B[p + 1][j + 1];
                C[i + 2][j + 1] += A[i + 2][p + 2] * B[p + 2][j + 1];
                C[i + 2][j + 1] += A[i + 2][p + 3] * B[p + 3][j + 1];

                C[i + 2][j + 2] += A[i + 2][p + 0] * B[p + 0][j + 2];
                C[i + 2][j + 2] += A[i + 2][p + 1] * B[p + 1][j + 2];
                C[i + 2][j + 2] += A[i + 2][p + 2] * B[p + 2][j + 2];
                C[i + 2][j + 2] += A[i + 2][p + 3] * B[p + 3][j + 2];

                C[i + 2][j + 3] += A[i + 2][p + 0] * B[p + 0][j + 3];
                C[i + 2][j + 3] += A[i + 2][p + 1] * B[p + 1][j + 3];
                C[i + 2][j + 3] += A[i + 2][p + 2] * B[p + 2][j + 3];
                C[i + 2][j + 3] += A[i + 2][p + 3] * B[p + 3][j + 3];

                C[i + 3][j + 0] += A[i + 3][p + 0] * B[p + 0][j + 0];
                C[i + 3][j + 0] += A[i + 3][p + 1] * B[p + 1][j + 0];
                C[i + 3][j + 0] += A[i + 3][p + 2] * B[p + 2][j + 0];
                C[i + 3][j + 0] += A[i + 3][p + 3] * B[p + 3][j + 0];

                C[i + 3][j + 1] += A[i + 3][p + 0] * B[p + 0][j + 1];
                C[i + 3][j + 1] += A[i + 3][p + 1] * B[p + 1][j + 1];
                C[i + 3][j + 1] += A[i + 3][p + 2] * B[p + 2][j + 1];
                C[i + 3][j + 1] += A[i + 3][p + 3] * B[p + 3][j + 1];

                C[i + 3][j + 2] += A[i + 3][p + 0] * B[p + 0][j + 2];
                C[i + 3][j + 2] += A[i + 3][p + 1] * B[p + 1][j + 2];
                C[i + 3][j + 2] += A[i + 3][p + 2] * B[p + 2][j + 2];
                C[i + 3][j + 2] += A[i + 3][p + 3] * B[p + 3][j + 2];

                C[i + 3][j + 3] += A[i + 3][p + 0] * B[p + 0][j + 3];
                C[i + 3][j + 3] += A[i + 3][p + 1] * B[p + 1][j + 3];
                C[i + 3][j + 3] += A[i + 3][p + 2] * B[p + 2][j + 3];
                C[i + 3][j + 3] += A[i + 3][p + 3] * B[p + 3][j + 3];
            }
        }
        printf("row %d done\n", i);
    }
}

void compute_sse(double **A, double **B, double **C, int m, int n, int k)
{
    for (int j = 0; j < n; j += 4)
    {
        for (int i = 0; i < m; i += 4)
        {
            double *bp0_ptr = B[0];
            double *bp1_ptr = B[1];
            double *bp2_ptr = B[2];
            double *bp3_ptr = B[3];

            __m128d a_0p_a_1p_vreg = _mm_setzero_pd();
            __m128d a_2p_a_3p_vreg = _mm_setzero_pd();

            __m128d b_p0_vreg = _mm_setzero_pd();
            __m128d b_p1_vreg = _mm_setzero_pd();
            __m128d b_p2_vreg = _mm_setzero_pd();
            __m128d b_p3_vreg = _mm_setzero_pd();

            __m128d c_00_c_10_vreg = _mm_setzero_pd();
            __m128d c_01_c_11_vreg = _mm_setzero_pd();
            __m128d c_02_c_12_vreg = _mm_setzero_pd();
            __m128d c_03_c_13_vreg = _mm_setzero_pd();

            __m128d c_20_c_10_vreg = _mm_setzero_pd();
            __m128d c_21_c_11_vreg = _mm_setzero_pd();
            __m128d c_22_c_12_vreg = _mm_setzero_pd();
            __m128d c_23_c_13_vreg = _mm_setzero_pd();

            for (int p = 0; p < k; p++)
            {
                // sse使用128位寄存器xmm,故将两个double变量存入其中
                // A为动态分配的指针数组，A[0]指向第一行的起始地址，遍历指针A[0]可以得到A[0][1],A[0][2]......的值
                // 本函数内传入的参数**A实际上为矩阵A的转置，
                // 如何使用循环变量p实现对A中逐行隔2取地址
                a_0p_a_1p_vreg = _mm_loadu_pd((double *)&A[p][j]);
                a_2p_a_3p_vreg = _mm_loadu_pd((double *)&A[p][j + 2]);

                // printf("i=%d\n", i);

                double val_0_1[2], val_2_3[2];

                _mm_storeu_pd(val_0_1, a_0p_a_1p_vreg);
                _mm_storeu_pd(val_2_3, a_2p_a_3p_vreg);
                printf("[%lf, %lf, %lf, %lf]\n", val_0_1[0], val_0_1[1], val_2_3[1], val_2_3[1]);

                // p++是对内层分量维度的遍历，

                // B的存储结构
                // B内的数据要以何种形式取出
                //
                // B内数据如何程序化循环取出
                b_p0_vreg = _mm_loadu_pd((double *)(&B[p + 0][j]));
                b_p1_vreg = _mm_loadu_pd((double *)(&B[p + 2][j]));

                b_p2_vreg = _mm_loadu_pd((double *)(&B[p + 2][j]));
                b_p3_vreg = _mm_loadu_pd((double *)(&B[p + 3][j]));

                // b_p0_vreg = _mm_loadu_pd((double *)(bp0_ptr++));
                // b_p1_vreg = _mm_loadu_pd((double *)(bp1_ptr++));
                // b_p2_vreg = _mm_loadu_pd((double *)(bp2_ptr++));
                // b_p3_vreg = _mm_loadu_pd((double *)(bp3_ptr++));

                // double b_0[2], b_1[2], b_2[2], b_3[2];
                // _mm_storeu_pd(b_0, b_p0_vreg);
                // _mm_storeu_pd(b_1, b_p1_vreg);
                // _mm_storeu_pd(b_2, b_p2_vreg);
                // _mm_storeu_pd(b_3, b_p3_vreg);
                // printf("[%lf, %lf, %lf, %lf]\n", b_0[0], b_0[1], b_1[0], b_1[1], b_3[0], b_3[1]);

                c_00_c_10_vreg = _mm_add_pd(_mm_mul_pd(a_0p_a_1p_vreg, b_p0_vreg), c_00_c_10_vreg);
                c_01_c_11_vreg = _mm_add_pd(_mm_mul_pd(a_0p_a_1p_vreg, b_p1_vreg), c_01_c_11_vreg);
                c_02_c_12_vreg = _mm_add_pd(_mm_mul_pd(a_0p_a_1p_vreg, b_p2_vreg), c_02_c_12_vreg);
                c_03_c_13_vreg = _mm_add_pd(_mm_mul_pd(a_0p_a_1p_vreg, b_p3_vreg), c_03_c_13_vreg);

                c_20_c_10_vreg = _mm_add_pd(_mm_mul_pd(a_2p_a_3p_vreg, b_p0_vreg), c_20_c_10_vreg);
                c_21_c_11_vreg = _mm_add_pd(_mm_mul_pd(a_2p_a_3p_vreg, b_p1_vreg), c_21_c_11_vreg);
                c_22_c_12_vreg = _mm_add_pd(_mm_mul_pd(a_2p_a_3p_vreg, b_p2_vreg), c_22_c_12_vreg);
                c_23_c_13_vreg = _mm_add_pd(_mm_mul_pd(a_2p_a_3p_vreg, b_p3_vreg), c_23_c_13_vreg);
            }

            // 将计算完成的向量化结果从xmm中取出到程序的二维数组C中
            double val[2];
            _mm_storeu_pd(val, c_00_c_10_vreg);
            C[i + 0][j + 0] += val[0];
            C[i + 0][j + 1] += val[1];
            _mm_storeu_pd(val, c_01_c_11_vreg);
            C[i + 0][j + 2] += val[0];
            C[i + 0][j + 3] += val[1];

            _mm_storeu_pd(val, c_02_c_12_vreg);
            C[i + 1][j + 0] += val[0];
            C[i + 1][j + 1] += val[1];
            _mm_storeu_pd(val, c_03_c_13_vreg);
            C[i + 1][j + 2] += val[0];
            C[i + 1][j + 3] += val[1];

            _mm_storeu_pd(val, c_20_c_10_vreg);
            C[i + 2][j + 0] += val[0];
            C[i + 2][j + 1] += val[1];
            _mm_storeu_pd(val, c_21_c_11_vreg);
            C[i + 2][j + 2] += val[0];
            C[i + 2][j + 3] += val[1];

            _mm_storeu_pd(val, c_22_c_12_vreg);
            C[i + 3][j + 0] += val[0];
            C[i + 3][j + 1] += val[1];
            _mm_storeu_pd(val, c_23_c_13_vreg);
            C[i + 3][j + 2] += val[0];
            C[i + 3][j + 3] += val[1];
        }

        printf("col %d done\n", j);
    }
}

double rand_double()
{
    return rand() / (RAND_MAX + 1.0);
}

void print_matrix(double **m, int dim1, int dim2)
{
    for (int i = 0; i < dim1; i++)
    {
        for (int j = 0; j < dim2; j++)
        {
            printf("%lf ", m[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    // int m = 5000;
    // int n = 5000;
    // int k = 5000;

    int m = 8;
    int n = 8;
    int k = 8;

    double **A = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < k; i++)
    {
        A[i] = (double *)malloc(sizeof(double) * k);
    }

    double **A_col_prior = (double **)malloc(k * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        A_col_prior[i] = (double *)malloc(sizeof(double) * n);
    }

    double **B = (double **)malloc(k * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        B[i] = (double *)malloc(sizeof(double) * n);
    }

    double **B_col_prior = (double **)malloc(k * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        B_col_prior[i] = (double *)malloc(sizeof(double) * n);
    }

    double **C = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        C[i] = (double *)malloc(sizeof(double) * n);
    }

    // int *A = (int *)malloc(m * k * sizeof(int));
    // int *B = (int *)malloc(k * n * sizeof(int));
    // int *C = (int *)malloc(m * n * sizeof(int));

    // srand(time(0));
    srand(0); // 为了验证优化后的结果是否正确，故使用相同的一组随机数

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = rand_double();
            A_col_prior[j][i] = A[i][j];
        }
    }
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            B[i][j] = rand_double();
            B_col_prior[j][i] = B[i][j];
        }
    }

    // printf("matrix B:\n");
    // print_matrix(B, k, n);
    // printf("\n");
    // printf("matrix B_prior:\n");
    // print_matrix(B_col_prior, k, n);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = 0;
        }
    }

    // compute_1_4(A, B, C, m, n, k);
    // compute_1_4_col_priority(A, B, C, m, n, k);
    // compute_1_4_reg(A, B, C, m, n, k);
    // compute_1_4_reg_col_priority(A, B, C, m, n, k);
    // compute_1_4_reg_col_priority_ptr(A, B_col_prior, C, m, n, k);
    // compute_4_4(A, B, C, m, n, k);
    // compute_4_4_reg(A, B, C, m, n, k);
    //  compute_4_4_loop_reduce(A, B, C, m, n, k);
    compute_sse(A, B_col_prior, C, m, n, k);
    // compute_naive(A, B, C, m, n, k);

    print_matrix(C, m, n);

    for (int i = 0; i < k; i++)
    {
        free(A[i]);
        free(A_col_prior[i]);
    }
    free(A);

    for (int i = 0; i < k; i++)
    {
        free(B[i]);
        free(B_col_prior[i]);
    }
    free(B);

    for (int i = 0; i < m; i++)
    {
        free(C[i]);
    }
    free(C);

    return 0;
}