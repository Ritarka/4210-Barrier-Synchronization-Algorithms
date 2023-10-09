#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gtmp.h"

typedef struct {
    bool pass;
    int children;
} node;

node* nodes;

int threads;

void gtmp_init(int num_threads){
    nodes = (node*)calloc(num_threads+1, sizeof(node));
    threads = num_threads;
}

void gtmp_barrier(){
    int tid = omp_get_thread_num() + 1;

    if (tid != 1)
        nodes[tid].pass = false;

    // printf("Thread %d is here\n", tid);

    // if (tid << 1 > threads)

    int num_children;
    if (tid << 1 <= threads) {
        num_children = (tid << 1 == threads) ? 1 : 2;
    } else {
        num_children = 0;
    }

    // printf("Thread %d Num children %d\n", tid, num_children);



    int child;
    do { // spin
        #pragma omp atomic read
        child = nodes[tid].children;
        // // printf("Thread %d child %d\n", tid, child);

    } while (child != num_children);

    // printf("Thread %d is after 1st spin\n", tid);

    
    if (tid != 1) {
        #pragma omp atomic update
        nodes[tid>>1].children++;

        bool pass;
        do { // spin
            // // printf("Thread %d child: %d pass: %d\n", tid, child, pass);
            #pragma omp atomic read
            child = nodes[tid].children;
            #pragma omp atomic read
            pass = nodes[tid].pass;
        } while (pass != true);
    }

    // printf("Thread %d is post\n", tid);

    if (tid == 1) {
        #pragma omp atomic write
        nodes[1].children = 0;
    }


    if (tid << 1 <= threads) {
        #pragma omp atomic write
        nodes[tid<<1].children = 0;
        #pragma omp atomic write
        nodes[tid<<1].pass = true;
    }

    if ((tid << 1)+1 <= threads) {
        #pragma omp atomic write
        nodes[(tid<<1)+1].children = 0;
        #pragma omp atomic write
        nodes[(tid<<1)+1].pass = true;
    }

    // spin
}

void gtmp_finalize(){

}

