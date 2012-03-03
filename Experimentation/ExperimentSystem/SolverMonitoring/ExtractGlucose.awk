# Matthew Gwynne, 20.5.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of the glucose solver, in a single line.

BEGIN { 
  rn="NA"; rc="NA"; t="NA"; sat=2; cfs="NA"; dec="NA"; rts="NA"; r1="NA";
  mem="NA"; ptime="NA"; cfl="NA"; rdb="NA"; rrc="NA"; ldlc="NA"; l2c="NA";
  l1c="NA"; }
/^c +\|  *Number of variables:/ { rn=$6; }
/^c +\|  *Number of clauses:/ { rc=$6; }
/^c +CPU time +: ([0-9]+|[0-9]+.[0-9]+) s/ { t=$5; }
/^s +UNSATISFIABLE *$/ { sat=0; }
/^s +SATISFIABLE *$/ { sat=1; }
/^(INDETERMINATE|*** INTERRUPTED ***) *$/ { sat=2; }
/^c +conflicts +:/ { cfs=$4; }
/^c +decisions +:/ { dec=$4; }
/^c +restarts +:/ { rts=$4; }
/^c +propagations +:/ { r1=$4; }
/^c +Memory used +:/ { mem=$5; }
/^c +conflict literals +:/ { cfl=$5; }
/^c +nb ReduceDB +:/ { rdb=$5 }
/^c +nb nb removed Clauses +:/ { rrc=$5 }
/^c +nb learnts DL2 +:/ { ldlc=$6 }
/^c +nb learnts size 2 +:/ { l2c=$7 }
/^c +nb learnts size 1 +:/ { l1c=$7 }
/^c +\|  *Pars(e|ing) time:/ { ptime=$5; }
END {
  print rn " " rc " " t " " sat " " cfs " " dec " " rts " " r1 " " mem " " ptime " " cfl " " rdb " " rrc " " ldlc " " l2c " " l1c; }
