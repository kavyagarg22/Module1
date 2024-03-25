#include <chrono>
#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <time.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], int size) {
  for (int i = 0; i < size; i++) {
    vector[i] = rand() % 100;
  }
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int world_rank;
  int world_size;

  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  unsigned long size = 100000000;
  unsigned long local_size = size / world_size;

  srand(time(0) + world_rank); // Different seed for each process

  int *local_v1 = (int *)malloc(local_size * sizeof(int));
  int *local_v2 = (int *)malloc(local_size * sizeof(int));
  int *local_v3 = (int *)malloc(local_size * sizeof(int));

  randomVector(local_v1, local_size);
  randomVector(local_v2, local_size);

  auto start = high_resolution_clock::now();

  // Scatter data to all processes
  MPI_Scatter(local_v1, local_size, MPI_INT, local_v1, local_size, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(local_v2, local_size, MPI_INT, local_v2, local_size, MPI_INT, 0, MPI_COMM_WORLD);

  // Compute local sum
  for (int i = 0; i < local_size; i++) {
    local_v3[i] = local_v1[i] + local_v2[i];
  }

  // Gather results back to root process
  MPI_Gather(local_v3, local_size, MPI_INT, local_v3, local_size, MPI_INT, 0, MPI_COMM_WORLD);

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);

  if (world_rank == 0) {
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
  }

  MPI_Finalize();

  free(local_v1);
  free(local_v2);
  free(local_v3);

  return 0;
}
