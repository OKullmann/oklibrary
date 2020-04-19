#!/usr/bin/awk -f

# Removes all a-variables.

# Removes all c-/e-/d-/a-lines, maintains the p-line (exactly),
# while the clauses, assuming one clause per line, are kept in order,
# while being sanitised regarding space symobls (no leading or trailing
# spaces, exactly one separating space, no empty lines, with exactly
# literals with declared underlying a-variable are not printed.
# Note that through the deletion of literals, repeated lines
# can be created (which might be handled via the Unix-/Linux-tool
# "uniq").

# Version 0.1.1

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
