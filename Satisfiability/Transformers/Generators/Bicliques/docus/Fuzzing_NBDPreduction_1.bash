#!/bin/bash

set -o errexit
set -o nounset

script_name=$(basename "$0")
version_number="0.0.10"

program="${program-../NBDPreduction_debug}"
clauseset="${clauseset-1}"
nval=${nval-500}

seq1="3,7,9,22,66"
seq2="66,22,9,7,3"

for (( i=0; i<1000; ++i )); do
  printf "%d:" $i
  BRG "$nval*100,5" "" $i | Dimacs2NOBOCONF.awk | $program $seq1 $clauseset 1 | awk '/^C/{next}{print}' > TEMP1
  c=$(cat TEMP1 | awk '/^c/{print $2; exit(0)}')
  printf "%d " $c
  BRG "$nval*100,5" "" $i | Dimacs2NOBOCONF.awk | $program $seq2 $clauseset 1 | awk '/^C/{next}{print}' > TEMP2
  diff TEMP1 TEMP2 > /dev/null
done

rm TEMP1 TEMP2

echo "COMPLETED."
