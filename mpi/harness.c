#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "gtmpi.h"

int main(int argc, char **argv)
{
  int num_processes, num_rounds = 1;
  int rank;
  printf("Inside the harness\n");
  printf("Harness, argc = %d\n", argc);
  MPI_Init(&argc, &argv);
  int comm_size;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  printf("Inside harness MPI_COMM_WORLD = %d\n", comm_size);

  if (argc < 2)
  {
    fprintf(stderr, "Usage: ./harness [NUM_PROCS]\n");
    exit(EXIT_FAILURE);
  }

  num_processes = strtol(argv[1], NULL, 10);

  gtmpi_init(num_processes);

  int k;
  for (k = 0; k < num_rounds; k++)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    gtmpi_barrier();
    printf("Process passed the barrier: %d\n", rank);
  }

  gtmpi_finalize();

  MPI_Finalize();

  return 0;
}