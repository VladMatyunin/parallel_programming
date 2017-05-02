#include <malloc.h>
#include <stdlib.h>

double * readResult(char* filePath, int n);

void findSpeedUp(double *first, double *second, int n, int proc);

//
// Created by vladislav on 02.05.17.
//
int main(int* argc, char** argv){
    int n = atoi(argv[1]);
    double * resultSimple = readResult(argv[2], n);
    double * resultParallel1 = readResult(argv[3], n);
    double * resultParallel2 = readResult(argv[4], n);
    double * resultParallel3 = readResult(argv[5], n);
    double * resultParallel4 = readResult(argv[6], n);
    findSpeedUp(resultSimple,resultParallel1,n, 2);
    findSpeedUp(resultSimple,resultParallel2,n, 4);
    findSpeedUp(resultSimple,resultParallel3,n, 8);
    findSpeedUp(resultSimple,resultParallel4,n, 10);
    return 0;
}
double * readResult(char* filePath, int n){
    double * result = malloc(sizeof(double)*(n));
    FILE *file = fopen(filePath, "r");
    for (int i = 0; i < n; ++i) {
        double *x = malloc(sizeof(double));
        fscanf(file, "%lf", x);
        result[i] = *x;
        free(x);
    }
    return result;
}
void findSpeedUp(double *first, double *second, int n, int processes){
    for (int i = 0; i < n; ++i) {
        printf("speedUp %lf for process numbers %d\n", first[i]/second[i], processes);
    }
}

