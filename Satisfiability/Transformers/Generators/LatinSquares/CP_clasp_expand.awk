#! /usr/bin/awk -f

# Expand the output of CP_clasp_first_columns.awk
# to full squares; needs N predefined.

{ for (i=1; i<=NF; ++i) {
    x = $i
    for (j=0; j<N; ++j) {
      y = (x + j) % N;
      printf " %2s", y
    }
    printf "\n"
  }
  printf "\n"
}
