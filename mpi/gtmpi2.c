#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h> //added
#include "gtmpi.h"

// Global struct node
typedef struct MCSNode
{
    int arrChildPt[4]; // max children for arrival tree
    int arrParentPt;
    int wakeupParentPt;
    int wakeupChildPt[2]; // max 2- binary tree
} MCSNode;

static MCSNode *node;
static int rank;
static int num_proc;

void gtmpi_init(int num_processes)
{
    num_proc = num_processes;
    // printf("num_processes = %d\n", num_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // printf("Rank in gtmpi init = %d\n", rank);
    node = (MCSNode *)malloc(sizeof(MCSNode));
    // Initialize arrival tree
    for (int i = 0; i < 4; i++)
    {
        int childRank = (4 * rank) + i + 1;
        if (childRank < num_proc)
        {

            node->arrChildPt[i] = childRank;
            // printf("gtmpi_init() Rank %d child %d: %d\n", rank, i, childRank);
        }
        else
        {
            node->arrChildPt[i] = -1;
        }
    }
    if (rank != 0)
    {
        node->arrParentPt = (rank - 1) / 4;
        node->wakeupParentPt = (rank - 1) / 2;
    }
    else
    {
        node->arrParentPt = -1;
        node->wakeupParentPt = -1;
    }
    // initialize the wakeup tree
    for (int i = 0; i < 2; i++)
    {
        int childRank = (2 * rank) + i + 1;
        if (childRank < num_proc)
        {
            node->wakeupChildPt[i] = childRank;
            // printf("gtmpi_init() Rank %d wakeup child %d: %d\n", rank, i, childRank);
        }
        else
        {
            node->wakeupChildPt[i] = -1;
        }
    }
}

void gtmpi_barrier()
{
    // unsigned char sense = 1;
    //  Synchronization for arrival tree
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // printf("Rank entering the barrier %d\n", rank);
    int comm_size;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    // printf("Comm for MPI_COMM_WORLD= %d\n", comm_size);
    for (int i = 0; i < 4; i++)
    {
        if (node->arrChildPt[i] != -1)
        {
            // printf("Rank waiting for child %d\n", node->arrChildPt[i]);

            MPI_Recv(NULL, 0, MPI_INT, node->arrChildPt[i], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    if (rank != 0)
    {
        // printf("Rank sending to parent %d\n", node->arrParentPt);
        MPI_Send(NULL, 0, MPI_INT, node->arrParentPt, 0, MPI_COMM_WORLD);
        // printf("Rank waiting for wakeup parent %d\n", node->wakeupParentPt);
        MPI_Recv(NULL, 0, MPI_INT, node->wakeupParentPt, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // udpate sense
    // sense = -sense; // QUESTION- do we need sense variable
    for (int i = 0; i < 2; i++)
    {
        if (node->wakeupChildPt[i] != -1)
        {
            // printf("Rank sending to wakeup child %d\n", node->wakeupChildPt[i]);
            MPI_Send(NULL, 0, MPI_INT, node->wakeupChildPt[i], 1, MPI_COMM_WORLD);
        }
    }
    // printf("Rank waiting to exit the barrier %d\n", rank);
}

void gtmpi_finalize()
{
    if (node != NULL)
    {
        free(node);
        node = NULL;
    }
}