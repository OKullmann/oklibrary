#!/bin/bash

# Oleg Zaikin, 7.1.2022 (Swansea)

# Generate benchmarks of the Euler square completion problem of order 8
# bIJCAI22_7_i : 1 <= i <= 1000
# where i is the benchmark index (and seed for LSRG).

# BUGS:
# 0. Follow the standard for bash-scripts:
#  - see the older bash-scripts.
# 1. By default the output must go to standard output:
#  - here that could be achieved by using the case that no
#    command-line input is given;
#  - but it's safer to ask for one special input..
# 2. The above text needs update.

N=7
for i in {1..1000};
do
	fname=bIJCAI22'_'$N'_'$i
	LSRG 7,2 "-co" "1*0,0,18;1*0,0,0" 1628808251117432880,1628808263227713690,0,$i | python symmetry-breaking-pair.py > $fname
done
