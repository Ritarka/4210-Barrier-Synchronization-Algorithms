#!/bin/bash

#SBATCH -J cs6210-proj2-combinedhello
#SBATCH -N 8 --ntasks-per-node=12
#SBATCH --mem-per-cpu=1G
#SBATCH -t 5
#SBATCH -q coc-ice
#SBATCH -o combined_hello.out

echo "Started on `/bin/hostname`"


# cd ~/examples

module load gcc/12.1.0-qgxpzk mvapich2/2.3
# export OMP_NUM_THREADS=8

cd combined
make clean; make
cd ..
./tcombined.sh