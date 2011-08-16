
// Oliver Kullmann, 22.9.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Optimisation/plans/general.hpp
  \brief General plans on optimisation variants of (generalised) satisfiability


  \todo Update namespaces.


  \todo Improve minimisation scripts
  <ul>
   <li> The following scripts need improvement:
    <ul>
     <li> all_minimum_cnf:
      <ul>
       <li> Statistics on each of transversals should be computed and
       output to a "statistics" file, and also to standard output. </li>
       <li> The dependency of FilterDimacs on the order and variable naming
       scheme in QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG must be
       fully documented, or even better, removed. </li>
       <li> DONE An experiment directory must be created. </li>
       <li> DONE The computed CNFs should have the ".cnf" extension. </li>
      </ul>
     </li>
     <li> minimise_cnf_cryptominisat. </li>
     <li> minimise_cnf_precosat236. </li>
     <li> minimise_cnf_oksolver. </li>
    </ul>
   </li>
   <li> Tests are urgently required. </li>
   <li> Error checking code is urgently required. </li>
   <li> The use of "mktemp" must be removed from all scripts. Todos need to
   be created to discuss an OKlibrary version of "mktemp". </li>
   <li> All scripts need improved documentation and bash shell scripting. </li>
   <li> The scripts "cardinality_bounded_cnf" and
   "extend_strict_dimacs_with_extended" must be removed and their
   functionality rethought. </li>
   <li> A new sub-module "Minimisation" should also
   be created with appropriate milestones. </li>
   <li> All minimisation scripts must be linked in with the rest of the
   library:
    <ul>
     <li> See Combinatorics/Hypergraphs/Transversals/Minimal/plans/general.hpp.
     </li>
     <li> See "Espresso" in
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Methods.hpp.
     </li>
     <li> See "R QCA package" in
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Methods.hpp.
     </li>
     <li> See "Maxima functions" in
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Methods.hpp.
     </li>
     <li> See "Minimisation using Weighted MaxSAT" in
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Methods.hpp.
     </li>
    </ul>
   </li>
   <li> See also "Minimum CNF/DNF representations" in
   Investigations/BooleanFunctions/MinimumRepresentations/plans/general.hpp and
   "Overview" in
   Experimentation/Investigations/Transversals/Bounded/plans/general.hpp. </li>
  </ul>


  \todo Create module MinOnes (or "WeightedSAT" ?)


  \todo Create module MinSAT.
  <ul>
   <li> This is about finding a satisfying partial assignment
   using a minimal number of variables, a natural
   generalisation of finding a minimum size transversal
   of a hypergraph. </li>
   <li> The parameterised case is discussed in
   AllSolutions/plans/MinimalAssignments.hpp ---
   should this move to this new module? </li>
   <li> Or does this module actually belong to
   Satisfiability/Algorithms/AllSolutions ? </li>
   <li> It seems that the minimisation problem is different
   also from the parameterised version, and thus
   perhaps the two different modules are justifed:
   Though the minimisation problem can be solved by running
   through the parameter levels, there are many other
   possibilities (for example regarding approximation). </li>
   <li> So perhaps also for finding a transversal of minimum
   size we need a module? </li>
  </ul>


  \todo Create module MaxSat.

*/

