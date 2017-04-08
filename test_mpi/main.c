#include <stdio.h>
#include <mpi.h>

short const int chunk = 13;

void receiver();

int main(int argc, char **argv) {
    int buf;
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            buf = 0;
            MPI_Send(&buf, 1, MPI_INT, i, chunk * i, MPI_COMM_WORLD);
        }
    } else {
        receiver();
    }
    MPI_Finalize();

    return 0;
}
void sayHello(int from, int processor) {
    printf("Hello from <%d> Processor, came from <%d> processor\n", from, processor);
}
void receiver() {
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int buf;
    MPI_Recv(&buf, 1, MPI_INT, 0, chunk * world_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    sayHello(world_rank, buf);
}