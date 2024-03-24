#! /usr/bin/awk -f

# Counts the values of
# 0 (unsat), 1 (sat), 2 (unknown), other
# in column 5 of an extracted SAT-solver data table.

BEGIN {
  header = 0
  unsat = 0; sat = 0; unknown = 0; other = 0
}

/^#/ { next }

header == 0 { header = 1; next }

{ x = $5
  if (x == 0) ++unsat
  else if (x == 1) ++sat
  else if (x == 2) ++unknown
  else ++other
}

END {
  printf "UNSAT=%d SAT=%d UNKNOWN=%d OTHER=%d\n", unsat, sat, unknown, other
}
