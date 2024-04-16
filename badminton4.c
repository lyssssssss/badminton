#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    const int PING_PONG_LIMIT = 16;

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Find out rank, size
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Check if the world size is 4
    if (world_size != 4) {
        fprintf(stderr, "World size must be four for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(EXIT_FAILURE);
    }

    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1) % 4;
    while (ping_pong_count < PING_PONG_LIMIT) {
        if (world_rank != ping_pong_count % 4) {
            MPI_Status status;
            MPI_Recv(&ping_pong_count, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            printf("P%d received ping_pong_count %d from P%d\n",
                   world_rank, ping_pong_count, status.MPI_SOURCE);
            }
        } else {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, (world_rank == 0 ? 3 : world_rank - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("P%d received ping_pong_count %d from P%d\n",
                   world_rank, ping_pong_count, (world_rank == 0 ? 3 : world_rank - 1));
        }
    }

    MPI_Finalize();
}
