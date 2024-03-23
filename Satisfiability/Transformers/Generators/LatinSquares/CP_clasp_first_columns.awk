#! /usr/bin/awk -f

# For clasp-output of (possibly many) satisfying assignments:
#  - combine different v-lines into one (when belonging together)
#  - extract the positive literals
#  - interprete them as values of the first column of
#    SAT-instances produced by CPandiagonal.
# Works indeed for every output of partial assignments mixed with other
# text, if the partial assignments are on lines with a leading "v"
# and have a closing "0" as last entry on a line.
# Thus for each solution we get the corresponding first column
# of the solution-square (as one row).
# Needs N predefined; ignores auxiliary variables (with index
# > N^2).

/^v/ {
  N2=N*N
  for (i=2; i<=NF; ++i)
    if ($i !~ /^-/) {
      if ($i != "0") {
        x=$i - 1
        if (x < N2) {
          k=x % N
          printf " %d", k
        }
      }
    }
  if ($NF == "0") printf "\n"
}
