#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
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


  struct timeval t1, t2;
  double time;

  gettimeofday(&t1, NULL);

#pragma omp parallel shared(num_threads)
   {
     int i;
     for(i = 0; i < num_iter; i++){
       gtmp_barrier();
     }
   }

  gettimeofday(&t2, NULL);

  time = (t2.tv_sec - t1.tv_sec) * 1000.0;
  time += (t2.tv_usec - t1.tv_usec) / 1000.0;
  printf("%lf\n", time);


  gtmp_finalize();

  return 0;
}
