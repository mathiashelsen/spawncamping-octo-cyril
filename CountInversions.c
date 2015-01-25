#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define N 100000

uint64_t CountInversions(int *set, int length);

int main(void)
{
    int *numbers = malloc(sizeof(int)*N);
    char strBuf[1024];

    FILE * inputFile = fopen("IntegerArray.txt", "r");
    for(int i = 0; i < N; i++)
    {
	memset(strBuf, 0, 1024);
	fgets(strBuf, 1024, inputFile);
	numbers[i] = atoi(strBuf);
    }

    printf("%" PRIu64 " number of inversions on %d numbers\n", CountInversions(numbers, N), N);
    free(numbers);
    return 0;
}

uint64_t CountInversions(int *set, int length)
{
    if( length == 1 )
    {
	return (uint64_t) 0;
    }
    else if( length == 2)
    {
	if(set[0] < set[1])
	{
	    return (uint64_t) 0;
	}
	else
	{
	    int tmp = set[1];
	    set[1] = set[0];
	    set[0] = tmp;
	    return (uint64_t) 1;
	}
    }
    else
    {
	int half = length/2;
	uint64_t invs = 0;
	int *firstHalf = malloc(sizeof(int)*half);
	int *secondHalf = malloc(sizeof(int)*(length-half));
	memcpy(firstHalf, set, sizeof(int)*half);
	memcpy(secondHalf, set+half, sizeof(int)*(length-half));
	invs += CountInversions(firstHalf, half);
	invs += CountInversions(secondHalf, length-half);

	int j = 0, k = 0;
	for(int i = 0; i < length; i++)
	{
	    if( (j < half) && (k < (length-half)) )
	    {
		if(firstHalf[j] < secondHalf[k])
		{
		    set[i] = firstHalf[j];
		    j++;
		}
		else
		{
		    set[i] = secondHalf[k];
		    k++;
		    invs += (uint64_t) (half-j);
		}
	    }
	    else if( j < half )
	    {
		set[i] = firstHalf[j];
		j++;
	    }
	    else
	    {
		set[i] = secondHalf[k];
		k++;
		invs += (uint64_t) (half-j);
	    }
	}
	free(firstHalf); free(secondHalf);

	return invs;
    }
}
