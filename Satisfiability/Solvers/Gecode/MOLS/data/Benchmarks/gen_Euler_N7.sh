#!/bin/bash

# Oleg Zaikin, 7.1.2022 (Swansea)

# Generate benchmarks of the Euler square completion problem of order 8
# bIJCAI22_7_i : 1 <= i <= 1000
# where i is the benchmark index (and seed for LSRG).

N=7
for i in {1..1000};
do
	fname=bIJCAI22'_'$N'_'$i
	LSRG 7,2 "-co" "1*0,0,18;1*0,0,0" 1628808251117432880,1628808263227713690,0,$i | python symmetry-breaking-pair.py > $fname
done
