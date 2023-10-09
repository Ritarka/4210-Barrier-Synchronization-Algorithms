#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gtmp.h"

static bool* pass;
static bool sense = false;
static int counter = 0;
static int threads;

void gtmp_init(int num_threads){
    pass = (bool*)calloc(num_threads, sizeof(bool));
    counter = num_threads;
    threads = num_threads;
}

void gtmp_barrier(){

    int r_counter;
    bool r_sense;
    
    int tid = omp_get_thread_num();
    pass[tid] = !pass[tid];


    #pragma omp atomic update
    counter--;

    #pragma omp atomic read
    r_counter = counter;


    #pragma omp atomic read
    r_sense = sense;
    // printf("Thread %d counter is %d pass[id] is %d, sense is %d\n", tid, r_counter, pass[tid], r_sense);
    
    if (r_counter == 0) {
        #pragma omp atomic write
        counter = threads;

        #pragma omp atomic write
        sense = !sense;
    } else {
        while (pass[tid] != r_sense) {
            #pragma omp atomic read
            r_sense = sense;
        }
    }
}

void gtmp_finalize(){
    free(pass);
}

