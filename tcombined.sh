#!/bin/bash

cd combined
make
echo "Doing combined"
echo "combined" >> combined_stats.log
for j in $(seq 2 2 8)
do
    for i in $(seq 2 2 12)
    do
        echo "Doing $i threads and $j processes"
        echo "$i $j $(mpiexec -n $j ./combined $i)" >> combined_stats.log
    done
done
