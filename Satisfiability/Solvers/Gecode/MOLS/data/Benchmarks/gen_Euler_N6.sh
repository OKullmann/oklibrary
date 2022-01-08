#!/bin/bash

# Oleg Zaikin, 7.1.2022 (Swansea)

# Generate benchmarks of the Euler square completion problem of order 6
# bIJCAI22_6_i : 0 <= i <= 6.
# where i is the number of given cells (from the symmetry breaking)
# in the second square.

N=6
for i in {0..6};
do
    fname=bIJCAI22'_'$N'_'$i
    echo "$N 2" > $fname
    echo "" >> $fname
    echo "0 1 2 3 4 5" >> $fname
    echo "1 * * * * *" >> $fname
    echo "2 * * * * *" >> $fname
    echo "3 * * * * *" >> $fname
    echo "4 * * * * *" >> $fname
    echo "5 * * * * *" >> $fname
    echo "" >> $fname
    for ((k=0;k<$i;k++)); do
        echo -n $k >> $fname
        if [ $k -lt $((N-1)) ]
        then
            echo -n " " >> $fname
        fi
    done
    for ((k=$i;k<$N;k++)); do
        echo -n "*" >> $fname
        if [ $k -lt $((N-1)) ]
        then
            echo -n " " >> $fname
        fi
    done
    echo "" >> $fname
    for k in {1..5};
    do
        echo "* * * * * *" >> $fname
    done
done
