# Matthew Gwynne, 3.6.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of satz215, in a single line.

BEGIN {
  rn=0; rc=0; t=0; sat=2; nds=0; r1=0; file=""; bnds=0; r2la=0
  r2=0; prpr=0; r3la=0; r3=0
}

/^satz215 ([^ ]+ )+/ { 
  file=$2
  t=$3
  nds=$4
  bnds=$5
  r2la=$6
  r2=$7
  sat=$8
  rn=$9
  rc=$10
  prpr=$11 # preprocessing: number of added resolvents of lengths 2,3 minus number of clauses eliminated by subsumption
  r3la=$12
  r3=$13
}

/^NB_MONO= / {
  pls=$2; sub(/,/, "",pls)
  r1=$4; sub(/,/, "",r1)
}

END { 
  print rn " " rc " " t " " sat " " nds " " r1 " " r2 " " pls " \"" file "\" " bnds " " \
    r2la " " r3 " " r3la " " prpr
}
