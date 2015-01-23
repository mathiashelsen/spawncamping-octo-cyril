
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This code only illustrates Karabatsu multiplication, it is not meant to be
 * an efficient implementation.
 */

int accumulate(uint32_t *dest, uint32_t *x, int offset, int length);
int accumulatep(uint32_t *dest, int offsetDest, uint32_t *x, int length);
int add(uint32_t **dest, uint32_t *x, int offsetX, int lengthX, uint32_t *y, int offsetY, int lengthY);
int decrease(uint32_t *dest, uint32_t *x, int offset, int length);
int multiply(uint32_t **dest, uint32_t *x, uint32_t *y, int start, int length);


int main(void)
{
    uint32_t x[3] = {10, 20, 0};
    uint32_t y[2] = {20, 0};
    uint32_t *xy;
    multiply(&xy, x, y, 0, 2);
    printf("%d %d %d %d\n", xy[0], xy[1], xy[2], xy[3]);

    uint64_t xp = 10 + ((uint64_t)20 << 32);
    uint64_t yp = 20;
    printf("%" PRIu64 "\n", xp*yp);

    return 0;
}



// Accumulate a value into a destination, i.e. dest += x[offset]
int accumulate(uint32_t *dest, uint32_t *x, int offsetX, int length)
{
    for(int i = 0; i < length; i++)
    {
	// Use 64-bit int for carry and add after casting
	uint64_t tmp = ((uint64_t) dest[i] | ((uint64_t)dest[i + 1] << 32));// + (uint64_t) x[offset + i];
	tmp += (uint64_t) x[offsetX + i];
	// Redivide the 64-bit over the 32-bit int array
	dest[i] = (uint32_t)(tmp & 0xffffffff);
	dest[i + 1] = (uint32_t)(tmp >> 32);
    }   
    int i = 0;
    while( (dest[i] != 0) && (i <= length) )
	i++;

    return i;
}

int accumulatep(uint32_t *dest, int offsetDest, uint32_t *x, int length)
{
    for(int i = 0; i < length; i++)
    {
	// Use 64-bit int for carry and add after casting
	uint64_t tmp = ((uint64_t) dest[i+offsetDest] | ((uint64_t)dest[i + offsetDest + 1] << 32));// + (uint64_t) x[offset + i];
	tmp += (uint64_t) x[i];
	// Redivide the 64-bit over the 32-bit int array
	dest[offsetDest + i] = (uint32_t)(tmp & 0xffffffff);
	dest[offsetDest + i + 1] = (uint32_t)(tmp >> 32);
    }   
    int i = 0;
    while( (dest[offsetDest + i] != 0) && ((offsetDest + i) <= length) )
	i++;

    return i;
}

int add(uint32_t **dest, uint32_t *x, int offsetX, int lengthX, uint32_t *y, int offsetY, int lengthY)
{
    //Prepare destination memory
    int maxLength = (lengthX > lengthY) ? lengthX : lengthY;
    *dest = malloc(sizeof(uint32_t)*(maxLength + 1));
    memset(*dest, 0, sizeof(uint32_t)*(maxLength + 1));

    // Add the two numbers by calling accumulate
    accumulate( *dest, x, offsetX, lengthX );
    return accumulate( *dest, y, offsetY, lengthY );
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


int multiply(uint32_t **dest, uint32_t *x, uint32_t *y, int start, int length)
{
    *dest = malloc(sizeof(uint32_t)*2*length); 
    memset(*dest, 0, sizeof(uint32_t)*2*length);

    if((length) == 1)
    {
	printf("\tFinal call\n");
	uint64_t tmp = (uint64_t)x[start]*(uint64_t)y[start];
	(*dest)[0] = (uint32_t) (tmp & 0xffffffff);
	(*dest)[1] = (uint32_t) (tmp >> 32);
	printf("\tFinal call finished\n");
	if( (*dest)[start+1] == 0 )
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

	printf("x = {");
	for(int i = 0; i < length; i++)
	{
	    printf("%d, ", x[start+i]);
	}
	printf("}\n");

	printf("y = {");
	for(int i = 0; i < length; i++)
	{
	    printf("%d, ", y[start+i]);
	}
	printf("}\n");

	int lengthAC = multiply(&ac, x, y, start, lengthL);		// A*C
	printf("ac = {");
	for(int i  = 0; i < lengthAC; i++)
	    printf("%d, ", ac[i]);
	printf("}\n");


	int lengthBD = multiply(&bd, x, y, start+lengthL, lengthH);	// B*D
	printf("bd = {");
	for(int i  = 0; i < lengthBD; i++)
	    printf("%d, ", bd[i]);
	printf("}\n");

	int lengthAPB = add(&apb, x, start, lengthL, x, start+lengthL, lengthH);    // APB = A + B 
	printf("a+b = {");
	for(int i  = 0; i < lengthAPB; i++)
	    printf("%d, ", apb[i]);
	printf("}\n");


	int lengthCPD = add(&cpd, y, start, lengthL, y, start+lengthL, lengthH);    // CPD = C + D
	printf("c+d = {");
	for(int i  = 0; i < lengthCPD; i++)
	    printf("%d, ", cpd[i]);
	printf("}\n");

	int maxLength = (lengthAPB > lengthCPD) ? lengthAPB : lengthCPD;
	int lengthIP = multiply(&ip, apb, cpd, 0, maxLength);		// IP = APB*CPD
	printf("ip = {");
	for(int i  = 0; i < lengthIP; i++)
	    printf("%d, ", ip[i]);
	printf("}\n");

	decrease(ip, ac, 0, lengthAC);			// IP = IP - AC 
	decrease(ip, bd, 0, lengthBD);			// IP = IP - BD
	printf("(a+b)*(c+d) = {");
	for(int i  = 0; i < lengthIP; i++)
	    printf("%d, ", ip[i]);
	printf("}\n");

	accumulatep(*dest, start, ac, lengthAC);
	accumulatep(*dest, start+lengthL, ip, lengthIP);
	accumulatep(*dest, start+length, bd, lengthBD);

	free(ac); free(bd); free(apb); free(cpd); free(ip);

	int i = 0;
	while( ((*dest)[start + i] != 0) && (i < lengthH*2) )
	{
	    i++;
	}
	return i;
    }
}
