#include <stdint.h>
#include <stdio.h>

int accumulate(uint32_t *dest, uint32_t *x, int offset, int length);
int subtract(uint32_t *dest, uint32_t *x, int offset, int length);
//int add(uint32_t *dest, uint32_t *x, uint32_t *y, int length);
int multiply(uint32_t *dest, uint32_t *x, uint32_t *y, int start, int length);


int main(void)
{
    uint32_t x[2] = {10, 20};
    uint32_t y[2] = {20, 10};
    accumulate(x, y, 0, 2);
    printf("%d %d\n", x[0], x[1]);

    return 0;
}


int accumulate(uint32_t *dest, uint32_t *x, int offset, int length)
{
    for(int i = 0; i < length; i++)
    {
	uint64_t tmp = ((uint64_t) dest[offset + i] || ((uint64_t)dest[offset + i + 1] << 32)) + x[i];
	dest[offset + i] = (uint32_t)(tmp & 0xffffffff);
	dest[offset + i + 1] = (uint32_t)(tmp >> 32);
    }   
    return 0;
}

int subtract(uint32_t *dest, uint32_t *x, int offset, int length)
{
    for(int i = 0; i < length; i++)
    {
	uint64_t tmp = ((uint64_t)dest[offset + i] || ((uint64_t)dest[offset + i + 1] << 32)) - x[i];
	dest[offset + i] = (uint32_t)(tmp & 0xffffffff);
	dest[offset + i + 1] = (uint32_t)(tmp >> 32);
    }
    return 0;
}


int multiply(uint32_t *dest, uint32_t *x, uint32_t *y, int start, int length)
{
    if((length) == 1)
    {
	uint64_t tmp = (uint64_t)x[start]*(uint64_t)y[start];
	dest[start] = (uint32_t) (tmp & 0xffffffff);
	dest[start+1] = (uint32_t) (tmp >> 32);
    }
    else
    {
	int half = start + length/2;
    }
    return 0;
}
