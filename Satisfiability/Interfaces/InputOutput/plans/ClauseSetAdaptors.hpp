// Oliver Kullmann, 6.6.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/ClauseSetAdaptors.hpp
  \brief Plans for adaptors (transferring clause-sets in streams to data structures)


  \todo Need "ReverseDimacs" application and clause-set adaptor
  <ul>
   <li> When dealing with clause-sets which have some ordering,
   it is sometimes useful to reverse this ordering (perhaps
   whatever we ordered the clause-set with doesn't support the
   inverse order that we truly want). </li>
   <li> Therefore we need a simple application and clause-set 
   adaptor which reverses the given clause-set. </li>
  </ul>


  \todo Add MinOnes2WCNF output clause-set adaptor
  <ul>
   <li> We need to translate problems involving finding satisfying 
   assingments for CNF formulas minimising the number of variables set to
   one, into weighted MaxSAT problems to allow the use of
   MaxSAT solvers to solve such problems. We call such problems MinOnes. </li>
   <li> See "Prepare benchmarks for SAT 2011" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Benchmarks.hpp.
   </li>
   <li> Weighted partial MaxSAT CNF Dimacs files take the form of a standard
   Dimacs where each clause is prefixed with an integer weight, and the solver
   must find an assignment which maximises the sum of the weights of the 
   satisfied clauses. See http://maxsat.ia.udl.cat/requirements/. </li>
   <li> To translate the problem of MinOnes for some clause-set F into a
   weighted MaxSAT problem, we construct a partial weighted MaxSAT
   problem G, which:
   <ul>
    <li> Includes the clauses from F, and a unit clause for every variable in
    F, containing the negation of the variable. </li>
    <li> The clauses from F are given a high weight, greater than the number
    of variables. </li>
    <li> The unit clauses are given weight 1. </li>
   </ul>
   Assuming the original CNF is satisfiable, the optimum solutions to this
   weighted MaxSAT problem are those satisfying assignments on F with
   the minimum number of variables assigned to true/one. </li>
   </li>
   <li> One way to provide such a translation is to write a output clause-set
   adaptor based on CLSAdaptorDIMACSOutput, which outputs a weighted MaxSAT
   translation of the MinOnes problem for the input clause-set. </li>
  </ul>


  \todo Document the basic concept CLSAdaptor
  <ul>
   <li> See Interfaces/InputOutput/ClauseSetAdaptors.hpp. </li>
  </ul>


  \todo Discussing the basic concept CLSAdaptor
  <ul>
   <li> Perhaps the adaptor should perform cleaning-up the input (regarding
   tautological clauses and multiple literal occurrences) ? </li>
   <li> But perhaps this is better done by some other component, while the
   adaptor is only responsible for the data transfer. </li>
  </ul>


  \todo Write concept for InputOutput::Statistics class
  <ul>
   <li> See Interfaces/InputOutput/ClauseSetAdaptors.hpp. </li>
   <li> InputOutput::FullStatistics:
    <ol>
     <li> Should this fall under the same concept? </li>
     <li> Or should here the design pose more restrictions on the use
     (becoming a "proper class", not just a POD). </li>
    </ol>
   </li>
  </ul>


  \todo Write tests!


  \todo CLSAdaptorFullStatistics
  <ul>
   <li> Perhaps it is better that InputOutput::FullStatistics uses std::map
   instead of std::vector. </li>
   <li> Should there be a more direct relation to
   InputOutput::CLSAdaptorStatistics ? </li>
  </ul>


  \todo RawDimacsCLSAdaptor
  <ul>
   <li> What are the precise assumptions on template parameter Lit? </li>
   <li> Handling of the additional information: Perhaps we have
   (optionally?) an embedded object of type CLSAdaptorStatistics ?! </li>
   <li> Should there be policies for checking n and c (or should this
   go into a more complex adaptor?)? </li>
  </ul>


  \todo Create "base class" for "raw adaptors"
  <ul>
   <li> Yet we have InputOutput::RawDimacsCLSAdaptor and
   InputOutput::RawDimacsCLSAdaptorSets, which are very similar. </li>
   <li> Creating std::vector<std::vector<int>> or std::set<std::set<int>>
   only differs in the one line where a clause is transferred ("insert"
   instead of "push_back"). </li>
  </ul>

*/

