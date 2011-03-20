# Matthew Gwynne, 4.1.2011 (Swansea)
# Copyright 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Awk script which, given a stream on STDIN containing a standard DIMACS file,
# returns a weighted partial MaxSAT file where all original
# clauses are hard clauses, and additional (negated) unit clauses with weight 
# 1 are added for every variable in the input.
#
# In such a way, this script yields a weighted partial MaxSAT Dimacs file
# which has a maximum weight assignment phi when phi is a satisfying
# assignment for the original clause-set with the minimum number of
# variables set to true.

BEGIN    { VARS=0; CLAUSES=0 }
/^p/     { VARS=$3; CLAUSES=$4; print "p wcnf " $3 " " ($3+$4) }
/^[^cp]/ { print (VARS + 1) " " $0 }
/^c/     { print }
END      { for (i = 1; i <= VARS; i++) print "1 " "-" i " 0" }
