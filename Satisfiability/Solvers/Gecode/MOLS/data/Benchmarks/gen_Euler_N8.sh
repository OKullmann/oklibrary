#!/bin/bash

# Oleg Zaikin, 6.1.2022 (Swansea)

# Generate benchmarks of the Euler square completion problem of order 8
# bIJCAI22_8_i_j : 1 <= i <= 1000, 0 <= j <= 8.
# where i is the benchmark instance (and seed for LSRG), while j is the
# number of given cells (from the symmetry breaking) in the second square.

N=8
for i in {1..1000};
do
    for j in {0..8};
    do
        fname=bIJCAI22'_'$N'_'$i'_'$j
        #echo $fname
        echo "$N 2" > $fname
        echo "" >> $fname
        LSRG $N "-co" "" 1628808251117432880,1628808263227713690,0,$i >> $fname
        echo "" >> $fname
        for ((k=0;k<$j;k++)); do
            echo -n $k >> $fname
            if [ $k -lt $((N-1)) ]
            then
                echo -n " " >> $fname
            fi
        done
        for ((k=$j;k<$N;k++)); do
            echo -n "*" >> $fname
            if [ $k -lt $((N-1)) ]
            then
                echo -n " " >> $fname
            fi
        done
        echo "" >> $fname
        for k in {1..7};
        do
            echo "* * * * * * * *" >> $fname
        done
    done
done
