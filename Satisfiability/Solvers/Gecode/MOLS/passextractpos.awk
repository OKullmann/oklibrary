#! /usr/bin/awk -f

# For possibly many satisfying assignments:
#  - combine different v-lines into one (when belonging together)
#  - extract the positive literals
#  - print the partial assignments on one line in sorted order.

# EXAMPLE:

# > echo -e "c dh\nv 5 -3 3 \nv 12 -1 0\nc\nv 55 33 -1 0\nv 0" | ./passextractpos.awk
#  3 5 12 0
#  33 55 0
#  0


# version 0.0.9

/^v/ {
  for (i=2; i<=NF; ++i) {
    x=$i
    if (x !~ /^-/ && x != "0") phi[x] = ""
  }
  if ($NF == "0") {
    for (x in phi) printf " %s", x
    printf " 0\n"
    delete phi
  }
}
