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
    int partner_rank = (world_rank + 2) % 4; // Opposite team partner
    int teammate_rank = (world_rank % 2 == 0) ? world_rank + 2 : world_rank - 2;

    while (ping_pong_count < PING_PONG_LIMIT) {
        if (world_rank == ping_pong_count % 4) {
            // Increment and send to the opposite team
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            printf("P%d sent and incremented ping_pong_count %d to P%d\n", world_rank, ping_pong_count, partner_rank);

            // Inform the teammate
            MPI_Send(&ping_pong_count, 1, MPI_INT, teammate_rank, 0, MPI_COMM_WORLD);
            printf("P%d informed teammate P%d about ping_pong_count %d\n", world_rank, teammate_rank, ping_pong_count);
        } else {
            MPI_Status status;
            MPI_Recv(&ping_pong_count, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            printf("P%d received ping_pong_count %d from P%d\n", world_rank, ping_pong_count, status.MPI_SOURCE);

            // Check if the source is the teammate or the opponent
            if (status.MPI_SOURCE == teammate_rank) {
                // Received from teammate, no need to inform again
                printf("P%d acknowledged ping_pong_count %d from teammate P%d\n", world_rank, ping_pong_count, teammate_rank);
            } else {
                // Received from opponent, inform the teammate
                MPI_Send(&ping_pong_count, 1, MPI_INT, teammate_rank, 0, MPI_COMM_WORLD);
                printf("P%d informed teammate P%d about ping_pong_count %d\n", world_rank, teammate_rank, ping_pong_count);
            }
        }
    }

    MPI_Finalize();
    return 0;
}


