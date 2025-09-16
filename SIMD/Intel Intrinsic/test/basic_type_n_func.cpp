#include <iostream>
#include <immintrin.h>
#include <stdlib.h>
using namespace std;

void print(__m128 value)
{
    float *v = (float *)&value;
    cout << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << endl;
}

__m128 func_load(float *mem_addr)
{
    return _mm_load_ps(mem_addr);
}

__m128 func_set(float *mem_addr)
{
    return _mm_set_ps(mem_addr[3], mem_addr[2], mem_addr[1], mem_addr[0]);
}

__m128 direct_init(float *mem_addr)
{
    __m128 value = {mem_addr[0], mem_addr[1], mem_addr[2], mem_addr[3]};
    return value;
}

int main()
{
    float *mem_addr = (float *)malloc(sizeof(float) * 4);
    mem_addr[0] = 1.0f;
    mem_addr[1] = 2.0f;
    mem_addr[2] = 3.0f;
    mem_addr[3] = 4.0f;

    print(func_load((float *)mem_addr));
    print(func_set((float *)mem_addr));
    print(direct_init((float *)mem_addr));

    return 0;
}