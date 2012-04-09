// Matthew Gwynne, 5.10.2010 (Swansea)
/* Copyright 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskeySubsumptionHypergraph.hpp
  \brief Plans regarding the direct implementation of the computation of the subsumption hypergraph for computing minimal representations of full boolean clause-sets


  \todo Specification
  <ul>
   <li> A precise specification of QuineMcCluskeySubsumptionHypergraph.cpp is
   needed. </li>
   <li> Of course, with precise recourse to the Maxima specification. </li>
  </ul>


  \todo Tests for statistics levels
  <ul>
   <li> QuineMcCluskeySubsumptionHypergraphWithFullStatistics and
   QuineMcCluskeySubsumptionHypergraphFullStatistics allow the options
   "ni", "n" and "f" as optional options, which set different handling
   of clause-lengths with zero occurrences in the statistics output. 
   </li>
   <li> At present we test only the default ("ni"), but not specifying
   the options. </li>
   <li> We need tests for all options. </li>
  </ul>


  \todo Preparations for computing optimum representations
  <ul>
   <li> Optionally the necessary prime clauses should be output (in Dimacs
   format). </li>
   <li> Also optionally, the hypergraph as computed by subsumption_ghg
   (see
   ComputerAlgebra/Satisfiability/Lisp/Primality/PrimeImplicatesImplicants.mac
   ) should be created (the minimum transversals of this hypergraph yield the
   minimum representations), in strict Dimacs format, where the meaning of
   variables is explained in the comments. </li>
   <li> For that hypergraph, the unit-clause-propagation should have
   been performed already (just involving the necessary clauses, which just
   correspond to unit-hyperedges).
    <ol>
     <li> See rsubsumption_hg_full_cs(F) in
     Satisfiability/Lisp/Primality/PrimeImplicatesImplicants.mac
     for the Maxima-specification. </li>
    </ol>
   </li>
   <li> DONE The ordering of the prime clauses likely should be 
   lexicographical. </li>
  </ul>


  \todo Improving efficiency
  <ul>
   <li> For the AES Sbox (created by output_rijnsbox_fullcnf_stdname()) the
   computation takes 7m36s on csltok. </li>
   <li> One needs a better understanding where this time is going. </li>
   <li> Hopefully speed can be improved. </li>
   <li> Another aspect is memory usage: it seems that short before the end
   of the computation an unnecessary duplication of the data structure takes
   place. </li>
  </ul>


  \bug DONE (tidied and specified output) Broken application tests
  <ul>
   <li> The tests do not output what is tested, by which script, and using
   which files. </li>
   <li> One also finds "AUXILLIARY OUTPUT", which is nonsense, since it is
   not the solver-output, neither it is "auxilliary"; and finally, not
   context is given. </li>
   <li> Thus the tests cannot be updated. </li>
  </ul>


  \todo DONE (see QuineMcCluskeySubsumptionHypergraphWithFullStatistics.cpp
  and QuineMcCluskeySubsumptionHypergraphFullStatistics.cpp)
  Add option to generate statistics files
  <ul>
   <li> A popular use case (see 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/Methods.hpp) 
   for this program is to examine the statistics for the prime implicates and 
   subsumption hypergraph. </li>
   <li> Currently one must output the various clause-sets and then
   read them into the statistics program, which given the size of
   the output is often very time consuming. </li>
   <li> Therefore we need options to:
   <ul>
    <li> generate the statistics for the prime implicates and
    subsumption hypergraph and output them to files, and also
    output the subsumption hypergraph (and prime implicates). </li>
    <li> generate just the statistics for the prime implicates and
    subsumption hypergraph. </li>
    <li> generate just the subsumption hypergraph (and prime implicates).
    </li>
   </ul>
   </li>
   <li> As in "Standardise name of prime implicates file", standard
   names should be used for these files. </li>
  </ul>


  \todo DONE Write application tests


  \todo DONE Standardise name of prime implicates file
  <ul>
   <li> Currently the prime implicates calculated during the computation
   of the subsumption hypergraph can optionally be output, if one specifies
   the filename to output them to as an argument. </li>
   <li> In general, it is better if we simply generate a file with a 
   standard name based on the input file, and then we do not have
   different names being used all over the library. </li>
  </ul>

*/
