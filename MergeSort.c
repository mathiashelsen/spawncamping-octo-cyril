#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100000

void MergeSort(int *set, int length);

int main(void)
{
    int *numbers = malloc(sizeof(int)*N);
    for(int i = 0; i < N; i++)
    {
	numbers[i] = rand() % N;
    }

    MergeSort(numbers, N);
    for(int i = 0; i < N; i++)
	printf("%d ", numbers[i]);
    printf("\n");
    return 0;
}

void MergeSort(int *set, int length)
{
    if( length == 1 )
    {
	return;
    }
    else if( length == 2)
    {
	if(set[0] < set[1])
	{
	    return;
	}
	else
	{
	    int tmp = set[1];
	    set[1] = set[0];
	    set[0] = tmp;
	}
    }
    else
    {
	int half = length/2;
	int *firstHalf = malloc(sizeof(int)*half);
	int *secondHalf = malloc(sizeof(int)*(length-half));
	memcpy(firstHalf, set, sizeof(int)*half);
	memcpy(secondHalf, set+half, sizeof(int)*(length-half));
	MergeSort(firstHalf, half);
	MergeSort(secondHalf, length-half);

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
	    }
	}
	free(firstHalf); free(secondHalf);
    }
}
