#! /usr/bin/awk -f

# Reads partial assignments and a Dimacs-file F, and adds the
# partial assignment as unit-clauses to F, at the beginning,
# outputting the new F (with corrected c-value in the p-line).
# Stores only the partial assignment (which can be spread over
# several lines, as in CP_clasp_first_columns.awk, and there can also
# be other text in the files; several partial assignments are combined
# into one).
# Comment-lines are transferred also from the partial assignments.
# In the presence of several p-line, always prints the updated
# p-pline and everything after it (except of v-lines, which add to
# the partial assignment, and another p-line).

# Normal example:
# LatinSquares> echo -e "Text\nv -3 4 -5 0\nc COMM\nv -3 -6\nv -7\nc XXX\np cnf 10 2\n-7 9 0\n6 8 0" | ./InsertPassAsUnit.awk
# c COMM
# c XXX
# p cnf 10 8
# -3 0
# 4 0
# -5 0
# -3 0
# -6 0
# -7 0
# -7 9 0
# 6 8 0
# We see that the "0" at the end of a partial assignment is just ignored.

# Showing an example with several p-lines:
# LatinSquares> echo -e "v A B\np cnf n1 3\nv C DDD\np cnf n2 11" | ./InsertPassAsUnit.awk
# p cnf n1 5
# A 0
# B 0
# p cnf n2 15
# A 0
# B 0
# C 0
# DDD 0
# We see that except of the c-value, everything else are just strings.


# version = 0.1.0

BEGIN {
  count = 0
  mode = 0
}

/^c/ { print; next }

/^v/ {
  final_index = $NF=="0" ? NF-1 : NF
  for (i=2; i <= final_index; ++i)
    unit_cls[count++] = $i
  next
}

/^p cnf / {
  n = $3
  c = $4 + count
  printf "p cnf %s %d\n", n, c
  for (i = 0; i < count; ++i)
    printf "%s 0\n", unit_cls[i]
  mode = 1
  next
}

mode == 1 { print }
