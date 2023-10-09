#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h> //added
#include "gtmpi.h"

// Global variables
static int num_procs;
static int rank;
static int num_rounds;
static int *send_flags;    // buffers
static int *receive_flags; // buffers
void gtmpi_init(int num_processes)
{
    num_procs = num_processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    num_rounds = (int)ceil((log2(num_procs)));
    send_flags = (int *)malloc(num_rounds * sizeof(int));
    receive_flags = (int *)malloc(num_rounds * sizeof(int));
    for (int i = 0; i < num_rounds; i++)
    {
        send_flags[i] = 1;
        receive_flags[i] = 1;
    }
}

void gtmpi_barrier()
{
    for (int round = 0; round < num_rounds; round++)
    {
        int send_pt = (rank + (1 << round)) % num_procs;                      // get the partner to send to
        int recv_pt = (rank - (1 << round) + num_procs) % num_procs;          // get the node to receive the message from
        MPI_Send(&send_flags[round], 1, MPI_INT, send_pt, 0, MPI_COMM_WORLD); // should 2nd param be 0?
        MPI_Recv(&receive_flags[round], 1, MPI_INT, recv_pt, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void gtmpi_finalize()
{
    return;
}
// main method provided -  in harness.c