
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This code only illustrates Karabatsu multiplication, it is not meant to be
 * an efficient implementation.
 */

int accumulate(uint32_t *dest, int offsetDest, uint32_t *x, int offsetX, int length);;
int add(uint32_t **dest, uint32_t *x, int offsetX, int lengthX, uint32_t *y, int offsetY, int lengthY);
int decrease(uint32_t *dest, uint32_t *x, int offset, int length);
int multiply(uint32_t **dest, int store_start, uint32_t *x, uint32_t *y, int startxy, int length);


int main(void)
{
    uint32_t x[4] = {0xffffffff, 0xffffffff};
    uint32_t y[4] = {400, 500};
    uint32_t *xy;
    //accumulate(x, 0, y, 0, 4);
    multiply(&xy, 0, x, y, 0, 2);
    for(int i = 0; i < 6; i++)
    {
	printf("%u ", xy[i]);
    }
    printf("\n");

    return 0;
}


int accumulate(uint32_t *dest, int offsetDest, uint32_t *x, int offsetX, int length)
{

	/* DANGER DANGER: THIS DOES NOT WORK PROPERLY PROBLEMS WITH OVERFLOW!!! */
    for(int i = 0; i < length; i++)
    {
	// Use 64-bit int for carry and add after casting
	uint64_t tmp = (uint64_t) dest[i+offsetDest];
	tmp |= ((uint64_t)dest[i + offsetDest + 1] << 32);
	tmp += (uint64_t) x[offsetX + i];
	// Redivide the 64-bit over the 32-bit int array
	dest[offsetDest + i] = (uint32_t)(tmp & 0xffffffff);
	dest[offsetDest + i + 1] = (uint32_t)(tmp >> 32);
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
    accumulate( *dest, 0, x, offsetX, lengthX );
    accumulate( *dest, 0, y, offsetY, lengthY );
    int i = maxLength;
    while( (i >= 0) && (dest[i] == 0))
	i--;

    return i;
}

int decrease(uint32_t *dest, uint32_t *x, int offset, int length)
{
    for(int i = 0; i < length; i++)
    {
	// Almost the same a accumulate, but now with a minus sign
	uint64_t tmp = ((uint64_t) dest[i] | ((uint64_t)dest[i + 1] << 32)) - (uint64_t) x[offset + i];
	// Redivide the 64-bit over the 32-bit int array
	dest[i] = (uint32_t)(tmp & 0xffffffff);
	dest[i + 1] = (uint32_t)(tmp >> 32);
    }
    return 0;
}


int multiply(uint32_t **dest, int store_start, uint32_t *x, uint32_t *y, int startxy, int length)
{
    *dest = malloc(sizeof(uint32_t)*2*length); 
    memset(*dest, 0, sizeof(uint32_t)*2*length);

    if(length == 1)
    {
	uint64_t tmp = (uint64_t)x[startxy]*(uint64_t)y[startxy];
	(*dest)[0] = (uint32_t) (tmp & 0xffffffff);
	(*dest)[1] = (uint32_t) (tmp >> 32);
	if( (*dest)[1] == 0 )
	    return 1;
	else
	    return 2;
    }
    else
    {
	int lengthL = length/2;
	int lengthH = length - lengthL;
	// Multiply A and C
	uint32_t *ac, *bd, *apb, *cpd, *ip;
	int lengthAPB = add(&apb, x, startxy, lengthL, x, startxy+lengthL, lengthH);    // APB = A + B 
	int lengthCPD = add(&cpd, y, startxy, lengthL, y, startxy+lengthL, lengthH);    // CPD = C + D

	int maxLength = (lengthAPB > lengthCPD) ? lengthAPB : lengthCPD;
	int lengthIP = multiply(&ip, 0, apb, cpd, 0, maxLength);		// IP = APB*CPD
	int lengthAC = multiply(&ac, 0, x, y, startxy, lengthL);		// A*C
	int lengthBD = multiply(&bd, 0, x, y, startxy+lengthL, lengthH);	// B*D

	decrease(ip, ac, 0, lengthAC);			// IP = IP - AC 
	decrease(ip, bd, 0, lengthBD);			// IP = IP - BD

	accumulate(*dest, store_start, ac, 0, lengthAC);
	accumulate(*dest, store_start+lengthL, ip, 0, lengthIP);
	accumulate(*dest, store_start+2*lengthL, bd, 0, lengthBD);

	free(ac); free(bd); free(apb); free(cpd); free(ip);

	int i = 2*length;
	while( (i > 0) && ((*dest)[i] == 0) )
	{
	    i--;
	}

	return i+1;
    }
}
