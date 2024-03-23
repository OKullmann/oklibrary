#! /usr/bin/awk -f

# Translate the output of CP_clasp_first_columns.awk
# to partial assignments (the queens-placements),
# using the number of columns for N.

{ printf "v"
  for (i=1; i<=NF; ++i) {
    N2=NF*NF
    x = $i
    printf " %s", 1 + (i-1)*N2 + x*NF
  }
  printf " 0\n"
}
