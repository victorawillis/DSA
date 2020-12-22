// Vic Willis - grades merge sort
#include <stdio.h>
#include <stdlib.h>

typedef struct Instructor
{
	float avg_gpa, avg_rating;
	char *name;
} instructor;

instructor *readData(char *fileName, int size);
instructor *mergeSort(instructor *teachers, int leftIndex, int rightIndex);
void merge(instructor *subArray, instructor *leftHalf, int leftCount, instructor *rightHalf, int rightCount);
int is_equal(float, float);
int binarySearch(instructor *teachers, int leftIndex, int rightIndex, float x);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Error: this program requires 3 command line arguments.\n1: a.out\n2: number of teachers in file\n3: the_desired_file.txt\n");
		return -1;
	}

	// Save file name and number of lines in file
	char *fileName = argv[2];
	int size = atoi(argv[1]);
	
	// Return allocated / initiated array of instructors to main
	instructor *list = readData(fileName, size);
	
	// Sort
	mergeSort(list, 0, size-1);
/*
	// test print
	int i;
	for (i=0; i<size; i++)
		printf("%f", list[i].avg_rating);
*/
	// Search
	float x;
	printf("\nEnter a value to search: ");
	scanf("%f", &x);
	while (x <= 0)
	{
		printf("You must enter a positive value: ");
		scanf("%f", &x);
	}
	binarySearch(list, 0, size-1, x);

return 0;
}

instructor *readData(char *fileName, int size)
{
	FILE *fstream = fopen(fileName, "r");
	if (fstream == NULL)
	{
		printf("File could not be opened. Exiting program.\n");
		return NULL;
	}
	
	// Make space to hold "instructor" elements in "list"
	instructor *list = malloc(sizeof(instructor)*size);
	int i;
	for (i=0; i<size; i++)
	{
		instructor temp;
		// Make space to hold name within each instance of instructor
		temp.name = malloc(sizeof(char)*15);
		fscanf(fstream, "%f %f %s\n", &temp.avg_gpa, &temp.avg_rating, temp.name);
		list[i] = temp;
	}

return list;
}

instructor *mergeSort(instructor *teachers, int leftIndex, int rightIndex)
{
	// Find number of elements
	int n = rightIndex+1;

	if (n > 2)
	{
	int mid = (leftIndex + rightIndex)/2;
	instructor *leftHalf = malloc(sizeof(instructor)*mid);
	instructor *rightHalf = malloc(sizeof(instructor)*(n-mid));

	int i;
	for (i=0; i<mid; i++)
		leftHalf[i] = teachers[i];

	for (i=mid; i<n; i++)
		rightHalf[i-mid] = teachers[i];

	// cut left
	mergeSort(teachers, 0, mid);

	// cut right
	mergeSort(teachers, mid+1, n-1);

	// Mid is the no. of elements in the left half, (rightIndex - mid) is
	merge(teachers, leftHalf, mid, rightHalf, n-mid);

	// free
	free(leftHalf);
	free(rightHalf);
	}
	return teachers;
}

void merge(instructor *subArray, instructor *leftHalf, int leftCount, instructor *rightHalf, int rightCount)
{
	int i=0, j=0, k=0;
	while (i<leftCount && j<rightCount)
	{
		if (leftHalf[i].avg_rating < rightHalf[j].avg_rating)
			subArray[k++] = leftHalf[i++];
		else
			subArray[k++] = rightHalf[j++];
	}

	while (i<leftCount)
		subArray[k++] = leftHalf[i++];

	while (j<rightCount)
		subArray[k++] = rightHalf[j++];
}

int is_equal(float f1, float f2)
{
	float precision = 0.01;
	if ((f1 - precision) < f2 && (f1 + precision) > f2)
		return 1;
	else
		return 0;
}

int binarySearch(instructor *teachers, int leftIndex, int rightIndex, float x)
{
	int n = rightIndex+1, first=0, last=rightIndex, mid;
	mid = (n-1)/2;
	while(first <= last)
	{
		if (teachers[mid].avg_rating < x)
		{
			first = mid+1;
		}
		if (teachers[mid].avg_rating == x)
		{
			return mid;
		}
		else
		{
			last = mid-1;
		}

		mid = (first + last)/2;
	}
	
	if (first > last)
	{
		printf("Search not found.\n");
		return -1;
	}
	else
		return 0;
}
