#include <openmpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

    int N = 20;
    
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank; 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int A[N], B[N];

    for (int i = 0; i < N; i++) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
    }

    for (int i = 0; i < N; i++) {
        printf("%d ", A[i]);
    }

    printf("\n");
    
    for (int i = 0; i < N; i++) {
        printf("%d ", A[i]);
    }

    int local_chunk = N / size;

    printf("local_size: %d\n", local_chunk);

    int *local_A = (int *)malloc(local_chunk * sizeof(int));
    int *local_B = (int *)malloc(local_chunk * sizeof(int));
    int local_res = 0;

    MPI_Scatter(rank == 0 ? A : NULL, local_chunk, MPI_INT, local_A, local_chunk, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(rank == 0 ? B : NULL, local_chunk, MPI_INT, local_B, local_chunk, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Rank %d: ", rank);
    for (int i = 0; i < local_chunk; i++) {
        printf("%d ", local_A[i]);
    }
    printf("\n");

    for (int i = 0; i < local_chunk; i++) {
        local_res += local_A[i] * local_B[i];
    }

    printf("\nLocal res: %d\n", local_res);

    int final_res = 0;

    if (rank == 0) {
        MPI_Reduce(&local_res, &final_res, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
        printf("\n final res: %d\n", final_res);
    }


    MPI_Finalize();
    return 0;

}