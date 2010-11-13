// Matthew Gwynne, 25.8.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/Ramsey_2_5_5.hpp
  \brief On investigations into %Ramsey problems of the form ramsey_2^2(5,5)


  \todo General information
  <ul>
   <li> Known is 43 <= ramsey_2^2(5) <= 49. </li>
   <li> So we get 1-2 million clauses. </li>
   <li> To generate an instance of SAT problem specifying "ramsey_2^2(5) > n?"
   use the following:
   \verbatim
Ramsey-O3-DNDEBUG 5 5 2 6 | ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_6.cnf
   \endverbatim
   where n has been replaced by 6 in this example.
   </li>
   <li> To generate the additional clauses for each of the symmetry 
   breaking techniques (see 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/RamseyProblems.hpp)
   and cardinality constraints, run the following in maxima (generating
   each of the additional clauses for a range of n values):
   <ul>
    <li> Symmetry breaking by fixing colours/labels to break monochromatic
    k-subsets:
    \verbatim
for n : 5 thru 49 do output_ramsey2_symbr1_stdname(5,n);
    \endverbatim
    </li>
    <li> Symmetry breaking by fixing monochromatic cliques:
    \verbatim
for n : 5 thru 49 do output_ramsey2_symbr2_stdname(n);
    \endverbatim
    </li>
    <li> Symmetry breaking by recursive application of the pigeon hole
    principle:
    \verbatim
for n : 5 thru 49 do output_ramsey2_symbr3_stdname(n);
    \endverbatim
    </li>
    <li> Cardinality constraints:
    \verbatim
:lisp (ext:set-limit 'ext:heap-size 3000000000)
:lisp (ext:set-limit 'ext:frame-stack 10000)
:lisp (ext:set-limit 'ext:c-stack 200000)
:lisp (ext:set-limit 'ext:lisp-stack 200000)
for n : 5 thru 49 do block([n_e : binomial(n,2)],
  output_fcs(
    sconcat("Card ",n),
    fcl2fcs(standardise_fcl(cl2fcl(unary_bb_crd2fcl([floor(n_e/2),create_list(i,i,1,n_e),ceiling(n_e/2)])))[1]),
    sconcat("Card_n",n,".cnf")))$
    \endverbatim
    </li>
   </ul>
   </li>
  </ul>


  \todo Using DPLL and Conflict driven solvers
  <ul>
   <li> Initial investigations:
    <ul>
     <li> OKsolver can handle n=33 in ~ 3 minutes, but has problems with
     n=34. </li>
     <li> march_pl seems to have problems already for n=30. </li>
     <li> minisat can handle n=30 in 2 seconds; n=35 ? </li>
     <li> UnitMarch: n=30 in 3 seconds, but n=33 unsolved after 20 minutes.
     </li>
    </ul>
   </li>
   <li> A more systematic investigation, involving each of the symmetry breaking
   techniques and cardinality constraints (see 
   Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/SymmetryBreaking.hpp)
   can be generated and run using the experiment system discussed at "General 
   experiment system" in 
   Experimentation/Investigations/Cryptography/AES/plans/BreakingAES.hpp), with
   the following "generate_exp.sh" script:
   \verbatim
# Generates Ramsey experiments for "ramsey_2^2(5,5) > n?" for 5 <= n <= 49.


for n in `seq 5 49`; do
# No Symmetry breaking
    echo "PICOSAT_"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_${n}.cnf && ./solvers/picosat913 -v Ramsey_exp.cnf > Ramsey_5_5_2_${n}.cnf.result.picosat 2>&1 && rm Ramsey_5_5_2_${n}.cnf" >> experiments;
    echo "MINISAT2_"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_${n}.cnf && ./solvers/minisat2 Ramsey_5_5_2_${n}.cnf minisat-temp > Ramsey_5_5_2_${n}.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_5_5_2_${n}.cnf.result.minisat2;rm Ramsey_5_5_2_${n}.cnf" >> experiments;
    echo "MARCH_PL_"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_${n}.cnf && ./solvers/march_pl Ramsey_5_5_2_${n}.cnf > Ramsey_5_5_2_${n}.cnf.result.march_pl 2>&1;rm Ramsey_5_5_2_${n}.cnf" >> experiments;
    echo "OKSOLVER_"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_${n}.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_5_5_2_${n}.cnf > Ramsey_5_5_2_${n}.cnf.result.OKsolver 2>&1; mv Ramsey_5_5_2_${n}.cnf.mo Ramsey_5_5_2_${n}.cnf.mo.OKsolver;mv Ramsey_5_5_2_${n}.cnf.pa Ramsey_5_5_2_${n}.cnf.pa.OKsolver;rm Ramsey_5_5_2_${n}.cnf" >> experiments;
    echo "OKSOLVERM2PP_"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_${n}.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_5_5_2_${n}.cnf > Ramsey_5_5_2_${n}.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_5_5_2_${n}.cnf_m2pp_*.mo Ramsey_5_5_2_${n}.cnf.mo.OKsolver-m2pp;mv Ramsey_5_5_2_${n}.cnf_m2pp_*.pa Ramsey_5_5_2_${n}.cnf.pa.OKsolver-m2pp;rm Ramsey_5_5_2_${n}.cnf" >> experiments;
# Symmetry breaking 1
   echo "PICOSAT_FC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_FC_5_${n}.ecnf > Ramsey_5_5_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_FC.ecnf > Ramsey_5_5_2_${n}_SB_FC.cnf && ./solvers/picosat913 -v Ramsey_exp.cnf > Ramsey_5_5_2_${n}_SB_FC.cnf.result.picosat 2>&1 && rm -f Ramsey_5_5_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
    echo "MINISAT2_FC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_FC_5_${n}.ecnf > Ramsey_5_5_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_FC.ecnf > Ramsey_5_5_2_${n}_SB_FC.cnf && ./solvers/minisat2 Ramsey_5_5_2_${n}_SB_FC.cnf minisat-temp > Ramsey_5_5_2_${n}_SB_FC.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_5_5_2_${n}_SB_FC.cnf.result.minisat2;rm -f Ramsey_5_5_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
  echo "MARCH_PL_FC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_FC_5_${n}.ecnf > Ramsey_5_5_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_FC.ecnf > Ramsey_5_5_2_${n}_SB_FC.cnf && ./solvers/march_pl Ramsey_5_5_2_${n}_SB_FC.cnf > Ramsey_5_5_2_${n}_SB_FC.cnf.result.march_pl 2>&1;rm -f Ramsey_5_5_2_${n}{_SB_FC,}.{e,}cnf" >> experiments;
   echo "OKSOLVER_FC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_FC_5_${n}.ecnf > Ramsey_5_5_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_FC.ecnf > Ramsey_5_5_2_${n}_SB_FC.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_5_5_2_${n}_SB_FC.cnf > Ramsey_5_5_2_${n}_SB_FC.cnf.result.OKsolver 2>&1; mv Ramsey_5_5_2_${n}_SB_FC.cnf.mo Ramsey_5_5_2_${n}_SB_FC.cnf.mo.OKsolver;mv Ramsey_5_5_2_${n}_SB_FC.cnf.pa Ramsey_5_5_2_${n}_SB_FC.cnf.pa.OKsolver;rm -f Ramsey_5_5_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_FC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_FC_5_${n}.ecnf > Ramsey_5_5_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_FC.ecnf > Ramsey_5_5_2_${n}_SB_FC.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_5_5_2_${n}_SB_FC.cnf > Ramsey_5_5_2_${n}_SB_FC.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_5_5_2_${n}_SB_FC.cnf_m2pp_*.mo Ramsey_5_5_2_${n}_SB_FC.cnf.mo.OKsolver-m2pp;mv Ramsey_5_5_2_${n}_SB_FC.cnf_m2pp_*.pa Ramsey_5_5_2_${n}_SB_FC.cnf.pa.OKsolver-m2pp;rm -f Ramsey_5_5_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
# Symmetry breaking 2
   echo "PICOSAT_MC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_5_5_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_MC.ecnf > Ramsey_5_5_2_${n}_SB_MC.cnf && ./solvers/picosat913 -v Ramsey_5_5_2_${n}_SB_MC.cnf > Ramsey_5_5_2_${n}_SB_MC.cnf.result.picosat 2>&1 && rm -f Ramsey_5_5_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
    echo "MINISAT2_MC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_5_5_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_MC.ecnf > Ramsey_5_5_2_${n}_SB_MC.cnf && ./solvers/minisat2 Ramsey_5_5_2_${n}_SB_MC.cnf minisat-temp > Ramsey_5_5_2_${n}_SB_MC.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_5_5_2_${n}_SB_MC.cnf.result.minisat2;rm -f Ramsey_5_5_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
    echo "MARCH_PL_MC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_5_5_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_MC.ecnf > Ramsey_5_5_2_${n}_SB_MC.cnf && ./solvers/march_pl Ramsey_5_5_2_${n}_SB_MC.cnf > Ramsey_5_5_2_${n}_SB_MC.cnf.result.march_pl 2>&1;rm -f Ramsey_5_5_2_${n}{_SB_MC,}.{e,}cnf" >> experiments;
    echo "OKSOLVER_MC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_5_5_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_MC.ecnf > Ramsey_5_5_2_${n}_SB_MC.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_5_5_2_${n}_SB_MC.cnf > Ramsey_5_5_2_${n}_SB_MC.cnf.result.OKsolver 2>&1; mv Ramsey_5_5_2_${n}_SB_MC.cnf.mo Ramsey_5_5_2_${n}_SB_MC.cnf.mo.OKsolver;mv Ramsey_5_5_2_${n}_SB_MC.cnf.pa Ramsey_5_5_2_${n}_SB_MC.cnf.pa.OKsolver;rm -f Ramsey_5_5_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_MC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_5_5_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_MC.ecnf > Ramsey_5_5_2_${n}_SB_MC.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_5_5_2_${n}_SB_MC.cnf > Ramsey_5_5_2_${n}_SB_MC.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_5_5_2_${n}_SB_MC.cnf_m2pp_*.mo Ramsey_5_5_2_${n}_SB_MC.cnf.mo.OKsolver-m2pp;mv Ramsey_5_5_2_${n}_SB_MC.cnf_m2pp_*.pa Ramsey_5_5_2_${n}_SB_MC.cnf.pa.OKsolver-m2pp;rm -f Ramsey_5_5_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
# Symmetry breaking 3
   echo "PICOSAT_PHP"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_5_5_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_PHP.ecnf > Ramsey_5_5_2_${n}_SB_PHP.cnf && ./solvers/picosat913 -v Ramsey_5_5_2_${n}_SB_PHP.cnf > Ramsey_5_5_2_${n}_SB_PHP.cnf.result.picosat 2>&1 && rm -f Ramsey_5_5_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
    echo "MINISAT2_PHP"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_5_5_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_PHP.ecnf > Ramsey_5_5_2_${n}_SB_PHP.cnf && ./solvers/minisat2 Ramsey_5_5_2_${n}_SB_PHP.cnf minisat-temp > Ramsey_5_5_2_${n}_SB_PHP.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_5_5_2_${n}_SB_PHP.cnf.result.minisat2;rm -f Ramsey_5_5_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
    echo "MARCH_PL_PHP"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_5_5_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_PHP.ecnf > Ramsey_5_5_2_${n}_SB_PHP.cnf && ./solvers/march_pl Ramsey_5_5_2_${n}_SB_PHP.cnf > Ramsey_5_5_2_${n}_SB_PHP.cnf.result.march_pl 2>&1;rm -f Ramsey_5_5_2_${n}{_SB_PHP,}.{e,}cnf" >> experiments;
    echo "OKSOLVER_PHP"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_5_5_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_PHP.ecnf > Ramsey_5_5_2_${n}_SB_PHP.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_5_5_2_${n}_SB_PHP.cnf > Ramsey_5_5_2_${n}_SB_PHP.cnf.result.OKsolver 2>&1; mv Ramsey_5_5_2_${n}_SB_PHP.cnf.mo Ramsey_5_5_2_${n}_SB_PHP.cnf.mo.OKsolver;mv Ramsey_5_5_2_${n}_SB_PHP.cnf.pa Ramsey_5_5_2_${n}_SB_PHP.cnf.pa.OKsolver;rm -f Ramsey_5_5_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_PHP"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} > Ramsey_5_5_2_${n}.ecnf && ./merge_cnf.sh Ramsey_5_5_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_5_5_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_5_5_2_${n}_SB_PHP.ecnf > Ramsey_5_5_2_${n}_SB_PHP.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_5_5_2_${n}_SB_PHP.cnf > Ramsey_5_5_2_${n}_SB_PHP.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_5_5_2_${n}_SB_PHP.cnf_m2pp_*.mo Ramsey_5_5_2_${n}_SB_PHP.cnf.mo.OKsolver-m2pp;mv Ramsey_5_5_2_${n}_SB_PHP.cnf_m2pp_*.pa Ramsey_5_5_2_${n}_SB_PHP.cnf.pa.OKsolver-m2pp;rm -f Ramsey_5_5_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
# Cardinality Constraints
   echo "PICOSAT_CC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_${n}.cnf && ./merge_cnf.sh Card/Card_n${n}.cnf Ramsey_5_5_2_${n}.cnf > Ramsey_5_5_2_${n}_CC.cnf && ./solvers/picosat913 -v Ramsey_5_5_2_${n}_CC.cnf > Ramsey_5_5_2_${n}_CC.cnf.result.picosat 2>&1 && rm -f Ramsey_5_5_2_${n}{,_CC}.{e,}cnf" >> experiments;
    echo "MINISAT2_CC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_${n}.cnf && ./merge_cnf.sh Card/Card_n${n}.cnf Ramsey_5_5_2_${n}.cnf > Ramsey_5_5_2_${n}_CC.cnf && ./solvers/minisat2 Ramsey_5_5_2_${n}_CC.cnf minisat-temp > Ramsey_5_5_2_${n}_CC.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_5_5_2_${n}_CC.cnf.result.minisat2;rm -f Ramsey_5_5_2_${n}{,_CC}.{e,}cnf" >> experiments;
    echo "OKSOLVER_CC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_${n}.cnf && ./merge_cnf.sh Card/Card_n${n}.cnf Ramsey_5_5_2_${n}.cnf > Ramsey_5_5_2_${n}_CC.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_5_5_2_${n}_CC.cnf > Ramsey_5_5_2_${n}_CC.cnf.result.OKsolver 2>&1; mv Ramsey_5_5_2_${n}_CC.cnf.mo Ramsey_5_5_2_${n}_CC.cnf.mo.OKsolver;mv Ramsey_5_5_2_${n}_CC.cnf.pa Ramsey_5_5_2_${n}_CC.cnf.pa.OKsolver;rm -f Ramsey_5_5_2_${n}{,_CC}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_CC"$n "./Ramsey-O3-DNDEBUG 5 5 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_5_5_2_${n}.cnf && ./merge_cnf.sh Card/Card_n${n}.cnf Ramsey_5_5_2_${n}.cnf > Ramsey_5_5_2_${n}_CC.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_5_5_2_${n}_CC.cnf > Ramsey_5_5_2_${n}_CC.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_5_5_2_${n}_CC.cnf_m2pp_*.mo Ramsey_5_5_2_${n}_CC.cnf.mo.OKsolver-m2pp;mv Ramsey_5_5_2_${n}_CC.cnf_m2pp_*.pa Ramsey_5_5_2_${n}_CC.cnf.pa.OKsolver-m2pp;rm -f Ramsey_5_5_2_${n}{,_CC}.{e,}cnf" >> experiments;
done
   \endverbatim
   </li>
   <li> The above experiment (in the generation script) is currently running on
   cspasiphae with a timeout of 2 hours. </li>
   <li> Combining all such investigations (different symmetry breaking 
   techniques, cardinality contraints etc) into a single experiment allows one
   to run through all such investigations for a short time and then split the
   experiments if certain aspects are slowing the entire experiment set down
   or it makes more sense to run certain parts, or solvers in parallel. </li>
  </ul>


  \todo Using UBCSAT
  <ul>
   <li> The list of algorithms from ubcsat:
    <ol>
     <li> "ubcsat-okl -alg gsat -runs 100 -i Ramsey_5_2_n.cnf": trivial for
     n=30; n = 35 needs 300 - 30000 steps. Using "-cutoff 2000" seems
     reasonable. </li>
     <li> gwsat: trivial for n=30; n = 35 ? </li>
     <li> hsat: n = 35 needs ~ 3000 steps; n = 40 ? </li>
     <li> adaptnovelty+: n = 35 ? </li>
     <li> saps: n = 35 in 600 steps. Using "-cutoff 2000" seems reasonable;
     looks strong. n = 40 ? </li>
     <li> rsaps: similar; but looks bleak for n = 40 (cutoffs 10000 or
     30000 yield nearly the same). </li>
     <li> samd performs yet best, and also scales to higher cutoffs,
     but very slowly: With n=40, cutoff = 10 000 000, in 12 rounds 9 outcomes
     were 30-something, one was 46, and we had 23 (the minimum) and 27
     (regarding the falsified clauses). </li>
    </ol>
   </li>
   <li> Investigating samd:
    <ol>
     <li> n=35 very easy. </li>
     <li> n=38:
     \verbatim
> ubcsat-okl -alg samd -cutoff 20000 -runs 10000 -i Ramsey_5_5_2_38.cnf > Exp_Ramsey_5_5_2_38
Clauses = 1003884
Variables = 703
TotalLiterals = 10038840
FlipsPerSecond = 800    
BestStep_Mean = 11197.468000
Steps_Mean = 20000.000000
Steps_Max = 20000.000000
PercentSuccess = 0.00  
BestSolution_Mean = 16.367800
BestSolution_Median = 16.000000
BestSolution_Min = 7.000000
BestSolution_Max = 51.000000
     \endverbatim
     We need to investigate the distribution of falsified clauses reached,
     using R.
     </li>
     <li> Summary:
     \verbatim
> E = read.table("Exp_Ramsey_5_5_2_38", colClasses = c("character", "factor", "integer", "integer", "integer", "character"))
> summary(E)
 sat            min            osteps   
 0:10000   Min.   : 7.00   Min.   :  166
           1st Qu.:14.00   1st Qu.: 6576
           Median :16.00   Median :11372
           Mean   :16.37   Mean   :11197
           3rd Qu.:18.00   3rd Qu.:16167
           Max.   :51.00   Max.   :20000
     \endverbatim
     </li>
     <li>
     \verbatim
> plot(min ~ osteps, data = E)
     \endverbatim
     shows that there is a strong correlation between the worst min-values
     and the smallest osteps-values, but then the correlation gets much
     weaker (and the largest osteps-value include also some rather bad cases
     (without much improvement). </li>
     <li> Frequencies of min-values:
     \verbatim
> table(E$min)

   7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22
   1    4   21   72  181  410  798 1236 1545 1598 1315 1007  621  413  238  130

  23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38
  94   65   63   43   28   26   14   14   19    2    9    3    4    1    1    6

  39   40   41   42   43   44   45   47   50   51
   3    2    1    1    2    2    4    1    1    1
     \endverbatim
     concentrates around the mean, but with a long tail. </li>
     <li> Frequencies of osteps-values:
     \verbatim
> hist(E$osteps, plot=F)
$breaks
 [1]     0  1000  2000  3000  4000  5000  6000  7000  8000  9000 10000 11000
[13] 12000 13000 14000 15000 16000 17000 18000 19000 20000

$counts
 [1] 133 277 382 449 476 498 516 518 510 531 512 541 527 534 500 511 581 594 679
[20] 731
     \endverbatim
     </li>
     <li> For cutoff = 40000:
     \verbatim
Clauses = 1003884
Variables = 703
TotalLiterals = 10038840
FlipsPerSecond = 1273   
BestStep_Mean = 20416.061900
Steps_Mean = 40000.000000
Steps_Max = 40000.000000
PercentSuccess = 0.00  
BestSolution_Mean = 14.926500
BestSolution_Median = 15.000000
BestSolution_Min = 4.000000
BestSolution_Max = 51.000000
     \endverbatim
     </li>
     <li> There is now more data available ("Exp_Ramsey_5_5_2_38-samd_40000_2")
     which needs to be merged with the old data (and then the evaluation
     needs to be repeated). </li>
     <li> The density of osteps has a clear minimum at around 20000, while
     the minimum 4 is reached for osteps = 34213. Linear modelling via
     L = lm(min ~ poly(log(osteps), k),data=E) makes sense up to k=4 (plotting
     by
     \verbatim
plot(log(E$osteps),E$min)
points(log(E$osteps),predict(L),col="blue")
     \endverbatim
     shows a reasonable fit). </li>
     <li> For cutoff = 80000:
     \verbatim
ubcsat-okl -alg samd -cutoff 80000 -runs 10000 -i Ramsey_5_5_2_38.cnf > Exp_Ramsey_5_5_2_38_80000
Clauses = 1003884
Variables = 703
TotalLiterals = 10038840
FlipsPerSecond = 3404   
BestStep_Mean = 35709.517200
Steps_Mean = 80000.000000
Steps_Max = 80000.000000
PercentSuccess = 0.00  
BestSolution_Mean = 13.943300
BestSolution_Median = 13.000000
BestSolution_Min = 4.000000
BestSolution_Max = 56.000000
     \endverbatim
     </li>
     <li> Also experimental results for a 10x larger experiments are available
     (the above smaller experiment should be merged into the larger one).
     \verbatim
Clauses = 1003884
Variables = 703
TotalLiterals = 10038840
FlipsPerSecond = 3470   
BestStep_Mean = 35903.717040
Steps_Mean = 80000.000000
Steps_Max = 80000.000000
PercentSuccess = 0.00  
BestSolution_Mean = 13.908400
BestSolution_Median = 13.000000
BestSolution_Min = 1.000000
BestSolution_Max = 67.000000
     \endverbatim
     </li>
     <li> Basically the same picture as for cutoff = 20000; the main question
     here is which questions to ask?? </li>
     <li> It seems reasonable to consider log(osteps) instead of osteps;
     plot(min ~ log(osteps), data = E) shows then a clear negative correlation,
     but where, due to the "cutoff effect", the minimum is reached already at
     around osteps = 63000 (not with 80000). </li>
     <li> It seems that the density of osteps reaches its minimum also around
     osteps = 63000?! (use hist(E$osteps,breaks=50)) </li>
     <li> This seems reasonable: The higher osteps the better the result but
     the harder to get there, except of the "cutoff-effect", which makes
     very high osteps-values more likely but less fruitful. </li>
     <li> Using here log(osteps) doesn't seem sensible:
     plot(density(E$osteps),lab=c(10,5,7)) shows the minimum density somewhat
     below 60000, while plot(density(log(E$osteps)),lab=c(10,5,7)) has its
     *maximum* around exp(11) ~ 60000. </li>
     <li> n=40:
      <ol>
       <li> cutoff = 10 000 000 and noimprove = 1 000 000: 100 rounds,
       maximum 3 500 000 steps, average 1 700 000 steps, minimum 7
       falsified clauses. </li>
       <li> cutoff = 20 000 000, noimprove = 3 000 000: 100 rounds, maximum
       10 500 000 steps, average 4 300 000 steps, minimum 1 falsified clause
       (seed 18745606). </li>
       <li> One round, cutoff = 20 000 000, with previous best seed: didn't
       improve the result. </li>
       <li> So let's try 500 runs, with cutoff = 20 000 000, noimprove =
       2 000 000:
       \verbatim
BestStep_Mean = 1121189.316000
Steps_Mean = 3121190.316000
Steps_Max = 7623900.000000
PercentSuccess = 0.00
BestSolution_Mean = 34.696000
BestSolution_Median = 35.000000
BestSolution_Min = 7.000000
BestSolution_Max = 76.000000
       \endverbatim
       </li>
       <li> Alright, with noimprove = 3 000 000:
        <ol>
         <li>
         \verbatim
BestStep_Mean = 1596309.594000
Steps_Mean = 4590310.592000
Steps_Max = 14784949.000000
PercentSuccess = 0.20
BestSolution_Mean = 33.976000
BestSolution_Median = 35.000000
BestSolution_Min = 0.000000
BestSolution_Max = 61.000000
         \endverbatim
         </li>
         <li> With seed = 391532901 a solution was found (in round
         293, using 964281 steps)!
          <ul>
           <li> The command line instruction to reproduce this is:
           \verbatim
ubcsat -alg samd -seed 391532901 -cutoff 1000000 -i Ramsey_5_2_40.cnf
           \endverbatim
           </li>
           <li> A solution is output by adding "-solve". </li>
           <li> Perhaps this solution should be stored; the question is
           where? Perhaps this is similar to data for minimally unsatisfiable
           clause-sets etc., which finally is too much to put into source
           control, but should exist as additional packages. </li>
           <li> Of course, there is the whole complex of "database
           questions"; see Experimentation/Database/plans/general.hpp
           <li> We need to find out whether this solution is of interest
           (i.e., "unknown") or not. </li>
           <li> So we need to find out about the known solutions for n=40
           in the literature, and about classifications of solutions (when
           solutions are considered to be the "same"). </li>
          </ul>
         </li>
         <li> So, in general perhaps one should just use a cutoff of
         1 000 000, and use more rounds? </li>
        </ol>
       </li>
      </ol>
     </li>
     <li> n=41:
      <ol>
       <li> cutoff =  10 000 000, with 40 rounds most results were
       fiftyish to sixtyish, but one outlier reached 28 (the minimum). </li>
       <li> cutoff = 20 000 000, noimprove = 2 000 000:
       100 rounds has a maximum of 8 000 000 steps, average of
       3 300 000 steps, and 2261 flips per second. The optimum reached was
       13 falsified clauses, which seems very good. </li>
       <li> noimprove = 3 000 000: 100 rounds, maximum 12 600 000 steps,
       average 5 000 000 steps, 2294 flips per second, minimum 12 falsified
       clauses (seed 1833218426) </li>
       <li> One round, cutoff = 20 000 000, with previous best seed: didn't
       improve the result. </li>
       <li> So let's try cutoff = 20 000 000, noimprove = 3 000 000.
       \verbatim
> ubcsat-okl -alg samd -cutoff 20000000 -noimprove 3000000 -runs 100 -i Ramsey_5_5_2_41.cnf
       Clauses = 1498796
Variables = 820
TotalLiterals = 14987960
FlipsPerSecond = 552
BestStep_Mean = 2271951.410000
Steps_Mean = 5271952.410000
Steps_Max = 14351275.000000
PercentSuccess = 0.00
BestSolution_Mean = 54.470000
BestSolution_Median = 56.000000
BestSolution_Min = 1.000000
BestSolution_Max = 70.000000
       \endverbatim
       (the best solution was found with the maximum number of steps).
       </li>
       <li> Removing the noimprove-value, actually a solution was found:
       \verbatim
> ubcsat-okl -alg samd -cutoff 20000000 -runs 100 -i Ramsey_5_5_2_41.cnf
      1 0    54     387633   20000000  451534902
      2 0    55    4031001   20000000 2277530380
      3 1     0   17699250   17699250  147704963
       \endverbatim
       Was this pure chance? (By the way, that was using ubcsat-1.1.0,
       and we should check whether the seed is the same for 1.0.0.) </li>
      </ol>
     </li>
     <li> n=42
      <ol>
       <li> On cscharon (64 bit, ubcsat version 1.1.0):
       \verbatim
> ubcsat-okl -alg samd -cutoff 20000000 -runs 100 -i Ramsey_5_5_2_42.cnf
       \endverbatim
       on 7 runs minimum was 77, maximum 86 falsified clauses.
       </li>
       <li> 500 runs with a cutoff 100000 yields between 83 and 107 falsified
       clauses: Initially it's very easy, but then improvement is very hard,
       and only very few runs are better --- but then they are much
       better, while the rest is very close together. We need better data:
        <ol>
         <li> cutoff = 1000, 5000 runs:
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 1938
BestStep_Mean = 875.772200
Steps_Mean = 1000.000000
Steps_Max = 1000.000000
PercentSuccess = 0.00
BestSolution_Mean = 130.212200
BestSolution_Median = 130.000000
BestSolution_Min = 108.000000
BestSolution_Max = 160.000000
         \endverbatim
         </li> cutoff = 10000, 5000 runs: (cs-wsok)
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 2276
BestStep_Mean = 7167.124200
Steps_Mean = 10000.000000
Steps_Max = 10000.000000
PercentSuccess = 0.00
BestSolution_Mean = 106.278800
BestSolution_Median = 106.000000
BestSolution_Min = 85.000000
BestSolution_Max = 126.000000
         \endverbatim
         </li>
         <li> Alternatively with gsat-tabu:
         \verbatim
> ubcsat-okl -alg gsat-tabu -i Ramsey_5_5_2_42.cnf -runs 5000 -cutoff 10000
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 1721
BestStep_Mean = 7232.085000
Steps_Mean = 10000.000000
Steps_Max = 10000.000000
PercentSuccess = 0.00
BestSolution_Mean = 105.163400
BestSolution_Median = 105.000000
BestSolution_Min = 85.000000
BestSolution_Max = 124.000000
         \endverbatim
         </li>
         Might be slightly better, but seems to be quite a bit slower. </li>
         <li> cutoff = 100,000, 5000 runs: (cs-wsok)
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 2010
BestStep_Mean = 59262.030800
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 94.735400
BestSolution_Median = 95.000000
BestSolution_Min = 82.000000
BestSolution_Max = 111.000000
         \endverbatim
         only a bit progress.
         </li>
         <li> Alternatively with gsat-tabu:
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 1646
BestStep_Mean = 59331.269400
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 93.784200
BestSolution_Median = 94.000000
BestSolution_Min = 75.000000
BestSolution_Max = 143.000000
         \endverbatim
         might have a higher variability? </li>
         <li> cutoff = 1,000,000, 500 runs: (cscarme)
         \verbatim
Clauses = 1701336
Variables = 861
TotalLiterals = 17013360
FlipsPerSecond = 2135
BestStep_Mean = 477504.9
Steps_Mean = 1000000
/compsci/saturn/staff/csoliver/OKplatform/bin/ubcsat-okl: line 12: 26989 Segmentation fault
         \endverbatim
         where the data was then analysed using R, copying the output to
         a file "ExpRamsey1000000" starting like
         \verbatim
sat min osteps msteps seed
      1 0    90     153638    1000000 1649159937
      2 0    92     284850    1000000 1886480441
      3 0    86     636866    1000000 2139335150
         \endverbatim
         and where then analysis is very simple, %e.g.
         \verbatim
R1 = read.table("ExpRamsey1000000")
> min(R1$min)
[1] 77
> max(R1$min)
[1] 106
> median(R1$min)
[1] 88
> mean(R1$min)
[1] 87.8
         \endverbatim
         we see, again very little progress.
         </li>
         <li> cutoff = 10,000,000, 500 runs (cs-wsok) </li>
         <li> cutoff = 40,000,000, 50 runs (cscharon) </li>
        </ol>
       </li>
      </ol>
     </li>
     <li> n=43:
      <ol>
       <li> Also if this is unsatisfiable, one would only expect gradual
       differences compared to n=42 for local-search algorithms. </li>
       <li> cutoff = 10,000,000, 50 runs (csoberon)
       \verbatim
Clauses = 1925196
Variables = 903
TotalLiterals = 19251960
FlipsPerSecond = 1947
BestStep_Mean = 4888045.6
Steps_Mean = 10000000
/compsci/saturn/staff/csoliver/OKplatform/bin/ubcsat-okl: line 12:  9651 Segmentation fault
       \endverbatim
       and furthermore
       \verbatim
> min(R2$min)
[1] 112
> max(R2$min)
[1] 126
> median(R2$min)
[1] 117.5
> mean(R2$min)
[1] 117.38
       \endverbatim
       </li>
       <li> cutoff = 100,000,000, 50 runs (csoberon) </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>
 

*/

