#! /usr/bin/awk -f

# Expand the output of CP_clasp_first_columns.awk
# to full squares; uses the number of columns for N.

{ for (i=1; i<=NF; ++i) {
    x = $i
    for (j=0; j<NF; ++j) {
      y = (x + j) % NF;
      printf " %2s", y
    }
    printf "\n"
  }
  printf "\n"
}
