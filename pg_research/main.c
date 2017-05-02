#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

float *readMatrix(char *pathToFile, int *sizeMatrix);

void multiplyVectors(float *vector1, float *vector2, int *references, int size);

float *getPageRank(float *matrix, float *pageRank, int size, int* ref);

void returnMatrix(int *matrix, int size);

int *getAllReferences(float *referenceArray, int size);

void normilizePageRank(float*pageRank, int size);

int main(int argc, char **argv) {
    clock_t begin = clock();
/* here, do your time-consuming job */
    float *matrix;
    int size;
    matrix = readMatrix(argv[1], &size);
    int iter = atoi(argv[2]);
    float pageRanks[size];
    for (int i = 0; i < size; ++i) {
        pageRanks[i] = (1.f / size);
    }
    for (int j = 0; j < iter; ++j) {
        getPageRank(matrix, pageRanks, size, getAllReferences(matrix,size));
    }
    free(matrix);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
    return 0;
}

float *readMatrix(char *pathToFile, int *sizeMatrix) {
    FILE *file = fopen(pathToFile, "r");
    int size;
    fscanf(file, "%d ", &size);
//    float matrix2[size*size];
    float *matrix = (float *) malloc(size * size * sizeof(float));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            float *x = malloc(sizeof(float));
            fscanf(file, "%f", x);
            matrix[i * size + j] = *x;
        }
    }
    fclose(file);
    *sizeMatrix = size;
    return matrix;
}

void returnMatrix(int *matrix, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", matrix[i]);
    }
}

void returnFloatMatrix(float *matrix, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%f ", matrix[i]);
    }
}

float *getPageRank(float *matrix, float *pageRank, int size, int* references) {
    float *sum = malloc(size * size * sizeof(float));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            sum[j] = matrix[i * size + j];
        }
        multiplyVectors(sum, pageRank, references, size);
    }
    normilizePageRank(pageRank,size);
    //returnFloatMatrix(pageRank,size);
    free(sum);
}

void multiplyVectors(float *vector1, float *pageRank, int *references, int size) {
    for (int i = 0; i < size; ++i) {
        if (references != 0)
            pageRank[i] = pageRank[i] + vector1[i] * pageRank[i] / references[i];
    }
}
int *getAllReferences(float *referenceArray, int size) {
    int *sum = malloc(size * sizeof(float));
    for (int i = 0; i < size; ++i) {
        int pageReference = 0;
        for (int j = 0; j < size; ++j) {
            if (referenceArray[i * size + j] > 0)
                pageReference++;
        }
        sum[i] = pageReference;
    }
    return sum;
}
void normilizePageRank(float*pageRank, int size){
    for (int i = 0; i < size; ++i) {
        pageRank[i] = pageRank[i]/size;
    }
}