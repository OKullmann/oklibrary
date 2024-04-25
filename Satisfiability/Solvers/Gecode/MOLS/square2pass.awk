#! /usr/bin/awk -f

# Reads a square, determining N as the number of entries on
# the first line, and prints as partial assignment the positive
# literals in the direct encoding (using N^3 variables).

BEGIN {
  row = 0; N = 0;
  printf "v"
}

{ N = NF
  N2 = N*N
  for (i=1; i<=N; ++i) {
    x = $i
    v = row * N2 + (i-1) * N + x + 1
    printf " %d", v
  }
  ++row
}

END {
  printf " 0\n"
}
