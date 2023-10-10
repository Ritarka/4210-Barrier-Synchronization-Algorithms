#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h> //added
#include "../mpi/gtmpi.h"
#include <omp.h>
#include <stdbool.h>
#include "../omp/gtmp.h"
#include "combined.h"

static int rank;
// static int num_proc;
// static int num_thread;

void combined_init(int num_processes, int num_threads)
{
    // num_proc = num_processes;
    // num_thread = num_threads;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    gtmp_init(num_threads);
    gtmpi_init(num_processes);
}

void combined_barrier()
{
    gtmp_barrier(); // sense reversal
    // only call MPI for master thread
    if (omp_get_num_threads() == 0)
    {
        gtmpi_barrier(); // dissemination barrier
    }
    // synchronization within node after MPI
    gtmp_barrier();
}

void combined_finalize()
{
    gtmp_finalize();
    gtmpi_finalize();
}