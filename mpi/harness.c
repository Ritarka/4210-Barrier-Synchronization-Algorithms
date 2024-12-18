#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#include "gtmpi.h"

int main(int argc, char **argv)
{
  int num_processes, num_rounds = 100;
  int rank;

  struct timeval t1, t2;
  double time;

  // printf("Inside the harness\n");
  // printf("Harness, argc = %d\n", argc);

  int ret;
  
  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &ret);
  int comm_size;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  // printf("Inside harness MPI_COMM_WORLD = %d\n", comm_size);

  if (argc < 2)
  {
    fprintf(stderr, "Usage: ./harness [NUM_PROCS]\n");
    exit(EXIT_FAILURE);
  }

  num_processes = strtol(argv[1], NULL, 10);

  gtmpi_init(num_processes);

  if (rank == 0)
    gettimeofday(&t1, NULL);

  int k;
  for (k = 0; k < num_rounds; k++)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    gtmpi_barrier();
    // printf("Process passed the barrier: %d\n", rank);
  }



  gtmpi_finalize();

  MPI_Finalize();

  if (rank == 0) {
    gettimeofday(&t2, NULL);

    time = (t2.tv_sec - t1.tv_sec) * 1000.0;
    time += (t2.tv_usec - t1.tv_usec) / 1000.0;

    printf("%lf\n", time);
  }

  return 0;
}