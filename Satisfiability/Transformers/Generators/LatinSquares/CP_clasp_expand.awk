#! /usr/bin/awk -f

# Expand the output of CP_clasp_first_columns.awk
# to full squares; uses the number of columns for N.
# With mode==0 (default) interpretes the input as the first
# column, while with mode==1 interpretes it as queens-solutions,
# used for digit 0.

# version 0.1.0

{ for (i=1; i<=NF; ++i) {
    x = $i
    if (mode == 1) x = (NF - x) % NF
    for (j=0; j<NF; ++j) {
      y = (x + j) % NF;
      printf " %2s", y
    }
    printf "\n"
  }
  printf "\n"
}
