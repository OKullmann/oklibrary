#!/usr/bin/awk -f

# Replaces the last line, as a disjunction of its literals, by the
# conjunction of its literals (as unit-clauses),
# and outputs the new Dimacs-parameters into file "Dis2Con_dimpars".

BEGIN {line_of_p=-1; num_cl=-1; last_line=-1; last_val=-1}

{
  if ($1 == "p") {
    line_of_p = NR
    var_par = $3
    num_cl = $4
    last_line = line_of_p + num_cl
    print "p cnf", var_par, " ", num_cl
  }
  else if (NR == last_line) {
    last_val = $(NF-1)
    for (i = 1; i < NF; ++i) print $i, "0"
  }
  else if ($1 != "c" && NF != 0) print $0
}

END {
  new_num_cl = num_cl - 1 + last_val
  print var_par, new_num_cl > "Dis2Con_dimpars"
}
