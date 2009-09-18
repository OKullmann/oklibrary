// Matthew Gwynne, 25.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/Ramsey_2_3_10.hpp
  \brief On investigations into %Ramsey problems of the form ramsey_2^2(3,10)


  \todo General information
  <ul>
   <li> Known is 40 <= ramsey_2^2(3,10) <= 43. </li>
   <li> So we get at most 1917347124 clauses. </li>
   <li> This is likely too many clauses for most solvers but perhaps
   some local search solvers might be able to manage something? </li>
   <li> To generate an instance of SAT problem specifying 
   "ramsey_2^2(3,10) > n?" use the following:
   \verbatim
Ramsey-O3-DNDEBUG 3 10 2 6 | ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_3_10_2_6.cnf
   \endverbatim
   where n has been replaced by 6 in this example.
   </li>
   <li> Also, we can at least gauge the performance of DPLL-like solvers
   on the satisfiable instances of the problems, and see whether symmetry
   breaking techniques improve matters. </li>
   <li> To generate the additional clauses for each of the symmetry 
   breaking techniques (see 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/RamseyProblems.hpp)
   and cardinality constraints, run the following in maxima (generating
   each of the additional clauses for a range of n values):
   <ul>
    <li> Symmetry breaking by fixing colours/labels to break monochromatic
    k-subsets:
    \verbatim
for n : 5 thru 43 do output_ramsey2_symbr1_stdname(3,n);
    \endverbatim
    </li>
    <li> Symmetry breaking by fixing monochromatic cliques:
    \verbatim
for n : 5 thru 43 do output_ramsey2_symbr2_stdname(n);
    \endverbatim
    </li>
    <li> Symmetry breaking by recursive application of the pigeon hole
    principle:
    \verbatim
for n : 5 thru 43 do output_ramsey2_symbr3_stdname(n);
    \endverbatim
    </li>
    <li> Note here that adding cardinality constraints as suggested in MGs
    conjecture in "Considering only labellings with bounded number of particular
    colours" in 
    ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/RamseyProblems.hpp 
    isn't appropriate here, as the conjecture only applies to diagonal 
    instances. </li>
   </ul>
   </li>
  </ul>


  \todo Using DPLL and Conflict driven solvers
  <ul>
   <li> A systematic investigation, involving the applicable symmetry breaking
   techniques (see 
   Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/SymmetryBreaking.hpp)
   can be generated and run using the experiment system discussed at "General 
   experiment system" in 
   Experimentation/Investigations/Cryptography/AES/plans/BreakingAES.hpp), with
   the following "generate_exp.sh" script:
   \verbatim
# Generates Ramsey experiments for "ramsey_2^2(3,10) > n?" for 5 <= n <= 43.


for n in `seq 5 43`; do
# No Symmetry breaking
    echo "PICOSAT_"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_3_10_2_${n}.cnf && ./solvers/picosat913 -v Ramsey_exp.cnf > Ramsey_3_10_2_${n}.cnf.result.picosat 2>&1 && rm Ramsey_3_10_2_${n}.cnf" >> experiments;
    echo "MINISAT2_"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_3_10_2_${n}.cnf && ./solvers/minisat2 Ramsey_3_10_2_${n}.cnf minisat-temp > Ramsey_3_10_2_${n}.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_3_10_2_${n}.cnf.result.minisat2;rm Ramsey_3_10_2_${n}.cnf" >> experiments;
    echo "MARCH_PL_"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_3_10_2_${n}.cnf && ./solvers/march_pl Ramsey_3_10_2_${n}.cnf > Ramsey_3_10_2_${n}.cnf.result.march_pl 2>&1;rm Ramsey_3_10_2_${n}.cnf" >> experiments;
    echo "OKSOLVER_"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_3_10_2_${n}.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_3_10_2_${n}.cnf > Ramsey_3_10_2_${n}.cnf.result.OKsolver 2>&1; mv Ramsey_3_10_2_${n}.cnf.mo Ramsey_3_10_2_${n}.cnf.mo.OKsolver;mv Ramsey_3_10_2_${n}.cnf.pa Ramsey_3_10_2_${n}.cnf.pa.OKsolver;rm Ramsey_3_10_2_${n}.cnf" >> experiments;
    echo "OKSOLVERM2PP_"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_3_10_2_${n}.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_3_10_2_${n}.cnf > Ramsey_3_10_2_${n}.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_3_10_2_${n}.cnf_m2pp_*.mo Ramsey_3_10_2_${n}.cnf.mo.OKsolver-m2pp;mv Ramsey_3_10_2_${n}.cnf_m2pp_*.pa Ramsey_3_10_2_${n}.cnf.pa.OKsolver-m2pp;rm Ramsey_3_10_2_${n}.cnf" >> experiments;
# Symmetry breaking 1
   echo "PICOSAT_FC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_FC_3_${n}.ecnf > Ramsey_3_10_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_FC.ecnf > Ramsey_3_10_2_${n}_SB_FC.cnf && ./solvers/picosat913 -v Ramsey_exp.cnf > Ramsey_3_10_2_${n}_SB_FC.cnf.result.picosat 2>&1 && rm -f Ramsey_3_10_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
    echo "MINISAT2_FC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_FC_3_${n}.ecnf > Ramsey_3_10_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_FC.ecnf > Ramsey_3_10_2_${n}_SB_FC.cnf && ./solvers/minisat2 Ramsey_3_10_2_${n}_SB_FC.cnf minisat-temp > Ramsey_3_10_2_${n}_SB_FC.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_3_10_2_${n}_SB_FC.cnf.result.minisat2;rm -f Ramsey_3_10_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
  echo "MARCH_PL_FC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_FC_3_${n}.ecnf > Ramsey_3_10_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_FC.ecnf > Ramsey_3_10_2_${n}_SB_FC.cnf && ./solvers/march_pl Ramsey_3_10_2_${n}_SB_FC.cnf > Ramsey_3_10_2_${n}_SB_FC.cnf.result.march_pl 2>&1;rm -f Ramsey_3_10_2_${n}{_SB_FC,}.{e,}cnf" >> experiments;
   echo "OKSOLVER_FC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_FC_3_${n}.ecnf > Ramsey_3_10_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_FC.ecnf > Ramsey_3_10_2_${n}_SB_FC.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_3_10_2_${n}_SB_FC.cnf > Ramsey_3_10_2_${n}_SB_FC.cnf.result.OKsolver 2>&1; mv Ramsey_3_10_2_${n}_SB_FC.cnf.mo Ramsey_3_10_2_${n}_SB_FC.cnf.mo.OKsolver;mv Ramsey_3_10_2_${n}_SB_FC.cnf.pa Ramsey_3_10_2_${n}_SB_FC.cnf.pa.OKsolver;rm -f Ramsey_3_10_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_FC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_FC_3_${n}.ecnf > Ramsey_3_10_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_FC.ecnf > Ramsey_3_10_2_${n}_SB_FC.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_3_10_2_${n}_SB_FC.cnf > Ramsey_3_10_2_${n}_SB_FC.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_3_10_2_${n}_SB_FC.cnf_m2pp_*.mo Ramsey_3_10_2_${n}_SB_FC.cnf.mo.OKsolver-m2pp;mv Ramsey_3_10_2_${n}_SB_FC.cnf_m2pp_*.pa Ramsey_3_10_2_${n}_SB_FC.cnf.pa.OKsolver-m2pp;rm -f Ramsey_3_10_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
# Symmetry breaking 2
   echo "PICOSAT_MC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_3_10_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_MC.ecnf > Ramsey_3_10_2_${n}_SB_MC.cnf && ./solvers/picosat913 -v Ramsey_3_10_2_${n}_SB_MC.cnf > Ramsey_3_10_2_${n}_SB_MC.cnf.result.picosat 2>&1 && rm -f Ramsey_3_10_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
    echo "MINISAT2_MC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_3_10_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_MC.ecnf > Ramsey_3_10_2_${n}_SB_MC.cnf && ./solvers/minisat2 Ramsey_3_10_2_${n}_SB_MC.cnf minisat-temp > Ramsey_3_10_2_${n}_SB_MC.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_3_10_2_${n}_SB_MC.cnf.result.minisat2;rm -f Ramsey_3_10_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
    echo "MARCH_PL_MC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_3_10_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_MC.ecnf > Ramsey_3_10_2_${n}_SB_MC.cnf && ./solvers/march_pl Ramsey_3_10_2_${n}_SB_MC.cnf > Ramsey_3_10_2_${n}_SB_MC.cnf.result.march_pl 2>&1;rm -f Ramsey_3_10_2_${n}{_SB_MC,}.{e,}cnf" >> experiments;
    echo "OKSOLVER_MC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_3_10_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_MC.ecnf > Ramsey_3_10_2_${n}_SB_MC.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_3_10_2_${n}_SB_MC.cnf > Ramsey_3_10_2_${n}_SB_MC.cnf.result.OKsolver 2>&1; mv Ramsey_3_10_2_${n}_SB_MC.cnf.mo Ramsey_3_10_2_${n}_SB_MC.cnf.mo.OKsolver;mv Ramsey_3_10_2_${n}_SB_MC.cnf.pa Ramsey_3_10_2_${n}_SB_MC.cnf.pa.OKsolver;rm -f Ramsey_3_10_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_MC"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_3_10_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_MC.ecnf > Ramsey_3_10_2_${n}_SB_MC.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_3_10_2_${n}_SB_MC.cnf > Ramsey_3_10_2_${n}_SB_MC.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_3_10_2_${n}_SB_MC.cnf_m2pp_*.mo Ramsey_3_10_2_${n}_SB_MC.cnf.mo.OKsolver-m2pp;mv Ramsey_3_10_2_${n}_SB_MC.cnf_m2pp_*.pa Ramsey_3_10_2_${n}_SB_MC.cnf.pa.OKsolver-m2pp;rm -f Ramsey_3_10_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
# Symmetry breaking 3
   echo "PICOSAT_PHP"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_3_10_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_PHP.ecnf > Ramsey_3_10_2_${n}_SB_PHP.cnf && ./solvers/picosat913 -v Ramsey_3_10_2_${n}_SB_PHP.cnf > Ramsey_3_10_2_${n}_SB_PHP.cnf.result.picosat 2>&1 && rm -f Ramsey_3_10_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
    echo "MINISAT2_PHP"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_3_10_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_PHP.ecnf > Ramsey_3_10_2_${n}_SB_PHP.cnf && ./solvers/minisat2 Ramsey_3_10_2_${n}_SB_PHP.cnf minisat-temp > Ramsey_3_10_2_${n}_SB_PHP.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_3_10_2_${n}_SB_PHP.cnf.result.minisat2;rm -f Ramsey_3_10_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
    echo "MARCH_PL_PHP"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_3_10_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_PHP.ecnf > Ramsey_3_10_2_${n}_SB_PHP.cnf && ./solvers/march_pl Ramsey_3_10_2_${n}_SB_PHP.cnf > Ramsey_3_10_2_${n}_SB_PHP.cnf.result.march_pl 2>&1;rm -f Ramsey_3_10_2_${n}{_SB_PHP,}.{e,}cnf" >> experiments;
    echo "OKSOLVER_PHP"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_3_10_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_PHP.ecnf > Ramsey_3_10_2_${n}_SB_PHP.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_3_10_2_${n}_SB_PHP.cnf > Ramsey_3_10_2_${n}_SB_PHP.cnf.result.OKsolver 2>&1; mv Ramsey_3_10_2_${n}_SB_PHP.cnf.mo Ramsey_3_10_2_${n}_SB_PHP.cnf.mo.OKsolver;mv Ramsey_3_10_2_${n}_SB_PHP.cnf.pa Ramsey_3_10_2_${n}_SB_PHP.cnf.pa.OKsolver;rm -f Ramsey_3_10_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_PHP"$n "./Ramsey-O3-DNDEBUG 3 10 2 ${n} > Ramsey_3_10_2_${n}.ecnf && ./merge_cnf.sh Ramsey_3_10_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_3_10_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_3_10_2_${n}_SB_PHP.ecnf > Ramsey_3_10_2_${n}_SB_PHP.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_3_10_2_${n}_SB_PHP.cnf > Ramsey_3_10_2_${n}_SB_PHP.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_3_10_2_${n}_SB_PHP.cnf_m2pp_*.mo Ramsey_3_10_2_${n}_SB_PHP.cnf.mo.OKsolver-m2pp;mv Ramsey_3_10_2_${n}_SB_PHP.cnf_m2pp_*.pa Ramsey_3_10_2_${n}_SB_PHP.cnf.pa.OKsolver-m2pp;rm -f Ramsey_3_10_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
done
   \endverbatim
   </li>
   <li> The above experiment (in the generation script) is currently running on
   cselara with a timeout of 2 hours. </li>
   <li> Combining all such investigations (different symmetry breaking 
   techniques, cardinality contraints etc) into a single experiment allows one
   to run through all such investigations for a short time and then split the
   experiments if certain aspects are slowing the entire experiment set down
   or it makes more sense to run certain parts, or solvers in parallel. </li>
  </ul>
 

*/

