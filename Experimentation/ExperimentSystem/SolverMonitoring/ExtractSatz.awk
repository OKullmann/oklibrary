# Matthew Gwynne, 3.6.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of satz215, in a single line.

BEGIN {
  rn=0; rc=0; t=0; sat=2; nds=0; r1=0; file=""; bck=0; src=0; fix=0; dc=0;
  src2=0;fix2=0
}

/^satz215 ([^ ]+ )+/ { 
  file=$2
  t=$3
  nds=$4
  bck=$5
  src=$6
  fix=$7
  sat=$8
  rn=$9
  rc=$10
  dc=-$11
  src2=$12
  fix2=$13
}

/^NB_MONO= / {
  pls=$2; sub(/,/, "",pls)
  r1=$4; sub(/,/, "",r1)
}

END { 
  print rn " " rc " " t " " sat " " nds " " r1 " " pls " \"" file "\" " bck " " \
    src " " fix " " dc " " src2 " " fix2
}
