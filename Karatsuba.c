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
    uint32_t *xy;
    accumulate(x, y, 0, 2);
    multiply(&xy, x, y, 0, 2);
    printf("%d %d %d %d\n", xy[0], xy[1], xy[2], xy[3]);

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
    memset(*dest, 0, sizeof(uint32_t)*(maxLength + 1));

    // Add the two numbers by calling accumulate
    accumulate( *dest, x, offsetX, lengthX );
    accumulate( *dest, y, offsetY, lengthY );
    int i = 0;
    while( (dest[i] != 0) && (i < (maxLength+1)) )
    {
	i++;
    }

    // Done and done
    return i;
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
    memset(*dest, 0, sizeof(uint32_t)*2*length);

    if((length) == 1)
    {
	uint64_t tmp = (uint64_t)x[start]*(uint64_t)y[start];
	(*dest)[start] = (uint32_t) (tmp & 0xffffffff);
	(*dest)[start+1] = (uint32_t) (tmp >> 32);
	if( (*dest)[start+1] == 0 )
	    return 1;
	else
	    return 2;
    }
    else
    {
	int lengthL = length/2;
	int lengthH = length - lengthL;
	printf("%d %d %d", length, lengthL, lengthH);
	// Multiply A and C
	uint32_t *ac, *bd, *apb, *cpd, *ip;

	int lengthAC = multiply(&ac, x, y, start, lengthL);		// A*C
	int lengthBD = multiply(&bd, x, y, start+lengthL, lengthH);	// B*D
	int lengthAPB = add(&apb, x, start, lengthL, x, start+lengthL, lengthH);    // APB = A + B 
	int lengthCPD = add(&cpd, y, start, lengthL, y, start+lengthL, lengthH);    // CPD = C + D
	printf("\t%d %d %d %d\n", lengthAC, lengthBD, lengthAPB, lengthCPD);

	int maxLength = (lengthAPB > lengthCPD) ? lengthAPB : lengthCPD;
	int lengthIP = multiply(&ip, apb, cpd, 0, maxLength);		// IP = APB*CPD

	decrease(ip, ac, 0, lengthAC);			// IP = IP - AC 
	decrease(ip, bd, 0, lengthBD);			// IP = IP - BD

	accumulate(*dest, ac, start, lengthAC);
	accumulate(*dest, ip, start+lengthL, lengthIP);
	accumulate(*dest, bd, start+length, lengthBD);

	free(ac); free(bd); free(apb); free(cpd); free(ip);

	int i = 0;
	while( ((*dest)[start + i] != 0) && (i < lengthH*2) )
	{
	    i++;
	}
	return i;
    }
}
