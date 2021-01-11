# Oleg Zaikin, 11.1.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.


# Extracts the numerical data from output of the MapleLCMDistChrBtDLv3 solver, and prints it in a single line.

BEGIN { rn=NA;rc=NA;t=NA;sat=2;ptime=NA;stime=NA;rts=NA;dlc=NA;dlm=NA;cfs=NA;dec=NA;r1=NA;cfl=NA;bt=NA;mem=NA }
/^c +\|  *Number of variables:/ { rn=$6 }
/^c +\|  *Number of clauses:/ { rc=$6 }
/^c +CPU time +: ([0-9]+|[0-9]+.[0-9]+) s/ { t=$5 }
/^s +UNSATISFIABLE *$/ { sat=0 }
/^s +SATISFIABLE *$/ { sat=1 }
/^c +\|  Parse time: +[0-9]+.[0-9]+ +s/ { ptime=$5 }
/^c +\|  Simplification time: +[0-9]+\.[0-9]+ s/ { stime=$5 }
/^c +restarts +:/ { rts=$4 }
/^c +duplicate learnts_cnf +:/ { dlc=$5}
/^c +duplicate learnts_min +:/ {dlm=$5}
/^c +conflicts +:/ { cfs=$4 }
/^c +decisions +:/ { dec=$4 }
/^c +propagations +:/ { r1=$4 }
/^c +conflict literals +:/ { cfl=$5 }
/^c +backtracks +:/ { bt=$4 }
/^c +Memory used +:/ { mem=$5 }

END { print rn " " rc " " t " " sat " " ptime " " stime " " rts " " dlc " " dlm " " \
        cfs " " dec " " r1 " " cfl " " bt " " mem " " }
