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

static MSCNode *node;
static int rank;
static int num_proc;

void gtmpi_init(int num_processes)
{
    num_proc = num_processes;
    MPI_Comm_Rank(MPI_COMM_WORLD, &rank);
    node = (MCSNode *)malloc(sizeof(MCSNode));
    // Initialize arrival tree
    for (int i = 0; i < 4; i++)
    {
        int childRank = (4 * rank) + i + 1;
        if (childRank < num_proc)
        {
            node->arrChildPt[i] = childRank;
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
        }
        else
        {
            node->wakeupChildPt[i] = -1;
        }
    }
}

void gtmpi_barrier()
{
    unsigned char sense = 1;
    // Synchronization for arrival tree
    for (int i = 0; i < 4; i++)
    {
        if (node->arrChildPt[i] != -1)
        {
            MPI_Recv(&sense, 1, MPI_BYTE, node->arrChildPt[i], 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank != 0)
        {
            MPI_Send(&sense, 1, MPI_BYTE, node->arrParentPt, 1, MPI_COMM_WORLD);
            MPI_Recv(&sense, 1, MPI_BYTE, node->wakeupParentPt, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        // udpate sense
        sense = -sense; // QUESTION- do we need sense variable
        for (int i = 0; i < 2; i++)
        {
            if (node->wakeupChildPt[i] != -1)
            {
                MPI_Send(&sense, 1, MPI_BYTE, node->wakeupChildPt[i], 1, MPI_COMM_WORLD);
            }
        }
    }
}

void gtmpi_finalize()
{
    if (node != NULL)
    {
        free(node);
        node = NULL;
    }
}
