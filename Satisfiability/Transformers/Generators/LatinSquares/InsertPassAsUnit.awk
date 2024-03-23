#! /usr/bin/awk -f

# Reads partial assignments and a Dimacs-file F, and adds the
# partial assignment as unit-clauses to F, at the beginning,
# outputting the new F (with corrected c-value in the p-line).
# Stores only the partial assignment (which can be spread over
# several lines, as in CP_clasp_first_columns.awk, and there can also
# be other text in the files; several partial assignments are combined
# into one).

# version = 0.0.1

BEGIN {
  count = 0
}

/^v/ {
  final_index = $NF=="0" ? NF-1 : NF
  for (i=2; i <= final_index; ++i)
    unit_cls[count++] = $i
  next
}

/^p cnf / {
  n = $3
  c = $4 + count
  printf "p cnf %d %d\n", n, c
  for (i = 0; i < count; ++i)
    printf "%s 0\n", unit_cls[i]
  next
}

{ print }
