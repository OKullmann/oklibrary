// Oliver Kullmann, 6.5.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/RamseyTheory/plans/GreenTaoProblems.hpp
  \brief Plans on the module on computing Green-Tao numbers (and related quantities)


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/GreenTaoProblems.hpp
   and
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/GreenTaoProblems.mac.
   </li>
   <li> See
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp.
   </li>
   <li> See the various generators in Satisfiability/Transformers/Generators/
   (they should all be moved to here). </li>
  </ul>


  \todo Generator for Green-Tao problems
  <ul>
   <li> DONE (see OKlib/Combinatorics/Hypergraphs/Generators/GreenTao.cpp)
   We need to write a program with functionality like arithprog_primes_hg
   from the Maxima/Lisp level (and all the other functions there in
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/GreenTaoProblems.mac).
   </li>
   <li> See
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp
   for experiments. </li>
   <li> Best we provide this functionality as Unix tools as well as at
   library level. </li>
   <li> DONE
   Perhaps this hypergraph creation happens in
   OKlib/Combinatorics/Hypergraphs, while here we plug these
   hypergraphs together to get a clause-set. </li>
   <li> DONE
   The hypergraph can be output as a positive clause-set,
   in DIMACS format, into a file. </li>
   <li> From OKlib/Satisfiability/Transformers we get the tools
   for standardising the names.
    <ol>
     <li> On the other hand, there should be a standard mapping from
     the original variables to natural numbers, so no general tools
     should be used here. </li>
     <li> This problem needs to be fixed at Maxima/Lisp level, and here
     supported. </li>
     <li> For boolean problems there aren't any choices (except of the
     symmetry-breaking unit-clause). </li>
     <li> But for the non-boolean problems we might consider different
     translations from non-boolean to boolean problems. </li>
     <li> And the special case of progression-length 2 could be handled
     differently. </li>
    </ol>
   </li>
   <li> Perhaps the number-theoretical tools should go to
   OKlib/Structures/NumberTheory.
    <ol>
     <li> Enumerating the primes:
      <ol>
       <li> Gmp has a "mpz_nextprime" function, by which we first create the
       complete list of primes and the corresponding boolean array (for the
       primality predicate). </li>
       <li> Optionally, if one of the randomised tests is not secure, then the
       whole computation is checked via a simple sieve of Erathostenes. </li>
       <li> But we should ask the Gmp-people(?), up to which prime number
       the results are known to be correct --- likely this is big enough! </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Also important to be able to compute the various statistics on
   arithmetical progressions of primes (without creating hypergraphs or
   clause-sets); see below. </li>
  </ul>


  \todo Statistics on the number of arithmetic progressions
  <ul>
   <li> A simple application is needed, which for inputs k, n prints in
   R-format for i from 1 to n the sizes of the hypergraphs for (k,i). </li>
   <li> This should be based on the hypergraph computed by
   Generators::GreenTao, to which then a function as sizes_cstrata_indmon_ohg
   is applied. </li>
   <li> So two range-transformations are to be provided, specified by
   sizes_strata_indmon_ohg and accumulate_l (in
   ComputerAlgebra/Hypergraphs/Lisp/Stratification.mac). </li>
   <li> Perhaps calling this application "CountProgressions_GreenTao". </li>
  </ul>


  \todo Computing minimum transversals
  <ul>
   <li> DONE
   Similar to
   Applications/RamseyTheory/plans/MinimumTransversals_VanderWaerden.cpp we
   need MinimumTransversals_GreenTao.cpp. </li>
   <li> Application tests are needed for
   Applications/RamseyTheory/plans/MinimumTransversals_GreenTao.cpp. </li>
   <li> The problem is that currently we cannot compile this application
   automatically, due to the build dependencies not expressible, and thus
   yet we cannot create application tests. </li>
  </ul>

*/
