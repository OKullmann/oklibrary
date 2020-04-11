#!/usr/bin/awk -f

# Removes all a-variables.

function abs(x) {return x < 0 ? -x : x}

{
  if ($1 == "e") {}
  else if ($1 == "d") {}
  else if ($1 == "a") {
    for (i = 2; i < NF; ++i) avar[$i]
  }
  else if ($1 == "p") print $0
  else if ($1 != "c" && NF != 0) {
    for (i = 1; i < NF; ++i) {
      if (! (abs($i) in avar)) printf "%d ", $i
    }
    print "0"
  }
}
