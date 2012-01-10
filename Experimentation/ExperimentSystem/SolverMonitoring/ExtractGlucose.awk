# Matthew Gwynne, 20.5.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of the glucose solver, in a single line.

BEGIN { 
  rn=0; rc=0; t=0; sat=2; cfs=0; dec=0; rts=0; r1=0; mem=0; ptime=0; cfl=0; rdb=0; ldlc=0; l2c=0;l1c=0; }
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
/^c +nb learnts DL2 +:/ { ldlc=$6 }
/^c +nb learnts size 2 +:/ { l2c=$7 }
/^c +nb learnts size 1 +:/ { l1c=$7 }
/^c +\|  *Pars(e|ing) time:/ { ptime=$5; }
END {
  print rn " " rc " " t " " sat " " cfs " " dec " " rts " " r1 " " mem " " ptime " " cfl " " rdb " " ldlc " " l2c " " l1c; }
