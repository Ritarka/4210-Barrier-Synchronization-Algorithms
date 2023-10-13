#include "../mpi/gtmpi.h"
#include <omp.h>
#include "../omp/gtmp.h"
#include "combined.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <time.h>
#include "../combined/combined.h" // Adjust the path accordingly

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    clock_t tic, toc;


    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &num_processes);
    int num_threads = atoi(argv[1]);

    combined_init(num_processes, num_threads);


    if (rank == 0)
        tic = clock();


    int num_rounds = 50;

#pragma omp parallel num_threads(num_threads)
    {
        // int tid = omp_get_thread_num();
        // printf("Rank %d, Thread %d: Before barrier\n", rank, tid);

        // Your computation or task

        for (int i = 0; i < num_rounds; i++)
            combined_barrier();

        // printf("Rank %d, Thread %d: After barrier\n", rank, tid);
    }




    combined_finalize();

    MPI_Finalize();

    if (rank == 0) {
        toc = clock();
        printf("%f\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    }


    return EXIT_SUCCESS;
}