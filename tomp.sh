cd omp
make
echo "Doing sense barrier"
echo "sense barrier" >> omp_stats.log
for i in 2 3 4 5 6 7 8
do
    echo "Doing $i threads"
    echo "$i $(./mp1 $i)" >> omp_stats.log
done

echo "Doing tournament barrier"
echo "tournament barrier" >> omp_stats.log
for i in 2 3 4 5 6 7 8
do
    echo "Doing $i threads"
    echo "$i $(./mp2 $i)" >> omp_stats.log
done