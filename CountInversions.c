#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10

int CountInversions(int *set, int length);

int main(void)
{
    int numbers[10] = {10, 2, 3, 4, 7, 6, 5, 8, 9, 1};
    /*
    for(int i = 0; i < N; i++)
    {
	numbers[i] = rand() % N;
    }
    */


    printf("%d number of inversions on %d numbers\n", CountInversions(numbers, N), N);
    return 0;
}

int CountInversions(int *set, int length)
{
    if( length == 1 )
    {
	return 0;
    }
    else if( length == 2)
    {
	if(set[0] < set[1])
	{
	    return 0;
	}
	else
	{
	    int tmp = set[1];
	    set[1] = set[0];
	    set[0] = tmp;
	    return 1;
	}
    }
    else
    {
	int half = length/2, invs = 0;
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
		    invs += (half-j);
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
		invs += (half-j);
	    }
	}
	free(firstHalf); free(secondHalf);

	return invs;
    }
}
