// Oliver Kullmann, 9.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/Learning.hpp
  \brief Plans regarding (pure) learning algorithms


  \todo basic_learning
  <ul>
   <li> Write a version of basic_learning which uses the most basic
   conflict analysis:
    <ol>
     <li> In some order go through phi and remove assignments which
     are not needed to produce the conflict. </li>
     <li> The two most natural orders are the given one and the reverse
     order. </li>
    </ol>
   </li>
   <li> The basic statistics is the number of learned clauses. </li>
   <li> A variation of the algorithm returns the set of learned clauses. </li>
  </ul>

*/

