# Oleg Zaikin, 17.3.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Version 0.1.0


# Extracts the numerical data from output of the CaDiCaL solver, and prints it in a single line.

BEGIN { t=NA;sat=2;mem=NA }
/^s +UNSATISFIABLE *$/ { sat=0 }
/^s +SATISFIABLE *$/ { sat=1 }
/^c +total real time since initialization: +[0-9]+|[0-9]+.[0-9]+ seconds/ {t=$7}
/^c +maximum resident set size of process:/ {mem=$8}

END { print t " " sat " " mem }
