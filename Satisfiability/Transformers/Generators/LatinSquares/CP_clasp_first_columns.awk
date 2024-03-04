#! /usr/bin/awk -f

# For clasp-output of satisfying assignments:
#  - combine different v-lines into one
#  - extract the positive literals
#  - interprete them as values of the first column of
#    SAT-instances produced by CPandiagonal.
# Thus for each solution we get the corresponding first column
# of the solution-square (as one row).
# Needs N predefined.

/^v/ {
  for (i=2; i<=NF; ++i)
    if ($i !~ /^-/) {
      if ($i != "0") {
        x=$i - 1
        k=x % N
        printf " %d", k
      }
    }
  if ($NF == "0") printf "\n"
}
