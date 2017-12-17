#include <stdlib.h>
#include <math.h>
#include <stdio.h>  

#define FILE_NAME "matrix.txt"
#define OUTPUT_FILE "output.txt"

int randi()
{
	return rand() % 100 + 1;
}

void populate_matrix()
{
	int row = randi();
	int column = randi();

	FILE *fp = fopen(FILE_NAME, "w");
	fprintf(fp, "%d %d\n", row, column);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0 ; j < column; j++)
		{
			int numberForMatrix = randi();
			if (j <= column - 2)
			{
				fprintf(fp, "%d ", numberForMatrix);
			}
			else if(j == column - 1)
			{
				fprintf(fp, "%d", numberForMatrix);
			}
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

int** process_file(char *nameOfFile) {
	int **arguments = (int **)malloc(3 * sizeof(int*));
	
	int rows = 0, columns = 0;

	FILE *fp = fopen(nameOfFile, "r");
	fscanf(fp, "%d %d\n", &rows, &columns);

	*arguments = (int*)malloc(sizeof(int));
	**arguments = rows;
	*(arguments + 1) = (int*)malloc(sizeof(int));
	**(arguments + 1) = columns;

	int *matrix;

	matrix = (int *)malloc(rows * columns * sizeof(int));
	int j = 0;
	for (int i = 0; i < rows; i++)
	{
		char *string = (char *)malloc(columns * 3 * (columns + 1) * sizeof(char));
		fgets(string, (columns * 3 * (columns + 1) * sizeof(char)), fp);
		
		char *beginningOfString = string;
		char *pointerToString = string;
		int position = 0;
		while (*string)
		{
			if (*string != ' ' && *string != '\n')
			{
				string++;
			}
			else
			{
				int numberFromMatrix = atoi(pointerToString);
				*(matrix + i * columns + position) = numberFromMatrix;
				position++;
				pointerToString = ++string;
				j++;
			}
		}

		free(beginningOfString);
	}
	fclose(fp);

	*(arguments + 2) = matrix;

	return arguments;
}

int sizeOfMatrix(int *matrix)
{
	int rows, columns, size;

	FILE *fp = fopen(FILE_NAME, "r");
	fscanf(fp, "%d %d\n", &rows, &columns);
	fclose(fp);

	size = rows * columns;

	return size;
}

void swap(int *x, int *y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

int partition(int arr[], int low, int high)
{
	int pivot = arr[high];    // pivot
	int i = (low - 1);  // Index of smaller element

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot)
		{
			i++;    // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}


void quickSort(int *array, int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(array, low, high);

		// Separately sort elements before
		// partition and after partition
		quickSort(array, low, pi - 1);
		quickSort(array, pi + 1, high);
	}
}

int calc_mode(int *matrix)
{
	int size = sizeOfMatrix(matrix);
	
	int mode = 0, count = 0, modeCount = 0, previousElement = 0;
	for (int i = 0; i < size; i++)
	{
		int element = matrix[i];
		
		if (i == 0 || element == previousElement)
		{
			count++;

			if (i == size - 1)
			{
				if (count > modeCount)
				{
					modeCount = count;
					mode = previousElement;
				}
			}
		}
		else if (element != previousElement) 
		{
			if (count > modeCount)
			{
				modeCount = count;
				mode = previousElement;
			}
			count = 1;
		}
		previousElement = element;
	}
	return mode;
}

int calc_avg(int *matrix)
{
	int size = sizeOfMatrix(matrix);

	int sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += matrix[i];
	}

	return sum / size;
}

int** find_mmm(int *matrix)
{
	int size = sizeOfMatrix(matrix);

	int **mmm = (int **)malloc(3 * sizeof(int *));
	int *medianPointer = (int *)malloc(sizeof(int));
	int *minPointer = (int *)malloc(sizeof(int));
	int *maxPointer = (int *)malloc(sizeof(int));

	*medianPointer = matrix[(size - 1) / 2];

	int max = 0, min = 0;
	for (int i = 0; i < size; i++)
	{
		if (i == 0)
		{
			max = matrix[i];
			min = matrix[i];
		}
		else
		{
			if (matrix[i] > max)
			{
				max = matrix[i];
			}
			
			if (matrix[i] < min)
			{
				min = matrix[i];
			}
		}
	}
	*minPointer = min;
	*maxPointer = max;

	mmm[0] = medianPointer;
	mmm[1] = minPointer;
	mmm[2] = maxPointer;

	return mmm;
}

void output_results(int rows, int columns, int mode, int average, int min, int max, int median)
{
	FILE *fp = fopen(OUTPUT_FILE, "w");
	fprintf(fp, "rows: %d\n", rows);
	fprintf(fp, "columns: %d\n", columns);
	fprintf(fp, "average: %d\n", average);
	fprintf(fp, "min: %d\n", min);
	fprintf(fp, "max: %d\n", max);
	fprintf(fp, "median: %d\n", median);
	fclose(fp);
}

void destroyDoublePointer(void **ptr)
{
	free(ptr[0]);
	free(ptr[1]);
	free(ptr[2]);

	free(ptr);
}


int main(void)
{
	//give rand() a seed
	srand(time(NULL));

	populate_matrix();
	int **infoFromFile = process_file(FILE_NAME);
	quickSort(infoFromFile[2], 0, sizeOfMatrix(infoFromFile[2]) - 1);
	int mode = calc_mode(infoFromFile[2]);
	int average = calc_avg(infoFromFile[2]);
	int **mmm = find_mmm(infoFromFile[2]);
	output_results(*infoFromFile[0], *infoFromFile[1], mode, average, *mmm[1], *mmm[2], *mmm[0]);
	destroyDoublePointer(infoFromFile);
	destroyDoublePointer(mmm);
	getchar();
	return 0;
}
