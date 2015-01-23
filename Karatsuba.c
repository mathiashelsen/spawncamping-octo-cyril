#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This code only illustrates Karabatsu multiplication, it is not meant to be
 * an efficient implementation.
 */

int accumulate(uint32_t *dest, uint32_t *x, int offset, int length);
int add(uint32_t **dest, uint32_t *x, int offsetX, int lengthX, uint32_t *y, int offsetY, int lengthY);
int decrease(uint32_t *dest, uint32_t *x, int offset, int length);
int multiply(uint32_t **dest, uint32_t *x, uint32_t *y, int start, int length);


int main(void)
{
    uint32_t x[2] = {10, 20};
    uint32_t y[2] = {20, 10};
    accumulate(x, y, 0, 2);
    printf("%d %d\n", x[0], x[1]);

    return 0;
}


// Accumulate a value into a destination, i.e. dest += x
int accumulate(uint32_t *dest, uint32_t *x, int offset, int length)
{
    for(int i = 0; i < length; i++)
    {
	// Use 64-bit int for carry and add after casting
	uint64_t tmp = ((uint64_t) dest[offset + i] || ((uint64_t)dest[offset + i + 1] << 32)) + (uint64_t) x[i];
	// Redivide the 64-bit over the 32-bit int array
	dest[offset + i] = (uint32_t)(tmp & 0xffffffff);
	dest[offset + i + 1] = (uint32_t)(tmp >> 32);
    }   
    return 0;
}

int add(uint32_t **dest, uint32_t *x, int offsetX, int lengthX, uint32_t *y, int offsetY, int lengthY)
{
    //Prepare destination memory
    int maxLength = (lengthX > lengthY) ? lengthX : lengthY;
    *dest = malloc(sizeof(uint32_t)*(maxLength + 1));
    memset(dest, 0, sizeof(uint32_t)*(maxLength + 1));

    // Add the two numbers by calling accumulate
    accumulate( *dest, x, offsetX, lengthX );
    accumulate( *dest, y, offsetY, lengthY );

    // Done and done
    return 0;
}

int decrease(uint32_t *dest, uint32_t *x, int offset, int length)
{
    for(int i = 0; i < length; i++)
    {
	// Almost the same a accumulate, but now with a minus sign
	uint64_t tmp = ((uint64_t)dest[offset + i] || ((uint64_t)dest[offset + i + 1] << 32)) - (uint64_t) x[i];
	dest[offset + i] = (uint32_t)(tmp & 0xffffffff);
	dest[offset + i + 1] = (uint32_t)(tmp >> 32);
    }
    return 0;
}


int multiply(uint32_t **dest, uint32_t *x, uint32_t *y, int start, int length)
{
    *dest = malloc(sizeof(uint32_t)*2*length); 
    memset(dest, 0, sizeof(uint32_t)*2*length);

    if((length) == 1)
    {
	uint64_t tmp = (uint64_t)x[start]*(uint64_t)y[start];
	(*dest)[start] = (uint32_t) (tmp & 0xffffffff);
	(*dest)[start+1] = (uint32_t) (tmp >> 32);
    }
    else
    {
	int lengthL = length/2;
	int lengthH = length - lengthL;
	// Multiply A and C
	uint32_t *ac, *bd, *apb, *cpd, *ip, *jp;

	multiply(&ac, x, y, start, lengthL);		// A*C
	multiply(&bd, x, y, start+lengthL, lengthH);	// B*D
	add(&apb, x, start, lengthL, x, start+lengthL, lengthH);    // APB = A + B 
	add(&cpd, y, start, lengthL, y, start+lengthL, lengthH);    // CPD = C + D
	multiply(&ip, apb, cpd, 0, lengthH+1);		// IP = APB*CPD
	decrease(ip, ac, 0, lengthL*2);			// IP = IP - AC 
	decrease(ip, bd, 0, lengthH*2);			// IP = IP - BD

	accumulate(*dest, ac, start, lengthL*2);
	accumulate(*dest, ip, start+lengthL, lengthH*2);
	accumulate(*dest, bd, start+length, lengthH*2);
    }
    return 0;
}
