# Matthew Gwynne, 26.4.2013 (Swansea)
# Copyright 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Converts the output of "cryptominisat" to a single line.

BEGIN {
  rn=0; rc=0; n=0; c=0; t=0; sat=2; cfs=0; dec=0; rts=0; mem=0; ptime=0
  file=""; rlc=0; rxc=0; threads=0; drts=0; srts=0; frts=0; stime=0
  ldl2=0; ls2=0; ls1=0; cfl=0; cfld=0; bogp=0; flt=0; vesel=0; set=0
  vex=0; xet=0; xtr2=0; xtr2c=0; x2ft=0; otfci=0; otfsdiff=0; otfws=0
  otfwl=0; upcac=0; cmg=0; x2t=0; x2=0; xnbt=0; xnb=0; dpt=0; dpp=0
  dpun=0; dpn=0; recmc=0
}

/^c +Reading file +'.+/ { file=$4; sub(/'/,"",file)  }
/^c +-- +header says num vars: +[0-9]+/ { rn=$7 }
/^c +-- +header says num clauses: +[0-9]+/ { rc=$7 }
/^c +-- +clauses added: + [0-9]+ learnts, +[0-9]+ normals, + [0-9]+ xors/ { rlc=$5; c=$7; rxc=$9 }
/^c +-- +vars added +[0-9]+/ { n=$5 }
/^c +Parsing time: +[0-9]+.[0-9]+ +s/ { ptime=$4 }
/^c +num threads +: +[0-9]+/ { threads=$5 }
/^c +restarts +: +[0-9]+/ { rts=$4 }
/^c +dynamic restarts +:/ { drts=$5 }
/^c +static restarts +:/ { srts=$5 }
/^c +full restarts  +:/ { frts=$5 }
/^c +total simplify time +: +[0-9]+\.[0-9]+/ { stime=$6 }
/^c +learnts DL2 +: +[0-9]+/ { ldl2=$5 }
/^c +learnts size 2 +: +[0-9]+/ { ls2=$6 }
/^c +learnts size 1 +: +[0-9]+/ { ls1=$6 }
/^c +conflicts +: +[0-9]+/ { cfs=$4 }
/^c +decisions +: +[0-9]+/ { dec=$4 }
/^c +conflict literals +: +[0-9]+/ { cfl=$5; cfld=$6; sub(/\(/,"",cfld) }
/^c +Memory used +: +[0-9]+/ { mem=$5 }
/^c +CPU time +: +[0-9]+\.[0-9]+/ { t=$5 }
/^c +bogo-props +: +[0-9]+/ { bogp=$4 }
/^c +filedLit time +: +[0-9]+\.[0-9]+/ { flt=$5 }
/^c +v-elim SatELite +: +[0-9]+/ { vesel=$5 }
/^c +SatELite time +: +[0-9]+.[0-9]+/ { set=$5 }
/^c +v-elim xor +: +[0-9]+/ { vex=$5 }
/^c +xor elim time +: +[0-9]+\.[0-9]+/ { xet=$6 }
/^c +num binary xor trees +: +[0-9]+/ { xtr2=$7 }
/^c +binxor trees' crown +: +[0-9]+/ { xtr2c=$6 }
/^c +bin xor find time +: +[0-9]+\.[0-9]+/ { x2ft=$7 }
/^c +OTF clause improved +: +[0-9]+/ { otfci=$6 }
/^c +OTF impr\. size diff +: +[0-9]+/ { otfsdiff=$7 }
/^c +OTF cl watch-shrink +: +[0-9]+/ { otfws=$6 }
/^c +OTF cl watch-sh-lit +: +[0-9]+/ { otfwl=$6 }
/^c +updated cache +: +[0-9]+/ { upcac=$5 }
/^c +clauses over max glue +: +[0-9]+/ { cmg=$7 }
/^s +UNSATISFIABLE *$/ { sat=0 }
/^s +SATISFIABLE *$/ { sat=1 }
/^*** INTERRUPTED *** +/ { sat=2 }
/^c +Finding binary XORs  +T: +[0-9]+\.[0-9]+ +s +found: +[0-9]+/ { x2t=$6; x2=$9 }
/^c +Finding non-binary XORs: +[0-9]+\.[0-9]+ +s +\(found: +[0-9]+/ { xnbt=$5; xnb=$8; sub(/,/,"",xnb) }
/^c +Calc default polars +- +time: +[0-9]+\.[0-9]+ s pos: +[0-9]+ +undec: +[0-9]+ +neg: +[0-9]+/ { dpt=$7; dpp=$10; dpun=$12; dpn=$14 }
/^c +tried to recurMin cls +: +[0-9]+/ { recmc=$7 }

END {
  print rn " " rc " " n " " c " " t " " sat " " cfs " " dec " " rts " " mem " " ptime " \"" \
        file "\" " rlc " " rxc " " threads " " drts " " srts " " frts " " stime " " \
        ldl2 " " ls2 " " ls1 " " cfl " " cfld " " bogp " " flt " " vesel " " set " " \
        vex " " xet " " xtr2 " " xtr2c " " x2ft " " otfci " " otfsdiff " " otfws " " \
        otfwl " " upcac " " cmg " " x2t " " x2 " " xnbt " " xnb " " dpt " " dpp " " \
        dpun " " dpn " " recmc
}
