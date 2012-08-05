// Oliver Kullmann, 27.7.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/general.hpp
  \brief Plans regarding %Ramsey problems


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Ramsey.hpp for
   the function ramsey_hg(q,r,n) and its variations. </li>
   <li> See
   Satisfiability/Lisp/Generators/RamseyTheory/plans/RamseyProblems.hpp
   for the SAT translation. </li>
   <li> See
   Investigations/RamseyTheory/RamseyProblems/plans/general.hpp for
   investigations. </li>
  </ul>


  \todo Automorphisms of %Ramsey hypergraphs
  <ul>
   <li> First the natural operation of S_n on ramsey_hg(q,r,n) needs to
   be made available.
    <ol>
     <li> Implemented in
     ComputerAlgebra/RamseyTheory/Lisp/Ramsey/Hypergraphs.mac. </li>
     <li> Easiest first to consider the elements of S_n as lists. </li>
    </ol>
   </li>
   <li> The operation of S_n should be faithful (in most cases), and we
   need to extract the induced automorphisms of ramsey_hg(q,r,n). </li>
   <li> Then we need to find out whether these automorphisms cover all.
    <ol>
     <li> For small parameters values (including q=3,r=2) we can
     enumerate all automorphisms by brute force. </li>
     <li> What tools are available for hypergraph automorphisms?
      <ul>
       <li> See "Hypergraph automorphisms" in
       ComputerAlgebra/Hypergraphs/Lisp/plans/general.hpp for general plans.
       </li>
       <li> It seems that Gap doesn't provide any (direct) help here?
       At least grape only considers graphs --- are there other packages?
       </li>
       <li> What about nauty? See "Graph isomorphism" in
       Buildsystem/ExternalSources/SpecialBuilds/plans/Graphs.hpp; but it
       seems that hypergraphs are not considered. </li>
       <li> Reducing the hypergraph isomorphism problem to (di)graph
       isomorphism can happen similar to the treatmeant of clause-sets (see
       ComputerAlgebra/Satisfiability/Lisp/Symmetries/Symmetries.mac). </li>
       <li> Considering just the 2-section of a hypergraph should in general
       be too rough (though it is attractive). </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> It would also be interesting here to try apply some of the symmetry
   breaking ideas discussed in
   Satisfiability/Lisp/Generators/RamseyTheory/plans/RamseyProblems.hpp
   and then consider the number and character of the remaining symmetries.
   </li>
  </ul>


  \todo Automorphisms of %Ramsey clause-sets
  <ul>
   <li> The obvious automorphisms of diagonal %Ramsey clause-sets are
   given by the inner product of automorphisms for the underlying %Ramsey
   hypergraph and the S_s when using s colours. </li>
   <li> So at least we have S_n x S_s (translated). </li>
   <li> Are there more?
    <ol>
     <li> It seems that by brute force (enumerating all clause-set
     automorphisms we cannot process any non-trivial example? </li>
     <li> We need stronger tools for computing automorphisms of clause-sets.
     </li>
    </ol>
   </li>
   <li> And what about non-diagonal %Ramsey clause-sets? One would guess
   that in "most" cases there are no other automorphisms than given by
   the underlying S_n ? </li>
  </ul>


  \todo Representing counter-examples
  <ul>
   <li> See '"Visualising" solutions' in
   Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/general.hpp
   . </li>
   <li> One should be able to translate a solution to a given Ramsey SAT
   problem (that is, "ramsey_r^s(q_1,...,q_s) > n?"), given as a set of
   literals using the standard variable naming, to a graph labelling (see
   "Representing edge and vertex labellings" in
   ComputerAlgebra/Graphs/Lisp/plans/general.hpp). </li>
   <li> For r=2, s=2, one should also be able to translate a solution to the
   graph representing the solution (where the 2 labels represent an edge being
   either in or out of the graph). </li>
  </ul>

*/
