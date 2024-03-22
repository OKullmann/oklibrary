#! /usr/bin/awk -f

# Expand the output of CP_clasp_first_columns.awk
# to Minizinc-assignments with value 0, to be added to the constraints;
# each line of input translated into one line of output;
# uses the number of columns for N.

{ for (i=1; i<=NF; ++i) {
    x = $i
    printf "constraint A[%d,%d]=0;", i-1, x
  }
  printf "\n"
}
