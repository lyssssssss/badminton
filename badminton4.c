#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const int PING_PONG_LIMIT = 10;

  // Initialize MPI environment
  MPI_Init(NULL, NULL);
  // Get the current process rank and the total number of processes
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Ensure the total number of processes is 4
  if (world_size != 4) {
    fprintf(stderr, "World size must be four for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int ping_pong_count = 0;
  int partner_rank = (world_rank + 1) % 4;
  int team_mate = (world_rank + 2) % 4;  // Find the teammate in the same team

  while (ping_pong_count < PING_PONG_LIMIT) {
    if (world_rank == ping_pong_count % 4) {
      // Increment ping_pong_count before sending it
      ping_pong_count++;
      // Send to the next process
      MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
      printf("P%d sent and incremented ping_pong_count %d to P%d\n",
             world_rank, ping_pong_count, partner_rank);
      // Also inform the teammate
      MPI_Send(&ping_pong_count, 1, MPI_INT, team_mate, 0, MPI_COMM_WORLD);
    } else {
      // Receive the number
      MPI_Recv(&ping_pong_count, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      printf("P%d received ping_pong_count %d from P%d\n",
             world_rank, ping_pong_count, MPI_ANY_SOURCE);
    }
  }

  MPI_Finalize();
  return 0;
}
