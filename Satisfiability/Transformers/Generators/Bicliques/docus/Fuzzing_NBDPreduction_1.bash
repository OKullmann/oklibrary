#!/bin/bash

set -o errexit
set -o nounset

script_name=$(basename "$0")
version_number="0.0.5"

seq1="3,7,9,22,66"
seq2="66,22,9,7,3"

for (( i=0; i<1000; ++i )); do
  printf " %d" $i
  BRG "500*100,5" $i | Dimacs2NOBOCONF.awk | ../NBDPreduction_debug $seq1 1 1 | awk '/^C/{next}{print}' > TEMP1
  BRG "500*100,5" $i | Dimacs2NOBOCONF.awk | ../NBDPreduction_debug $seq2 1 1 | awk '/^C/{next}{print}' > TEMP2
  diff TEMP1 TEMP2 > /dev/null
done

rm TEMP1 TEMP2

echo "COMPLETED."
