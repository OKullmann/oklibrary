# Matthew Gwynne, 2.6.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of march_pl, in a single line.

BEGIN { rn=0;rc=0;t=0;sat=2;nds=0;r1=0;r2=0 }
/^c +initFormula\(\):: the DIMACS p-line indicates a/ { rn=$10; rc=$13 }
/^c +main\(\):: time=[0-9\.]+/ { t=substr($3,6) }
/^s +UNSATISFIABLE *$/ { sat=0 }
/^s +SATISFIABLE *$/ { sat=1 }
/^c +main\(\):: nodeCount: [0-9]+/ { nds=$4 }
/^c +main\(\):: lookAheadCount: [0-9]+/ { r2=$4 }
/^c +main\(\):: unitResolveCount: [0-9]+/ { r1=$4 }
END { print rn " " rc " " t " " sat " " nds " " r1 " " r2 }
