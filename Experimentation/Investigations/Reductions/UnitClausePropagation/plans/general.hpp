// Oliver Kullmann, 24.11.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Reductions/UnitClausePropagation/plans/general.hpp
  \brief General plans on investigations into UCP


  \todo Connections
  <ul>
   <li> See Satisfiability/Reductions/UnitClausePropagation/plans/general.hpp
   for general plans on UCP at C++ level. </li>
   <li> Currently our own implementations are
   Satisfiability/Reductions/UnitClausePropagation/UnitClausePropagation.cpp
   and
   Satisfiability/Reductions/UnitClausePropagation/UnitClausePropagationW.cpp.
   </li>
   <li> These are discussed in
   Satisfiability/Reductions/UnitClausePropagation/plans/UnitClausePropagation.hpp.
   </li>
  </ul>


  \todo Experimentation tools
  <ul>
   <li> Experiments on 32-bit machines as well as on 64-bit machines. </li>
   <li> We need some tools for time-measurement, recording and evaluation
   (the last of course by R) of applications. </li>
   <li> Perhaps in this context we concentrate on "one-off" applications of
   UCP, that is, just reading a file, computing the simplification, and
   outputting it (as in UnitClausePropagation.cpp). </li>
   <li> Then we don't need to worry about reversing information stored in
   clauses (since pure UCP itself does not involve any form of backtracking).
   </li>
  </ul>


  \todo Saturated Horn clause-sets
  <ul>
   <li> Generation by
   \verbatim
k=5000; Smusat_Horn-O3-DNDEBUG ${k} > Smusat_Horn_${k}.cnf
   \endverbatim
   </li>
   <li> For k=20000 the file is 1229 MB. </li>
   <li> Also of relevance is to reverse the clause-order:
   \verbatim
tac Smusat_Horn_20000.cnf > Smusat_Horn_R_20000.cnf
   \endverbatim
   </li>
   <li> However this simple file reversal puts the comment- and parameter-
   section at the end of the file. The following repairs this (assuming
   that the comment- and parameter section spans 2 lines):
   \verbatim
k=1000; head -n 2 Smusat_Horn_${k}.cnf > Smusat_Horn_R_${k}.cnf; tail -n +3 Smusat_Horn_${k}.cnf | tac >> Smusat_Horn_R_${k}.cnf
   \endverbatim
   </li>
   <li> Sets versus vectors in UnitClausePropagation.cpp:
    <ol>
     <li> It seems that the set-version is definitely worse, time- and
     space-wise, than the vector-version (see below to compare with the
     vector-version):
     \verbatim
> time cat Smusat_Horn_k.cnf | UnitClausePropagation-O3-DNDEBUG set
n=1000: ~ 1.9s
n=2000: ~ 13.3s
n=4000: ~ 67s
     \endverbatim
     </li>
     <li> Growth for the vector-version:
     \verbatim
> time cat Smusat_Horn_k.cnf | UnitClausePropagation-O3-DNDEBUG
n=1000: ~ 0.8s
n=2000: ~ 3.5s
n=4000: ~ 15.0s
     \endverbatim
     </li>
     <li> All the above times are the same for the reversed versions
     (actually, the reversed versions seem to run a bit faster). </li>
     <li> That for small n it's slower than OKsolver_2002 seems to suggest that
     parsing in UnitClausePropagation is less efficient. </li>
    </ol>
   </li>
   <li> The OKsolver_2002-O3-DNDEBUG is definitely slower than the
   vector-version, but it uses somewhat less memory.
    <ol>
     <li> Growth (running times are the same for Smusat_Horn_n.cnf as for
     Smusat_Horn_R_n.cnf):
     \verbatim
n=1000: ~ 0.55s
n=2000: ~ 4.1s
n=4000: ~ 30.0s
     \endverbatim
     </li>
     <li> Looks worse than quadratic growth, so apparently the OKsolver_2002
     ucp-preprocessing is worse than linear time. </li>
    </ol>
   </li>
   <li> minisat2 is much faster (but slowest for the reverse clause-order):
    <ol>
     <li> First for the standard order:
     \verbatim
n=1000: ~ 0.05s
n=2000: ~ 0.2s
n=4000: ~ 0.85s
     \endverbatim
     </li>
     <li> For k=20000 on csltok it takes only 26 seconds for the whole
     computation, and also the memory usage is much smaller --
     just around 5 MB. </li>
     <li> So minisat2 performs UCP at the same time when reading the input.
     </li>
     <li> This wouldn't be hard to do ourselves; on such cases we will see
     a large speed-up, while hopefully for example on cases with no
     unit-clauses in the input the slow-down is negligible. </li>
     <li> However, with the reverse file minisat2 takes much longer, and uses
     much more memory:
     \verbatim
n=1000: ~ 2.1s
n=2000: ~ 28.4s
n=4000: ~ 230s
     \endverbatim
     Smusat_Horn_R_20000.cnf needed up to 2GB, and was aborted. </li>
     <li> Something desastreous must happen here. </li>
     <li> In both versions parsing time is everything. </li>
    </ol>
   </li>
   <li> satz215 creates a core dump (memory access error) for k=20000. </li>
   <li> march_pl for k=20000 needs about 120 s for parsing, and uses about 800
   MB (that's not too bad, quite a bit less than OKsolver_2002). However
   then aborted after more than 3 hours (without succeeding). </li>
   <li> So there seems to be a lot to gain. Since we have very long
   clauses here, the advantage of using just two watched literals per
   clause becomes very big. </li>
   <li> Perhaps we should use a memory pool; and using only vectors should
   minimise memory usage. </li>
   <li> And, of course, we need to experiment with watched literals; perhaps
   making an exception for binary clauses, where we can simplify the
   implementation. See "Implement initial UCP based on head-tail clauses" in
   Reductions/UnitClausePropagation/plans/general.hpp. </li>
   <li> Now considering UnitClausePropagationW.cpp:
    <ol>
     <li> Growth of running time (csltok):
     \verbatim
n=1000: ~ 0.68s
n=2000: ~ 2.87s
n=4000: ~ 12.2s
     \endverbatim
     This is basically the same for standard and reversed clause-order (again,
     it seems that the reverse order is processed slightly faster). </li>
     <li> If one could improve parsing time, and also perform ucp at the
     time of reading without impairing running times (substantially) in cases
     without units in the input, then this should be fastest (compared to all
     competitors above). </li>
    </ol>
   </li>
  </ul>


  \todo Saturated minimally unsatisfiable clause-sets of deficiency 2
  <ul>
   <li> We need also test-cases with small clause-sizes. Perhaps the Musat(2)
   cases with one additional unit-clause (then most of the clauses are
   binary, and we just have two full clauses). </li>
   <li> Generator at Maxima-level:
   \verbatim
musatd2p1_fcl(n) := block([FF:musatd2_fcl(n)], [FF[1], endcons({1},FF[2])]);
output_musatd2p1(n) := outputext_fcl(sconcat("SMU(2) with ",n," variables, plus one unit-clause."), musatd2p1_fcl(n), sconcat("Smusatd2p1_",n,".cnf"))$
   \endverbatim
   Just writing to file is very slow. </li>
   <li> The same at C++ level:
   \verbatim
n=5; Musatd2-O3-DNDEBUG ${n} 1 > Musatd2p1_${n}.cnf
   \endverbatim
   </li>
   <li> Sets versus vectors in UnitClausePropagation.cpp:
    <ol>
     <li> Growth:
     \verbatim
> time cat Musatd2p1_n.cnf | UnitClausePropagation-O3-DNDEBUG
n=500000: 5.78s
n=1000000: 11.8s
n=2000000: 24.6s

> time cat Musatd2p1_n.cnf | UnitClausePropagation-O3-DNDEBUG set
n=500000: 6.86s
n=1000000: 13.8s
n=2000000: 31s
     \endverbatim
     </li>
    </ol>
   </li>
   <li> OKsolver_2002 is extremely slow; what's going on here?
    <ol>
     <li> Growth:
     \verbatim
> time OKsolver_2002-O3-DNDEBUG Musatd2p1_n.cnf
n=50000: 7.1s
n=100000: 30s
n=200000: 132s
     \endverbatim
     </li>
     <li> That's definitely worse than linear (roughly quadratic); again one
     needs to investigate the preprocessing phase. </li>
    </ol>
   </li>
   <li> minisat2 is also very slow (5200 s, n=10^6); again, why is this?
    <ol>
     <li> Would the (single) unit-clause not be located as *last* clause, then
     likely the times would be much faster (though possibly misleading). </li>
     <li> As in the above disaster (using reverse order there), all time is
     spent in parsing. </li>
     <li> Growth:
     \verbatim
> time minisat2 Musatd2p1_n.cnf
n=50000: 7.9s
n=100000: 32s
n=200000: 190s
     \endverbatim
     </li>
     <li> This time even worse than quadratic growth? </li>
    </ol>
   </li>
   <li> Now considering UnitClausePropagationW.cpp:
    <ol>
     <li> Growth:
     \verbatim
> time cat Musatd2p1_n.cnf | UnitClausePropagationW-O3-DNDEBUG
n=500000: 5.1s
n=1000000: 10.3s
n=2000000: 21.5s
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Further test-cases
  <ul>
   <li> Horn clause-sets:
    <ol>
     <li> See "Pebbling contradictions" in
     ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/DeficiencyOne.hpp
     for special Horn clause-sets in MU(1). </li>
     <li> We should consider also all other Horn-elements of MU(1) (see above
     for the saturated cases). </li>
     <li> Likely we should also consider Horn clause-sets which are not in MU.
     </li>
    </ol>
   </li>
   <li> We should also consider random clause-sets with some added
   unit-clause(s). </li>
   <li> Sudoku puzzles can also be used. </li>
  </ul>

*/
