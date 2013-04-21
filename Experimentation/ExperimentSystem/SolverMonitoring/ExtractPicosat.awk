# Matthew Gwynne, 20.4.2013 (Swansea)
# Copyright 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Converts the output of "picosat913" to a single line.

BEGIN {
  rn=0; rc=0; t=0; sat=2; cfs=0; dec=0; rts=0; r1=0; mem=0; file=""; its=0;
  simp=0; red=0; ntc=0; fl=0; nfix=0; lln=0; ldel=0; uvar=0; tlib=0;
  mpps=0; mbrec=0;
}
/^c +parsing .+/ { file=$3 }
/^c +parsed header 'p cnf/ { rn=$6; rc=$7; sub(/'/,"",rc) }
/^c +found +[0-9]+ +non trivial clauses/ { ntc=$3 }
/^s +UNSATISFIABLE *$/ { sat=0 }
/^s +SATISFIABLE *$/ { sat=1 }
/^c +[0-9]+ conflicts/ { cfs=$2 }
/^c +[0-9]+ decisions/ { dec=$2; }
/^c +[0-9]+ iterations/ { its=$2 }
/^c +[0-9]+ restarts/ { rts=$2; }
/^c +[0-9]+ failed literals/ { fl=$2 }
/^c +[0-9]+ fixed variables/ { nfix=$2 }
/^c +[0-9]+ learned literals/ { lln=$2 }
/^c +[0-9]+.[0-9]+% deleted literals/ { ldel=$2; sub(/%/,"",ldel) }
/^c +[0-9]+ propagations/ { r1=$2 }
/^c +[0-9]+.[0-9]+% variables used/ { uvar=$2; sub(/%/,"",uvar) }
/^c +[0-9]+.[0-9]+ seconds in library/ { tlib=$2 }
/c +[0-9]+.[0-9]+ megaprops/ { mpps=$2 }
/c +[0-9]+ simplifications/ { simp=$2 }
/c +[0-9]+ reductions/ { red=$2 }
/c +[0-9]+.[0-9]+ MB recycled/ { mbrec=$2 }
/c +[0-9]+.[0-9]+ MB maximally allocated/ { mem=$2 }
/c +[0-9]+.[0-9]+ seconds total run time/ { t=$2 }
END {
  print rn " " rc " " t " " sat " " cfs " " dec " " rts " " r1 " " mem \
      " \"" file "\" " its " " simp " " red " " ntc " " fl " " nfix \
      " " lln " " ldel " " uvar " " tlib " " mpps " " mbrec
}
