// Matthew Gwynne, 11.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/plans/Ramsey.hpp
  \brief Plans for generators for %Ramsey problems


  \todo Strengthen connection to Maxima/Lisp level
  <ul>
   <li> For all components here there should be (close) procedural
   specification at the Maxima/Lisp level. </li>
   <li> One has then to consider the order of the hyperedges and of the
   clauses. </li>
  </ul>


  \todo Move todos to this file
  <ul>
   <li> The todos in Satisfiability/Transformers/Generators/Ramsey.cpp and
   Satisfiability/Transformers/Generators/Ramsey.hpp need to be
   (appropriately) moved to this plans-file. </li>
  </ul>


  \todo Write unit tests


  \todo Add generator to generate multi-label %Ramsey problems
  <ul>
   <li> Currently the Ramsey generator only handles Ramsey problems given
   2 labels/partitions/colours. </li>
   <li> A new generator should be written which generates CNF representations
   for %Ramsey problem instances of the form "ramsey_s^r(q_1,...,q_s) < n?". 
   </li>
   <li> See 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyProblems.mac for how
   to handle the translation of the non-boolean nature of the problem when 
   considering %Ramsey problems with more than 2 labels. </li>
  </ul>


  \todo DONE Generator should produce standardised variables using 
  colexicographical ordering
  <ul>
   <li> The C++ %Ramsey generator (see Ramsey.cpp) should use the same variable
   names as in 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyProblems.mac . </li>
   <li> This would allow simple combination of these generated clause-sets with
   symmetry breaking techniques etc. </li>
  </ul>


*/

