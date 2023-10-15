#!/bin/bash

cd mpi
make
echo "Doing dissemenation barrier"
echo "dissemenation barrier" >> mpi_stats.log
for i in $(seq 2 12)
do
    echo "Doing $i nodes"
    echo "$i $(mpiexec -n $i ./mpi1 $i)" >> mpi_stats.log
done

echo "Doing mcs tree"
echo "mcs tree" >> mpi_stats.log
for i in $(seq 2 12)
do
    echo "Doing $i nodes"
    echo "$i $(mpiexec -n $i ./mpi2 $i)" >> mpi_stats.log
done