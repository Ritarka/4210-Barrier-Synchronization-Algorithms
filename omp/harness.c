#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "gtmp.h"

int main(int argc, char** argv)
{
  int num_threads, num_iter=100;

  if (argc < 2){
    fprintf(stderr, "Usage: ./harness [NUM_THREADS]\n");
    exit(EXIT_FAILURE);
  }
  num_threads = strtol(argv[1], NULL, 10);

  omp_set_dynamic(0);
  if (omp_get_dynamic())
    printf("Warning: dynamic adjustment of threads has been set\n");

  omp_set_num_threads(num_threads);
  
  gtmp_init(num_threads);

  clock_t tic = clock();

#pragma omp parallel shared(num_threads)
   {
     int i;
     for(i = 0; i < num_iter; i++){
       gtmp_barrier();
     }
   }

  clock_t toc = clock();

  printf("%f\n", (double)(toc - tic) / CLOCKS_PER_SEC);

   gtmp_finalize();

   return 0;
}
