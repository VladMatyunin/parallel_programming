//
// Created by vladislav on 11.04.17.
//
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int *argc, char **argv) {
    FILE *file = fopen(argv[1], "w");
    int size = atoi(argv[2]);
    int max = atoi(argv[3]);
    srand((unsigned int) time(NULL));   // should only be called once
    fprintf(file, "%d\n", size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i==j) {
                fprintf(file,"0 ");
                continue;
            }
            int r = rand() % max;
            if (r == 0) {
                fprintf(file, "%d ", r);
            } else {
                float x = (float) rand() / (float) (RAND_MAX / max);
                fprintf(file, "%0.3f ", x);
                fflush(file);
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

