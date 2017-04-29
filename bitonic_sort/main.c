#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <omp.h>

float *readMatrix(char *pathToFile, int *sizeMatrix);

void sort(float *array, int direction, int size, int number, int startPosition);

void printMatrix(float *matrix, int size);

int main(int *argc, char **argv) {
    int *size = malloc(sizeof(int));
    float *matrix = readMatrix(argv[1], size);
    for (int i = 1; i <= log2(*size); ++i) {
        sort(matrix, 1, *size, (int) pow(2, i), 0);
    }
    printMatrix(matrix, *size);
    return 0;
}

void printMatrix(float *matrix, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%f ", matrix[i]);
    }
    printf("\n");
}

float *readMatrix(char *pathToFile, int *sizeMatrix) {
    FILE *file = fopen(pathToFile, "r");
    int size;
    fscanf(file, "%d ", &size);
//    float matrix2[size*size];
    float *matrix = (float *) malloc(size * sizeof(float));
    for (int j = 0; j < size; ++j) {
        float *x = malloc(sizeof(float));
        fscanf(file, "%f", x);
        matrix[j] = *x;
    }
    fclose(file);
    *sizeMatrix = size;
    return matrix;
}

void sort(float *array, int direction, int size, int number, int startPosition) {
//#pragma omp parallel

    if (startPosition < size) {

#pragma omp parallel
        {
#pragma omp for
            for (int i = startPosition; i < startPosition + number / 2; ++i) {
                //sort
                if (direction > 0) {
                    if (array[i] > array[i + number / 2]) {
                        float p = array[i];
                        array[i] = array[i + number / 2];
                        array[i + number / 2] = p;
                    }
                } else {
                    if (array[i] < array[i + number / 2]) {
                        float p = array[i];
                        array[i] = array[i + number / 2];
                        array[i + number / 2] = p;
                    }
                }
            }
        }

            if (number > 2) {

                sort(array, direction, size, number / 2, startPosition);

                sort(array, direction, size, number / 2, startPosition + number / 2);

                }

                sort(array, -direction, size, number, startPosition + number);
    }

}



