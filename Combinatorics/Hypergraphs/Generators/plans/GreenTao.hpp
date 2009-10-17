// Oliver Kullmann, 15.10.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Generators/plans/GreenTao.hpp
  \brief Plans on generators for Green-Tao-hypergraphs


  \todo Connections
  <ul>
   <li> See Hypergraphs/concepts/plans/general.hpp for the
   concepts; especiall todo "The simplest form of hypergraphs" there for the
   first implementation. </li>
   <li> See arithprog_primes_ohg(k,n) in
   ComputerAlgebra/Hypergraphs/Lisp/Generators/GreenTao.mac
   for the basic function specification. </li>
   <li> See Applications/RamseyTheory/plans/GreenTaoProblems.hpp on
   supporting to compute Gree-Tao numbers at C++ level. </li>
  </ul>


  \todo Generator
  <ul>
   <li> Write a generator for Green-Tao hypergraphs (as model of a general
   hypergraph concept). </li>
   <li> See arithprog_primes_ohg(k,n) in
   ComputerAlgebra/Hypergraphs/Lisp/Generators/GreenTao.mac for the
   specification. </li>
   <li> For simplicity we store the hyperedges. </li>
   <li> Using a vector of vectors. </li>
  </ul>

*/

