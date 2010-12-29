#!/bin/bash
# Script which given two DIMACS CNF files A and B as arguments
# returns on STDOUT, file A, followed by B, where the p-line
# of A has been updated to add the number of clauses in B
# and the p-line from B has been removed.
#
# Note that for such a "merge" operation to make sense, the
# variables occuring in file B are assumed to exist in file
# A.

# Grab the number of clauses from each
CL1=`grep "^p" $1  | cut -d " " -f "4"`
CL2=`grep "^p" $2  | cut -d " " -f "4"`

#NewCL=`expr $CL1 + $CL2`
cat $1 $2 | awk 'BEGIN { p_line=0; }
/^p/ && !p_line { print "p " $2 " " $3 " " ('$CL1'+'$CL2'); p_line=1; }
/^[^p]/ { print; }'

