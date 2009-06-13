// Oliver Kullmann, 21.9.2007 (Durham)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/Backdoors/plans/Horn.hpp
  \brief Plans on the module for backdoor algorithms targeting Horn


  \todo Relate to
  ComputerAlgebra/Satisfiability/Lisp/Backdoors/plans/general.hpp.


  \todo Design for the algorithm "sb-horn" from [Nishimura, Ragde, Szeider;
  2003, Detecting Backdoor Sets with Respect to Horn and Binary
  Clauses] (developers SS, MS, OK)
  <ul>
   <li> Similar to "sb-2cnf" (see Algorithms/Backdoors/plans/Horn.hpp). </li>
   <li> Again (as in Backdoors/plans/2CNF.hpp), likely we should not
   implement the algorithm, but just instantiate one of the generic
   algorithms from Hypergraphs/Transversals/Bounded/plans/general.hpp:
    <ol>
     <li> For boolean clause-set F, construct the hypergraph G by dropping
     all negative literals from clauses, and then let G' be the 2-section
     of G. </li>
     <li> Now the transversals of G' are exactly the backdoors for F. </li>
    </ol>
   </li>
  </ul>

*/

