#! /usr/bin/awk -f

# Expand the output of CP_clasp_first_columns.awk
# to toroidal N-queens solutions, with 0 for the queen,
# and * elsewhere; uses the number of columns for N.

{ for (i=1; i<=NF; ++i) {
    x = $i
    for (j=0; j<NF; ++j)
      printf " %2s", (j==x ? "0" : "*")
    printf "\n"
  }
  printf "\n"
}
