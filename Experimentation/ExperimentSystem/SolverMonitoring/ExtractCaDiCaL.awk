# Oleg Zaikin, 17.3.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Version 0.1.2


# Extracts the numerical data from output of the CaDiCaL solver, and prints it in a single line.

BEGIN { rn=0; rc=0; t=NA; sat=2; mem=NA; file="" }
/^s +UNSATISFIABLE *$/ { sat=0 }
/^s +SATISFIABLE *$/ { sat=1 }
/^c +reading DIMACS file from/ { file = $6; gsub(/'/,"",file) }
/^c +found 'p cnf/ { rn=$5; rc=$6; sub(/'/,"",rc) }
/^c +total real time since initialization: +|[0-9]+.[0-9]+ seconds/ { t=$7 }
/^c +maximum resident set size of process:/ { mem=$8 }

END { print rn " " rc " " t " " sat " " mem " " file }
