#!/usr/bin/awk -f

# Merges neighbouring a/e-lines, prints all clauses on seperate lines,
# sanitises all spaces (no leading or trailing space-symbols, exactly
# one space for separation); removes all comments.

# Version 0.1.0

BEGIN { prev_a=0; prev_e=0; clauses=0 }

{
  if (prev_a == 1) {
    if ($1 == "c") {}
    else if ($1 == "a") {
      for (i=2; i < NF; ++i) printf "%d ", $i
    }
    else if ($1 == "e") {
      print "0"
      prev_a = 0
      prev_e = 1
      printf "e "
      for (i=2; i < NF; ++i) printf "%d ", $i
    }
    else if ($1 == "d") {
      print "0"
      prev_a = 0
      printf "d "
      for (i=2; i < NF; ++i) printf "%d ", $i
      print "0"
    }
    else if ($1 == "p") {
      print "ERROR[Sanitiser]: p-line after a-line!" > "/dev/stderr"
    }
    else {
      print "0"
      prev_a = 0
      clauses = 1
      for (i=1; i <= NF; ++i) {
        if ($i == 0) print "0"
        else printf "%d ", $i
      }
    }
  }

  else if (prev_e == 1) {
    if ($1 == "c") {}
    else if ($1 == "e") {
      for (i=2; i < NF; ++i) printf "%d ", $i
    }
    else if ($1 == "a") {
      print "0"
      prev_e = 0
      prev_a = 1
      printf "a "
      for (i=2; i < NF; ++i) printf "%d ", $i
    }
    else if ($1 == "d") {
      print "0"
      prev_e = 0
      printf "d "
      for (i=2; i < NF; ++i) printf "%d ", $i
      print "0"
    }
    else if ($1 == "p") {
      print "ERROR[Sanitiser]: p-line after e-line!" > "/dev/stderr"
    }
    else {
      print "0"
      prev_e = 0
      clauses = 1
      for (i=1; i <= NF; ++i) {
        if ($i == 0) print "0"
        else printf "%d ", $i
      }
    }
  }

  else if ($1 == "c") {}

  else if ($1 == "p") { print "p cnf", $3, $4 }

  else if ($1 == "a") {
    printf "a "
    for (i=2; i < NF; ++i) printf "%d ", $i
    prev_a = 1
  }

  else if ($1 == "e") {
    printf "e "
    for (i=2; i < NF; ++i) printf "%d ", $i
    prev_e = 1
  }

  else if ($1 == "d") {
    printf "d "
    for (i=2; i < NF; ++i) printf "%d ", $i
    print "0"
  }

  else {
    clauses = 1
    for (i=1; i <= NF; ++i) {
      if ($i == 0) print "0"
      else printf "%d ", $i
    }
  }

}

END {

  if (clauses == 0 && (prev_a == 1 || prev_e == 1))
    print "0"

}
