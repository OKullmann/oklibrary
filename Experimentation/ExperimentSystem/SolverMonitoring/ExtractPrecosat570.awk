# Matthew Gwynne, 2.6.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Converts the output of "precosat-570.1 -v to a single line.

BEGIN {
  rn="NA"; rc="NA"; t="NA"; sat="NA"; cfs="NA"; dec="NA"; rts="NA"; r1="NA";
  pls="NA"; ats="NA"; mem="NA"; ptime="NA"; file="NA"; rnd="NA"; its="NA";
  skip="NA"; enl="NA"; shk="NA"; resc="NA"; rebi="NA"; simp="NA"; red="NA";
  ar="NA"; arx="NA"; atssz="NA"; bck="NA"; bckct="NA"; bckj="NA"; bckjln="NA";
  blkres="NA"; blkph="NA"; blkr="NA"; blk="NA"; blkimp="NA"; blkexp="NA";
  clsrec="NA"; clspur="NA"; clsaut="NA"; dom="NA"; domh="NA"; domlow="NA";
  elres="NA"; elph="NA"; elr="NA"; extf="NA"; extass="NA"; extfl="NA"; glu="NA";
  glusl="NA"; minln="NA"; mindel="NA"; minst="NA"; mininv="NA"; mind="NA";
  otfsd="NA"; otfsd2="NA"; otfsd3="NA"; otfsdlg="NA"; otfss="NA"; otfss2="NA";
  otfss3="NA"; otfsslg="NA"; pb="NA"; pbph="NA"; pbr="NA"; pbf="NA"; pblf="NA";
  pbmg="NA"; mpr="NA"; plexp="NA"; plel="NA"; plblk="NA"; plaut="NA"; sb="NA";
  sbn="NA"; sba="NA"; sbx="NA"; sbi="NA"; sccnt="NA"; sccf="NA"; sccm="NA";
  strf="NA"; strb="NA"; strdyn="NA"; strorg="NA"; strasy="NA"; subf="NA";
  subb="NA"; subdyn="NA"; suborg="NA"; subdm="NA"; subgc="NA"; srtime="NA";
  otime="NA"; nfix="NA"; neq="NA"; nel="NA"; npur="NA"; nzmb="NA"; naut="NA";
  zmb="NA"; zmbexp="NA"; zmbel="NA"; zmbblk="NA"; memr="NA"; }
/^c +found header 'p cnf/ { rn=$6; rc=$7; sub(/'/,"",rc); }
/^c +[0-9]+.[0-9]+ seconds, [0-9]+ MB max,/ { t=$2; mem=$4; memr=$7; }
/^s +UNSATISFIABLE *$/ { sat=0; }
/^s +SATISFIABLE *$/ { sat=1; }
/^\(*** CAUGHT SIGNAL +/ { sat=2; }
/^c +reading .+/ { file="\"" $3 "\""; }
/^c +[0-9]+ conflicts, [0-9]+ decisions,/ { cfs=$2; dec=$4; rnd=$6; }
/^c +[0-9]+ iterations, [0-9]+ restarts,/ { its=$2; rts=$4; skip=$6; }
/^c +[0-9]+ enlarged, [0-9]+ shrunken,/ { enl=$2; shk=$4; resc=$6; rebi=$8; }
/^c +[0-9]+ simplifications, [0-9]+ reductions/ { simp=$2; red=$4; }
/^c +arty: [0-9\.]+ ands [0-9\.]+ xors average arity/ { ar=$3; arx=$5; }
/^c +autk: [0-9]+ autarkies of [0-9\.]+ avg size/ { ats=$3; atssz=$6; }
/^c +back: [0-9]+ track with / { bck=$3; bckct=$6; bckj=$9; bckjln=$12; }
/^c +blkd: [0-9]+ resolutions,/ { blkres=$3; blkph=$5; blkr=$7; }
/^c +blkd: [0-9]+ = [0-9]+ implicit \+/ { blk=$3; blkimp=$5; blkexp=$8; }
/^c +clss: [0-9]+ recycled,/ { clsrec=$3; clspur=$5; clsaut=$7; }
/^c +doms: [0-9]+ dominators,/ { dom=$3; domh=$5; domlow=$7; }
/^c +elim: [0-9]+ resolutions,/ { elres=$3; elph=$5; elr=$7; }
/^c +extd: [0-9]+ forced, [0-9]+/ { extf=$3; extass=$5; extfl=$7; }
/^c +glue: [0-9\.]+ original glue,/ { glu=$3; glusl=$6; }
/^c +prbe: [0-9]+ probed, [0-9]+ / { pb=$3; pbph=$5; pbr=$7; }
/^c +prbe: [0-9]+ failed, [0-9]+ / { pbf=$3; pblf=$5; pbmg=$7; }
/^c +prps: [0-9]+ srch props, / { r1=$3; mpr=$6; }
/^c +mins: [0-9]+ lrnd, [0-9]+%/ { 
  minln=$3; mindel=$5; sub(/%/,"",mindel); minst=$7; mininv=$9; mind=$11; }
/^c +otfs: dynamic [0-9]+ =/ { otfsd=$4; otfsd2=$6; otfsd3=$9; otfsdlg=$12; }
/^c +otfs: static [0-9]+ =/ { otfss=$4; otfss2=$6; otfss3=$9; otfsslg=$12; }
/^c +pure: [0-9]+ =/ { pls=$3; plexp=$5; plel=$8; plblk=$11; plaut=$14; }
/c +sbst: [0-9\.]+% subst,/ {
  sb=$3; sub(/%/,"",sb); sbn=$5; sub(/%/,"",sbn); sba=$7; sub(/%/,"",sba);
  sbx=$9; sub(/%/,"",sbx); sbi=$11; sub(/%/,"",sbi); }
/^c +sccs: [0-9]+ non trivial, [0-9]+/ { sccnt=$3; sccf=$6; sccm=$8; }
/^c +strs: [0-9]+ for/ { strf=$3; strb=$5; strdyn=$7; strorg=$9; strasy=$11; }
/^c +subs: [0-9]+ fw, [0-9]+ bw,/ {
  subf=$3; subb=$5; subdyn=$7; suborg=$9; subdm=$11; subgc=$13; } 
/^c +time: [0-9\.]+ = [0-9\.]+/ { srtime=$5; ptime=$9; otime=$13; }
/^c +vars: [0-9]+ fxd, [0-9]+ eq,/ {
  nfix=$3; neq=$5; nel=$7; npur=$9; nzmb=$11; naut=$13; }
/^c zmbs: 0 = 0 explicit + 0 elim + 0 blkd + 0 autark/ { zmb=$3; zmbexp=$5; zmbel=$8; zmbblk=$11; }
END {
  print rn " " rc " " t " " sat " " cfs " " dec " " rts " " r1 " " pls " " \
    ats " " mem " " ptime " " file " " rnd " " its " " skip " " enl " " \
    shk " " resc " " rebi " " simp " " red " "  ar " " arx " " atssz " " \
    bck " " bckct " " bckj " " bckjln " " blkres " " blkph " " blkr " " \
    blk " " blkimp " " blkexp " " clsrec " " clspur " " clsaut " " dom " " \
    domh " " domlow " " elres " " elph " " elr " " extf " " extass " " \
    extfl " " glu " " glusl " " minln " " mindel " " minst " " mininv " " \
    mind " " otfsd " " otfsd2 " " otfsd3 " " otfsdlg " " otfss " " otfss2 " " \
    otfss3 " " otfsslg " " pb " " pbph " " pbr " " pbf " " pblf " " pbmg " " \
    mpr " " plexp " " plel " " plblk " " plaut " " sb " " sbn " " sba " " \
    sbx " " sbi " " sccnt " " sccf " " sccm " " strf " " strb " " strdyn " " \
    strorg " " strasy " " subf " " subb " " subdyn " " suborg " " subdm " " \
    subgc " " srtime " " otime " " nfix " " neq " " nel " " npur " " nzmb " " \
    naut " " zmb " " zmbexp " " zmbel " " zmbblk " " memr; }
