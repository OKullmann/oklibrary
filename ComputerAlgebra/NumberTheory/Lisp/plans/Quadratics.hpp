// Oliver Kullmann, 5.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/NumberTheory/Lisp/plans/Quadratics.hpp
  \brief Plans regarding quadratic functions


  \todo count_quadintsol
  <ul>
   <li> The testfunction has to be extended. </li>
   <li> More efficient computations are needed.
    <ol>
     <li> For a=1 the function nhyp_hindman_a1k2_ohg(b) (see
     ComputerAlgebra/Hypergraphs/Lisp/Generators/Hindman.mac) is identical,
     and is much faster. </li>
     <li> How can this be generalised? </li>
     <li> And are faster computations (at least in this special case)
     possible? </li>
     <li> What about good approximations? </li>
    </ol>
   </li>
  </ul>

*/

