#include <stdio.h>
#include <stdlib.h>

int main()
{
    int m = 3;
    int n = 3;

    // 使用指针数组方式创建二维数组
    int **A = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        A[i] = (int *)malloc(n * sizeof(int));
    }

    int **B = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        B[i] = (int *)malloc(n * sizeof(int));
    }

    int temp = 0;
    int temp1 = 0;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
        {
            A[i][j] = temp++;
        }

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
        {
            B[j][i] = temp1++;
        }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }

    int *ptr0 = A[0];
    int *ptr1 = A[1];
    int *ptr2 = A[2];

    printf("&A[0]++ self iterate: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", *ptr0++);
    }
    printf("\n");

    printf("&A[1]++ self iterate: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", *ptr1++);
    }
    printf("\n");

    printf("&A[2]++ self iterate: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", *ptr2++);
    }
    printf("\n");

    int *ptr00 = B[0];
    int *ptr01 = B[1];
    int *ptr02 = B[2];

    printf("&B[0]++ self iterate: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", *ptr00++);
    }
    printf("\n");

    printf("&B[1]++ self iterate: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", *ptr01++);
    }
    printf("\n");

    printf("&B[2]++ self iterate: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", *ptr02++);
    }
    printf("\n");

    // 使用一维数组模拟二维数组
    int *C = (int *)malloc(m * n * sizeof(int));

    //
    for (int i = 0; i < m; i++)
    {
        free(A[i]);
    }
    free(A);

    //
    free(B);

    return 0;
}