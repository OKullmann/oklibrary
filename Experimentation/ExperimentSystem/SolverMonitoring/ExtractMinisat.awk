# Matthew Gwynne, 20.5.2011 (Swansea)
# Copyright 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of a minisat-like solver, in a single line.

/^\|  *Number of variables:/ { n=$5; }
/^\|  *Number of clauses:/ { c=$5; }
/^CPU time +: ([0-9]+|[0-9]+.[0-9]+) s/ { t=$4; }
/^UNSATISFIABLE *$/ { sat=0; }
/^SATISFIABLE *$/ { sat=1; }
/^(INDETERMINATE|*** INTERRUPTED ***) *$/ { sat=2; }
/^conflicts +:/ { cfs=$3; }
/^decisions +:/ { dec=$3; }
/^restarts +:/ { rts=$3; }
/^propagations +:/ { r1=$3; }
/^Memory used +:/ { mem=$4; }
/^\|  *Pars(e|ing) time:/ { ptime=$4; }
/^\|  *Simplification time:/ { stime=$4; }
/^conflict literals +:/ { cfl=$4; }
END { print n " " c " " t " " sat " " cfs " " dec " " rts " " r1 " " mem " " ptime " " stime " " cfl; }
