#include <chrono>
#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <time.h>

using namespace std::chrono;

void randomVector(int vector[], int size) {
  for (int i = 0; i < size; i++) {
    vector[i] = rand() % 100;
  }
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  unsigned long total_size = 100000000;
  unsigned long local_size = total_size / size;
  srand(time(0) + rank);

  int *v1 = nullptr, *v2 = nullptr, *v3 = nullptr;
  int *local_v1 = (int *)malloc(local_size * sizeof(int));
  int *local_v2 = (int *)malloc(local_size * sizeof(int));
  int *local_v3 = (int *)malloc(local_size * sizeof(int));

  // Scatter v1 and v2 to all processes
  MPI_Scatter(v1, local_size, MPI_INT, local_v1, local_size, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(v2, local_size, MPI_INT, local_v2, local_size, MPI_INT, 0, MPI_COMM_WORLD);

  auto start = high_resolution_clock::now();

  // Perform local computation
  for (int i = 0; i < local_size; i++) {
    local_v3[i] = local_v1[i] + local_v2[i];
  }

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);

  // Gather local_v3 from all processes to v3 in root process
  MPI_Gather(local_v3, local_size, MPI_INT, v3, local_size, MPI_INT, 0, MPI_COMM_WORLD);

  // Perform reduction to calculate total sum of v3
  unsigned long total_sum = 0;
  unsigned long local_sum = 0;
  for (int i = 0; i < local_size; i++) {
    local_sum += v3[i];
  }
  MPI_Reduce(&local_sum, &total_sum, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
    std::cout << "Total sum of all elements in v3: " << total_sum << std::endl;
  }

  free(local_v1);
  free(local_v2);
  free(local_v3);

  MPI_Finalize();

  return 0;
}
