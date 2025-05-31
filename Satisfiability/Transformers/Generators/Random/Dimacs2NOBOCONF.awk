#! /usr/bin/awk -f

# Simple program to translate DIMACS CNFs into NOBOCONF:
#  - variable numbers are kept (and thus variable 0 becomes formal)
#  - comment-lines become prefixed with "C"
#  - clause-lines are assumed to contain only one clause per line (everthing else
#    after the 0 is copied, but ignored by the NOBOCONF-format).

/^c/ {printf "C%s\n", $0; next}
/^p/ {print "n ", $3+1; print "c ", $4; next}
{ copy = 0
  for (i=1; i<=NF; ++i) {
    if (copy) printf " %s", $i
    else if ($i=="0") {printf "0"; copy=1}
    else if (substr($i,1,1) == "-") printf "%s:0 ", substr($i,2)
    else printf "%s:1 ", $i
  }
  printf "\n"
  next
}
