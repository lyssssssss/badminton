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

    // Ensure there are exactly four processes
    if (world_size != 4) {
        fprintf(stderr, "World size must be four for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(EXIT_FAILURE);
    }

    int ping_pong_count = 0;
    int partner_rank = (world_rank % 2 == 0) ? (world_rank + 2) % 4 : (world_rank + 2) % 4;
    int team_mate = (world_rank % 2 == 0) ? (world_rank + 1) % 4 : (world_rank - 1) % 4;

    while (ping_pong_count < PING_PONG_LIMIT) {
        if (world_rank == ping_pong_count % 4) {
            ping_pong_count++;
            // Send to partner on the other team
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            // Notify the teammate
            MPI_Send(&ping_pong_count, 1, MPI_INT, team_mate, 0, MPI_COMM_WORLD);
            printf("P%d sent and incremented ping_pong_count %d to P%d and informed P%d\n",
                   world_rank, ping_pong_count, partner_rank, team_mate);
        } else {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("P%d received ping_pong_count %d from P%d\n",
                   world_rank, ping_pong_count, MPI_ANY_SOURCE);
        }
    }

    MPI_Finalize();
    return 0;
}
