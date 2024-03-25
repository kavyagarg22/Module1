#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAX_STRING 100

int main(void) {
    char greeting[MAX_STRING];
    int comm_rank, comm_size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    if (comm_rank == 0) {
        // Master process
        strcpy(greeting, "Hello World!");

        // Send the greeting to all worker processes
        for (int i = 1; i < comm_size; i++) {
            MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Worker process
        MPI_Recv(greeting, MAX_STRING, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received: %s\n", comm_rank, greeting);
    }

    MPI_Finalize();
    return 0;
}
