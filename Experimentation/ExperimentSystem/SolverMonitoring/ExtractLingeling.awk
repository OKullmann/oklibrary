# Matthew Gwynne, 1.5.2013 (Swansea)
# Copyright 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Converts the output of "lingelingala-b02aa1a-121013 -v" to a single line.

BEGIN {
  rn=0; rc=0; t=0; sat=2; cfs=0; dec=0; rts=0; r1=0; pls=0; mem=0; ptime=0
  file=""; n=0; c=0; l=0; bce=0; bcerm=0; bcers=0; bcest=0; blkl=0; cce=0;
  cceel=0; cceat=0; cceab=0; ccepr=0; ccelf=0; ccefd=0; clf=0; clflf=0;
  clffd=0; clfdec=0; clfst=0; cllsat=0; cllsim=0; cllfre=0; cllmel=0;
  cllad=0; cllass=0; clldrf=0; cllfd=0; cllcas=0; cllmos=0; colgcs=0; colrec=0
  cgcnt=0; cgest=0; cgcst=0; cgeqs=0; cgrs1=0; cgmat=0; cgmand=0; cgmxor=0;
  cgmite=0; cgsim=0; cgsand=0; cgsxor=0; cgsite=0; cgex=0;cgeand=0; cgexor=0;
  cgeite=0; dcpdcp=0; dcpeq=0; decrd=0; decflp=0; decphs=0; elim=0; elitd=0;
  elsm=0; ellg=0; eltsm=0; elsuc=0; elfd=0; elsub=0; elstr=0; elblk=0;
  elcop=0; elres=0; elips=0; elsch=0; elstc=0; frcmp=0; gsex=0;
  gsexd=0; gssz=0; gsmx=0; gsex1=0; gsex2=0; gsex3=0; gsgc=0; gsst=0; gsexr=0
  gsel=0; glav=0; glsav=0; glue=0; glmrg=0; lfph=0; lfpr=0; lflv1=0; lfl2=0
  lf1=0; lfeq=0; lfimp=0; hbrs=0; hbrtr=0; hbrlrg=0; hbrsub=0; lncl=0; lnuip=0
  lnlen=0; lnglu=0; intlb=0; intlbs=0; intio=0; intios=0; mnll=0; mnllm=0
  mvd=0; mvd2=0; mvd3=0; otstdy=0; otstrd=0; otstir=0; otstdr=0; otstrs=0
  otsbdy=0; otsbrd=0; otsbir=0; phcmp=0; phset=0; phpos=0; phneg=0; prbbs=0
  prbpr=0; prbfd=0; prblf=0; prppr=0; prpsr=0; prpsm=0; prplk=0; psns=0
  psnsh=0; qunw=0; qudl=0; qump=0; qumg=0; qucl=0; qugc=0; qudp=0; qula=0
  rdcnt=0; rdrst=0; rdact=0; rdexp=0; rdar=0; rddbl=0; rmbrm=0; rmbrd=0
  rtssk=0; rtskp=0; rtsav=0; smcnt=0; smilm=0; smplm=0; smclm=0; trncn=0
  trn2=0; trn3=0; trnst=0; tpfix=0; tpit=0; trdtr=0; trdrm=0; trdfd=0
  trdnd=0; trded=0; trdst=0; uhcnt=0; uhrnd=0; uhstp=0; uhscc=0; uhsccs=0
  uh1=0; uh2=0; uh3=0; uhlg=0; uhfld=0; uhfst=0; uhfd=0; uhf2=0; uhf3=0
  uhflg=0; uht=0; uht2=0; uht3=0; uht3=0; uhtlg=0; uhtst=0; uhtrd=0; uhhb=0
  uhhb3=0; uhhbl=0; uhhbr=0; uhst=0; uhst2=0; uhst3=0; uhstl=0; uhstr=0
  nrem=0; nrmv=0; nfix=0; nelm=0; neq=0; vst=0; vstsr=0; vstsm=0; vstlk=0
  wchp=0; wchen=0; wchdf=0; tanl=0; tblk=0; tbmp=0; tcrd=0; tcce=0
  tclf=0; tcwi=0; tdec=0; tfor=0; tgc=0; tcgcr=0; tdcmp=0; tdfg=0; telm=0
  tgaus=0; tlft=0; tmcls=0; tphs=0; tprb=0; tred=0; trest=0; ttres=0; ttred=0
  tunh=0; tprep=0; tinp=0; tsimp=0; tlook=0; tsearch=0
}
/^c reading input file +.+/ { file=$5 }
/^c +found +'p cnf/ { rn=$5; rc=$6; sub(/'/,"",rc) }
/^c +read [0-9]+ variables, +[0-9]+ clauses, +[0-9]+ literals in [0-9]+\.[0-9]+ seconds/ { n=$3; c=$5; l=$7; ptime=$10 }
/^c +blkd: [0-9]+ bces, +[0-9]+ removed, +[0-9]+ resolutions, +[0-9]+ steps/ { bce=$3; bcerm=$5; bcers=$7; bcest=$9 }
/^c +blkd: [0-9]+ blocking literals +[0-9]+%, +[0-9]+ pure/ { blkl=$3; pls=$7 }
/^c +cces: +[0-9]+ cces, +[0-9]+ eliminated, +[0-9]+ ate [0-9]+%, +[0-9]+ abce [0-9]+%/ { cce=$3; cceel=$5; cceat=$7; cceab=$10 }
/^c +cces: +[0-9]+ probed, +[0-9]+ lifted, +[0-9]+ failed/ { ccepr=$3; ccelf=$5; ccefd=$7 }
/^c +clff: +[0-9]+ cliffs, +[0-9]+ lifted, +[0-9]+ failed/ { clf=$3; clflf=$5; clffd=$7 }
/^c +clff: +[0-9]+ decisions, +[0-9]+ steps/ { clfdec=$3; clfst=$5 }
/^c +clls: +[0-9]+ sat, +[0-9]+ simp, +[0-9]+ freeze, +[0-9]+ melt/ { cllsat=$3; cllsim=$5; cllfre=$7; cllmel=$9 }
/^c +clls: +[0-9]+ add, +[0-9]+ assume,,? +[0-9]+ deref, +[0-9]+ failed/ { cllad=$3; cllass=$5; clldrf=$7; cllfd=$9 }
/^c +clls: +[0-9]+ cassume, +[0-9]+ mosat/ { cllcas=$3; cllmos=$5 }
/^c +coll: +[0-9]+ gcs, +[0-9]+ rescored clauses/ { colgcs=$3; colrec=$5 }
/^c +cgrs: +[0-9]+ count, +[0-9]+ esteps, +[0-9]+ csteps/ { cgcnt=$3; cgest=$5; cgcst=$7 }
/^c +cgrs: +[0-9]+ eqs, +[0-9]+ units/ { cgeqs=$3; cgrs1=$5 }
/^c +cgrs: +[0-9]+ matched \([0-9]+ ands [0-9]+%, [0-9]+ xors [0-9]+%, [0-9]+ ites [0-9]+%\)/ { cgmat=$3; cgmand=$5; cgmand=substr(cgmand, 2, length(cgmand)-1); cgmxor=$8; cgmite=$11 }
/^c +cgrs: +[0-9]+ simplified \([0-9]+ ands [0-9]+%, +[0-9]+ xors [0-9]+%, +[0-9]+ ites [0-9]+%\)/ { cgsim=$3; cgsand=$5; sub(/\(/,"",cgsand); cgsxor=$8; cgsite=$11 }
/^c +cgrs: +[0-9]+ extracted \([0-9]+ ands [0-9]+%, +[0-9]+ xors [0-9]+%, [0-9]+ ites [0-9]+%\)/ { cgex=$3; cgeand=$5; cgeand=substr(cgeand, 2, length(cgeand)-1); cgexor=$8; cgeite=$11 }
/^c +dcps: +[0-9]+ decompositions, +[0-9]+ equivalent [0-9]+%/ { dcpdcp=$3; dcpeq=$5 }
/^c +decs: +[0-9]+ decision, +[0-9]+ random [0-9]+\.[0-9]+%/ { decrd=$5 }
/^c +decs: +[0-9]+ flipped [0-9]+\.[0-9]+% +\(in [0-9]+ phases\)/ { decflp=$3; decphs=$7 }
/^c +elms: +[0-9]+ elims, +[0-9]+ eliminated [0-9]+%/ { elim=$3; elitd=$5 }
/^c +elms: +[0-9]+ small [0-9]+%, [0-9]+ large [0-9]+%/ { elsm=$3; ellg=$6 }
/^c +elms: +[0-9]+ tried small, +[0-9]+ succeeded [0-9]+%, [0-9]+ failed [0-9]+%/ { eltsm=$3; elsuc=$6; elfd=$9 }
/^c +elms: +[0-9]+ subsumed, +[0-9]+ strengthened, +[0-9]+ blocked/ { elsub=$3; elstr=$5; elblk=$7 }
/^c +elms: +[0-9]+ copies, +[0-9]+ resolutions, +[0-9]+ ipos/ { elcop=$3; elres=$5; elips=$7 }
/^c +elms: +[0-9]+ subchks, +[0-9]+ strchks/ { elsch=$3; elstc=$5 }
/^c +frcs: +[0-9]+ computed, +[0-9]+\.[0-9]+ - [0-9]+\.[0-9]+ average min-cut range/ { frcmp=$3; }
/^c +gaus: +[0-9]+ extractions, +[0-9]+ extracted, +[0-9]+\.[0-9]+ size, +[0-9]+ max/ { gsex=$3; gsexd=$5; gssz=$7; gsmx=$9 }
/^c +gaus: +exported [0-9]+ units, [0-9]+ binary and [0-9]+ ternary equations/ { gsex1=$4; gsex2=$6; gsex3=$9 }
/^c +gaus: +[0-9]+ gc, +[0-9]+ steps, +[0-9]+ extr [0-9]+%, +[0-9]+ elim/ { gsgc=$3; gsst=$5; gsexr=$7; gsel=$10 }
/^c +glue: +[0-9]+\.[0-9]+ avg, +[0-9]+\.[0-9]+ +scaled avg/ { glav=$3; glsav=$5 }
/^c +glue: +[0-9]+ maxredglue=[0-9]+/ { glue=$3; glmrg=$4; glmrg=substr(glmrg,12,length(glmrg)) }
/^c +lift: +[0-9]+ phases, +[0-9]+ probed +\([0-9]+ level1 [0-9]+%, [0-9]+ level2 [0-9]+%\)/ { lfph=$3; lfpr=$5; lflv1=$7; sub(/\(/,"",lflv1); lfl2=$10; sub(/\(/,"",lfl2) }
/^c +lift: +[0-9]+ units, +[0-9]+ equivalences, +[0-9]+ implications/ { lf1=$3; lfeq=$5; lfimp=$7 }
/^c +hbrs: +[0-9]+ hbrs = [0-9]+ trn [0-9]+% \+ [0-9]+ lrg [0-9]+%, [0-9]+ sub [0-9]+%/ { hbrs=$3; hbrtr=$6; hbrlrg=$10; hbrsub=$13 }
/^c +lrnd: +[0-9]+ clauses, +[0-9]+ uips [0-9]+%, [0-9]+\.[0-9]+ length, +[0-9]+\.[0-9]+ glue/ { lncl=$3; lnuip=$5; lnlen=$8; lnglu=$10 }
/^c +ints: +[0-9]+ luby +\([0-9]+ steps\), +[0-9]+ inout \([0-9]+ steps\)/ { intlb=$3; intlbs=$5; intlbs=substr(intlbs,2,length(intlbs)-1); intio=$7; intios=$9; intios=substr(intios,2,length(intios)-1) }
/^c +mins: +[0-9]+ learned lits, +[0-9]+% minimized/ { mnll=$3; mnllm=$6; sub(/%/,"",mnllm) }
/^c +move: +moved [0-9]+, +[0-9]+ binary [0-9]+%, +[0-9]+ ternary [0-9]+%/ { mvd=$4; sub(/,/,"",mvd); mvd2=$5; mvd3=$8 }
/^c +otfs: +str [0-9]+ dyn \([0-9]+ red, [0-9]+ irr\) [0-9]+ drv [0-9]+%, [0-9]+ rst [0-9]+%/ { otstdy=$4; otstrd=$6; sub(/\(/,"",otstrd); otstir=$8; otstdr=$10; otstrs=$13 }
/^c +otfs: +sub [0-9]+ dyn +\([0-9]+ red, +[0-9]+ irr\)/ { otsbdy=$4; otsbrd=$6; sub(/\(/,"",otsbrd); otsbir=$8 }
/^c +phas: +[0-9]+ computed, +[0-9]+ set, +[0-9]+ pos +\([0-9]+%\), +[0-9]+ neg \([0-9]+%\)/ { phcmp=$3; phset=$5; phpos=$7; phneg=$10 }
/^c +prbs: +[0-9]+ basic, +[0-9]+ probed, +[0-9]+ failed, +[0-9]+ lifted/ { prbbs=$3; prbpr=$5; prbfd=$7; prblf=$9 }
/^c +prps: +[0-9]+ props/ { prppr=$3 }
/^c +prps: +[0-9]+% srch, +[0-9]+% simp, +[0-9]+% lkhd/ { prpsr=$3; sub(/%/, "", prpsr); prpsm=$5; sub(/%/,"",prpsm); prplk=$7;  sub(/%/,"",prplk) }
/^c +psns: +[0-9]+ searches, +[0-9]+ hits/ { psns=$3; psnsh=$5 }
/^c +queu: +[0-9]+ new, +[0-9]+ del, +[0-9]+ maximum priority/ { qunw=$3; qudl=$5; qump=$7 }
/^c +queu: +[0-9]+ merged, +[0-9]+ collected, +[0-9]+ gcs/ { qumg=$3; qucl=$5; qugc=$7 }
/^c +queu: +[0-9]+ deprioritized, +[0-9]+\.[0-9]+ lines on average/ { qudp=$3; qula=$5 }
/^c +reds: +[0-9]+ count, +[0-9]+ reset, +[0-9]+ acts +[0-9]+%, +[0-9]+ exp [0-9]+%/ { rdcnt=$3; rdrst=$5; rdact=$7; rdexp=$10 }
/^c +reds: +[0-9]+ arithmetic [0-9]+%, +[0-9]+ double [0-9]+%/ { rdar=$3; rddbl=$6 }
/^c +rmbd: +[0-9]+ removed, +[0-9]+ red/ { rmbrm=$3; rmbrd=$5 }
/^c +rsts: +[0-9]+ restarts +[0-9]+%, +[0-9]+ skipped [0-9]+%/ { rts=$3; rtssk=$6 }
/^c +rsts: +[0-9]+ kept +[0-9]+\.[0-9]+% +average [0-9]+\.[0-9]+%/ { rtskp=$3; rtsav=$7; sub(/%/,"",rtsav) }
/^c +simp: +[0-9]+ count +\([0-9]+ ilim [0-9]+%, +[0-9]+ plim [0-9]+%, +[0-9]+ clim [0-9]+%\)/ { smcnt=$3; smilm=$5; sub(/\(/, "", smilm); smplm=$8; smclm=$11; }
/^c +trnr: +[0-9]+ count, +[0-9]+ bin, +[0-9]+ trn, +[0-9]+ steps/ { trncn=$3; trn2=$5; trn3=$7; trnst=$9 }
/^c +tops: +[0-9]+ fixed +[0-9]+%, +[0-9]+ iterations/ { tpfix=$3; tpit=$6 }
/^c +trds: +[0-9]+ transitive reductions, +[0-9]+ removed, +[0-9]+ failed/ { trdtr=$3; trdrm=$6; trdfd=$8 }
/^c +trds: +[0-9]+ nodes, +[0-9]+ edges, +[0-9]+ steps/ { trdnd=$3; trded=$5; trdst=$7 }
/^c +unhd: +[0-9]+ count, +[0-9]+ rounds, +[0-9]+ steps/ { uhcnt=$3; uhrnd=$5; uhstp=$7 }
/^c +unhd: +[0-9]+ non-trivial sccs of average size [0-9]+\.[0-9]+/ { uhscc=$3; uhsccs=$9 }
/^c +unhd: +[0-9]+ units, +[0-9]+ bin, +[0-9]+ trn, +[0-9]+ lrg/ { uh1=$3; uh2=$5; uh3=$7; uhlg=$9 }
/^c +unhd: +[0-9]+ failed, +[0-9]+ stamp, +[0-9]+ lits, +[0-9]+ bin, +[0-9]+ trn, +[0-9]+ lrg/ { uhfld=$3; uhfst=$5; uhfd=$7; uhf2=$9; uhf3=$11; uhflg=$13 }
/^c +unhd: +[0-9]+ tauts, +[0-9]+ bin [0-9]+%, +[0-9]+ trn [0-9]+%, +[0-9]+ lrg [0-9]+%/ { uht=$3; uht2=$5; uht3=$8; uhtlg=$11 }
/^c +unhd: +[0-9]+ tauts, +[0-9]+ stamp +[0-9]+%, +[0-9]+ red +[0-9]+%/ { uhtst=$5; uhtrd=$8 }
/^c +unhd: +[0-9]+ hbrs, +[0-9]+ trn [0-9]+%, +[0-9]+ lrg [0-9]+%, +[0-9]+ red [0-9]+%/ { uhhb=$3; uhhb3=$5; uhhbl=$8; uhhbr=$11 }
/^c +unhd: +[0-9]+ strd, +[0-9]+ bin [0-9]+%, +[0-9]+ trn +[0-9]+%, +[0-9]+ lrg [0-9]+%, +[0-9]+ red [0-9]+%/ { uhst=$3; uhst2=$5; uhst3=$8; uhstl=$11; uhstr=$14 }
/^c +vars: +[0-9]+ remaining +[0-9]+% and [0-9]+ removed [0-9]+% out of [0-9]+/ { nrem=$3; nrmv=$7 }
/^c +vars: +[0-9]+ fixed [0-9]+%, +[0-9]+ eliminated [0-9]+%, +[0-9]+ equivalent [0-9]+%/ { nfix=$3; nelm=$6; neq=$9 }
/^c +vsts: +[0-9]+ visits, +[0-9]+% srch, +[0-9]+% simp, +[0-9]+% lkhd/ { vst=$3; vstsr=$5; sub(/%/,"",vstsr); vstsm=$7; sub(/%/,"",vstsm); vstlk=$9; sub(/%/,"",vstlk) }
/^c +wchs: +[0-9]+ pushed, +[0-9]+ enlarged, +[0-9]+ defrags/ { wchp=$3; wchen=$5; wchdf=$7 }
/^c +[0-9]+ decisions, [0-9]+ conflicts/ { dec=$2; cfs=$4 }
/^c +[0-9]+ propagations/ { r1=$2 }
/^c +[0-9]+\.[0-9]+ seconds, +[0-9]+\.[0-9]+ MB/ { t=$2; mem=$4 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +analysis/ { tanl=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +block/ { tblk=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +bump/ { tbmp=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +card/ { tcrd=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +cce/ { tcce=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +cliff/ { tclf=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +cutwidth/ { tcwi=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +decide/ { tdec=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +force/ { tfor=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +gc/ { tgc=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +cgrclsr/ { tcgcr=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +decomp/ { tdcmp=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +defrag/ { tdfg=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +elim/ { telm=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +gauss/ { tgaus=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +lift/ { tlft=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +mincls/ { tmcls=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +phase/ { tphs=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +probe/ { tprb=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +reduce/ { tred=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +restart/ { trest=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +ternres/ { ttres=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +transred/ { ttred=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +unhide/ { tunh=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +preprocessing/ { tprep=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +inprocessing/ { tinp=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +simplifying/ { tsimp=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +lookahead/ { tlook=$2 }
/^c +[0-9]+\.[0-9]+ +[0-9]+% +search/ { tsearch=$2 }
/^s +UNSATISFIABLE *$/ { sat=0 }
/^s +SATISFIABLE *$/ { sat=1 }
END {
  print rn " " rc " " t " " sat " " cfs " " dec " " rts " " r1 " " \
  pls " " mem " " ptime " \"" file "\" " n " " c " " l " " bce " "  bcerm " " \
  bcers " "  bcest " " blkl " " cce " " cceel " " cceat " " cceab " " \
  ccepr " " ccelf " " ccefd " " clf " " clflf " " clffd " " clfdec " " \
  clfst " " cllsat " " cllsim " " cllfre " " cllmel " " cllad " " cllass " " \
  clldrf " " cllfd " " cllcas " " cllmos " " colgcs " " colrec " " cgcnt " " \
  cgest " " cgcst " " cgeqs " " cgrs1 " " cgmat " " cgmand " " cgmxor " " \
  cgmite " " cgsim " " cgsand " " cgsxor " " cgsite " " \
  cgex " " cgeand " " cgexor " " cgeite " " dcpdcp " " dcpeq " " decrd " " \
  decflp " " decphs " " elim " " elitd " " elsm " " ellg " " eltsm " " \
  elsuc " " elfd " " elsub " " elstr " " elblk " " elcop " " elres " " \
  elips " " elsch " " elstc " " frcmp " " gsex " " gsexd " " gssz " " \
  gsmx " " gsex1 " " gsex2 " " gsex3 " " gsgc " " gsst " " gsexr " " \
  gsel " " glav " " glsav " " glue " " glmrg " " lfph " " lfpr " " \
  lflv1 " " lfl2 " " lf1 " " lfeq " " lfimp " " hbrs " " hbrtr " " hbrlrg " " \
  hbrsub " " lncl " " lnuip " " lnlen " " lnglu " " intlb " " intlbs " " \
  intio " " intios " " mnll " " mnllm " " mvd " " mvd2 " " mvd3 " " \
  otstdy " " otstrd " " otstir " " otstdr " " otstrs " " otsbdy " " otsbrd " " \
  otsbir " " phcmp " " phset " " phpos " " phneg " " prbbs " " prbpr " " \
  prbfd " " prblf " " prppr " " prpsr " " prpsm " " prplk " " psns " " \
  psnsh " "  qunw " " qudl " " qump " " qumg " " qucl " " qugc " " qudp " " \
  qula " " rdcnt " " rdrst " " rdact " " rdexp " " rdar " " rddbl " " \
  rmbrm " " rmbrd " " rtssk " " rtskp " " rtsav " " smcnt " " smilm " " \
  smplm " " smclm " " trncn " " trn2 " " trn3 " " trnst " " tpfix " " tpit " " \
  trdtr " " trdrm " " trdfd " " trdnd " " trded " " trdst " " uhcnt " " \
  uhrnd " " uhstp " " uhscc " " uhsccs " " uh1 " " uh2 " " uh3 " " uhlg " " \
  uhfld " " uhfst " " uhfd " " uhf2 " " uhf3 " "  uhflg " " uht " " uht2 " " \
  uht3 " " uht3 " " uhtlg " " uhtst " " uhtrd " " uhhb " " uhhb3 " " uhhbl " " \
  uhhbr " " uhst " " uhst2 " " uhst3 " " uhstl " " uhstr " " nrem " " nrmv " " \
  nfix " " nelm " " neq " " vst " " vstsr " " vstsm " " vstlk " " wchp " " \
  wchen " " wchdf " " tanl " " tblk " " tbmp " " tcrd " " tcce " " tclf " " \
  tcwi " " tdec " " tfor " " tgc " " tcgcr " " tdcmp " " tdfg " " telm " " \
  tgaus " " tlft " " tmcls " " tphs " " tprb " " tred " " trest " " \
  ttres " " ttred " " tunh " " tprep " " tinp " " tsimp " " tlook " " tsearch
}
