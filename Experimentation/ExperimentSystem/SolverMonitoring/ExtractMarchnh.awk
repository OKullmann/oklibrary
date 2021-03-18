# Oleg Zaikin, 18.3.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.


# Extracts the numerical data from output of march_nh, in a single line.

BEGIN { rn=0; rc=0; t=0; sat=2; r1=0; r2=0 }
/^c +the DIMACS p-line indicates a/ { rn=$9; rc=$12 }
/^c +time = +[0-9]+.[0-9]+ +seconds/ { t=$4 }
/^s +UNSATISFIABLE *$/ { sat=0 }
/^s +SATISFIABLE *$/ { sat=1 }
/^c +main\(\):: lookAheadCount: [0-9]+/ { r2=$4 }
/^c +main\(\):: unitResolveCount: [0-9]+/ { r1=$4 }
END { print rn " " rc " " t " " sat " " r1 " " r2 }
