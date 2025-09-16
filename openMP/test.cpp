#include <stdio.h>
#include <omp.h>
int main(int argc, char *argv[])
{
#pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();

        printf("Thread %d before barrier\n", tid);

#pragma omp barrier // 所有线程必须在此同步

        printf("Thread %d after barrier\n", tid);
    }

    return 0;
}