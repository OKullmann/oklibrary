// Matthew Gwynne, 5.10.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
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


  \todo Write application tests


  \todo Preparations for computing optimum representations
  <ul>
   <li> Optionally the necessary prime clauses should be output (in Dimacs
   format). </li>
   <li> Also optionally, the hypergraph as computed by subsumption_ghg
   (see
   ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac
   ) should be created (the minimum transversals of this hypergraph yield the
   minimum representations), in strict Dimacs format, where the meaning of
   variables is explained in the comments. </li>
   <li> The ordering of the prime clauses likely should be lexicographical.
   </li>
   <li> For that hypergraph, the unit-clause-propagation should have
   been performed already (just involving the necessary clauses, which just
   correspond to unit-hyperedges).
    <ol>
     <li> See rsubsumption_hg_full_cs(F) in
     ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac
     for the Maxima-specification. </li>
    </ol>
   </li>
  </ul>




*/
