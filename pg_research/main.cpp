#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <mpi.h>
#include <algorithm>
#include <time.h>
using namespace std;

void print_vector(const float *vector, const int n);
void print_matrix(const float *matrix, const int n);

float *readMatrix(char *pathToFile, int *sizeMatrix) ;

float multiply_vectors(const float *vec1, const float *vec2, const int n) {
    float result = 0;
    for (int i = 0; i < n; ++i)
        result += vec1[i] * vec2[i];
    return result;
}


void multiply(const float *m, float *v, const int n, const int chunk_size, float *&res) {
    float result[chunk_size];
    for (int i = 0; i < chunk_size; ++i)
        result[i] = multiply_vectors(&m[i * n], v, n);
    memcpy(res, result, sizeof(float) * chunk_size);
}


void initPg(int n, float *&pageranks) {
    std::fill(pageranks, pageranks + n, 1.f / n);
}


void normalizePageranks(float *&pageranks, int n) {
    float sum = 0;
    for (int i = 0; i < n; ++i)sum += pageranks[i];
    for (int j = 0; j < n; ++j)
        pageranks[j] /= sum;

}


void findPagerank(int iterations, float *pageranks, float *matrix, int n,
                  int world_size, float *sub_pageranks, float *sub_matrix, int rank) {
    int chunk_size = n / world_size;
    int left_chunk_size = n % world_size;
    for (int i = 0; i < iterations; ++i) {
        MPI_Bcast(pageranks, n, MPI_FLOAT, 0, MPI_COMM_WORLD);
        multiply(sub_matrix, pageranks, n, chunk_size, sub_pageranks);
        if (rank == 0 && left_chunk_size > 0) {
            float left_pagerank[left_chunk_size];
            for (int chunk = left_chunk_size; chunk > 0; --chunk)
                left_pagerank[left_chunk_size - chunk] = multiply_vectors(&matrix[n * (n - chunk)], pageranks, n);
            for (int j = 0; j < left_chunk_size; ++j)
                pageranks[n - (left_chunk_size - j)] = left_pagerank[j];
        }
        MPI_Gather(sub_pageranks, chunk_size, MPI_FLOAT, pageranks, chunk_size, MPI_FLOAT, 0, MPI_COMM_WORLD);
        if (rank == 0) normalizePageranks(pageranks, n);

    }
}


int main(int argc, char **argv) {
    const int iterations = atoi(argv[2]);
    int rank, world_size, n, chunk_size = 0;
    float *matrix, *pageranks, *sub_matrix, *sub_pageranks = nullptr;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    clock_t begin = clock();
    if (rank == 0) {
        matrix = readMatrix(argv[1], &n);

    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    chunk_size = n / world_size;
    pageranks = new float[n];
    sub_matrix = new float[chunk_size * n];
    sub_pageranks = new float[chunk_size];

    if (rank == 0) initPg(n, pageranks);
    if (rank == 0) MPI_Scatter(matrix, chunk_size * n, MPI_FLOAT, sub_matrix, chunk_size * n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    else MPI_Scatter(sub_matrix, chunk_size * n, MPI_FLOAT, sub_matrix, chunk_size * n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    findPagerank(iterations, pageranks, matrix, n, world_size, sub_pageranks, sub_matrix, rank);
    //if (rank == 0) print_vector(pageranks, n);
    delete[] pageranks, sub_pageranks, matrix, sub_matrix;

    MPI_Finalize();
    if (rank==0) {
        clock_t end = clock();
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("%f\n", time_spent);
    }
    return 0;
}


void print_matrix(const float *matrix, const int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            printf("%8.3f", matrix[i * n + j]);
        printf("\n");
    }
}

void print_vector(const float *vector, const int n) {
    printf("( ");
    for (int i = 0; i < n - 1; ++i)
        printf("%4.6f\t", vector[i]);
    printf("%0.6f )\n", vector[n - 1]);
}


float *readMatrix(char *pathToFile, int *sizeMatrix) {
    FILE *file = fopen(pathToFile, "r");
    int size;
    fscanf(file, "%d ", &size);
//    float matrix2[size*size];
    float *matrix = (float *) malloc(size * size * sizeof(float));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            float *x = (float *) malloc(sizeof(float));
            fscanf(file, "%f", x);
            matrix[i * size + j] = *x;
        }
    }
    fclose(file);
    *sizeMatrix = size;
    return matrix;
}
