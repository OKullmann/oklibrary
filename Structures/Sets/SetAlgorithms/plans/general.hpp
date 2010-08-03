// Oliver Kullmann, 19.11.2006 (Swansea)
/* Copyright 2006 - 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/plans/general.hpp
  \brief Plans for the module on sets-related algorithms and datastructures


  \todo Update namespace


  \todo Update namespace-usage


  \todo Update doxygen-documentation


  \todo Update plans, transfer todos


  \todo Reorganisation
  <ul>
   <li> "Sets" is also in the supermodule-name, so should be removed from
   the module-name. </li>
   <li> Perhaps we should have a module with basic set operations (like
   union and intersection), and another one with subsumption. </li>
  </ul>


  \todo New test system


  \todo Write docus.


  \todo Write demos.

  \todo Introduce subsumption hypergraph generator
  <ul>
   <li> There are several instances where we have hypergraphs
   with a large number of nodes. For instance, we have
   <ul>
    <li> "Minimisation of the Sbox" in 
    Experimentation/Investigations/Cryptography/plans/SboxInvestigations.hpp
    </li>
    <li> "Smallest prime CNF-representation" in 
    Experimentation/Investigations/BooleanFunctions/plans/UnaryAddition.hpp
    </li>
    <li> "Smallest prime CNF-representation" in 
    Experimentation/Investigations/BooleanFunctions/plans/BinaryAddition.hpp
    </li>
   </ul>
   </li>
   <li> For all such instances, all such algorithms are available
   in the Maxima system (see 
   OKlib/ComputerAlgebra/Hypergraphs/Lisp/Basics.mac), however, for such large
   instances the Maxima system is far too inefficient. </li>
   <li> Details
   <ul>
    <li> As input we have 2 set-systems S1 and S2, and as output, we have the
    hypergraph H, where 
    <ol>
     <li> V(H) = S1, </li>
     <li> E(H) = {{a in A | b is a subset of a} | b in B}. </li>
    </ol>
    </li>
   </ul>
   </li>
   <li> The output should be generated as an extended DIMACS
   file where the variables names express the vertices in the hyperedges
   from S1 (see Satisfiability/Transformers/Generators/Ramsey.hpp for an
   example). </li>
   <li> A subsumption hypergraph generator is implemented now in
   SubsumptionHypergraph.hpp. All that is needed is the appropriate 
   CLSAdaptors to read and write the clause-sets. </li>
   <li> For outputting the subsumption hypergraph, the variables/nodes
   will themselves be hyperedges of the original hypergraph F and so 
   will need to be translated, either to integers or to extended DIMACS
   literals. MG: Is there an CLSAdaptorExtendedDimacsOutput or similar 
   (there is LiteralExtendedReading but seems to be no support for writing?).
   </li> 
   <li> DONE Therefore we need a C++ subsumption hypergraph generator. </li>
  </ul>

*/

/*!
  \namespace OKlib::SetAlgorithms
  \brief Components for sets-related algorithms and datastructures
*/


namespace OKlib {
  namespace SetAlgorithms {
  }
}

