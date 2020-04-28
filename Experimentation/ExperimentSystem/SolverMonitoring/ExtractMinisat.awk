# Matthew Gwynne, 20.5.2011 (Swansea)
# Copyright 2011, 2012, 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of a minisat-like solver, in a single line.

# The fields must be exactly as given by headers/minisat.

BEGIN { rn=NA;rc=NA;t=NA;sat=2;cfs=NA;dec=NA;rts=NA;r1=NA;mem=NA;ptime=NA;stime=NA;cfl=NA }
/^\|  *Number of variables:/ { rn=$5 }
/^\|  *Number of clauses:/ { rc=$5 }
/^CPU time +: ([0-9]+|[0-9]+.[0-9]+) s/ { t=$4 }
/^UNSATISFIABLE *$/ { sat=0 }
/^SATISFIABLE *$/ { sat=1 }
/^conflicts +:/ { cfs=$3 }
/^decisions +:/ { dec=$3 }
/^restarts +:/ { rts=$3 }
/^propagations +:/ { r1=$3 }
/^Memory used +:/ { mem=$4 }
/^\|  *Pars(e|ing) time:/ { ptime=$4 }
/^\|  *Simplification time:/ { stime=$4 }
/^conflict literals +:/ { cfl=$4 }
END { print rn " " rc " " t " " sat " " cfs " " dec " " rts " " r1 " " mem " " \
        ptime " " stime " " cfl }
