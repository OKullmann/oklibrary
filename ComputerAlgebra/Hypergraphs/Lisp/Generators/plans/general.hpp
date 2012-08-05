// Oliver Kullmann, 19.7.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/general.hpp
  \brief Plans regarding generators for hypergraphs

  \todo Connections
  <ul>
   <li> Finally all the generators provided here are also to be provided at
   C++ level in module Combinatorics/Hypergraphs/Generators; see
   Combinatorics/Hypergraphs/Generators/plans/general.hpp. </li>
  </ul>


  \todo Organisation
  <ul>
   <li> The current Generators/Generators.mac needs to be split into several
   files:
    <ol>
     <li> Basics.mac </li>
     <li> VanderWaerden.mac : DONE</li>
     <li> GreenTao.mac : DONE </li>
     <li> Schur.mac : DONE </li>
     <li> Rado.mac : DONE </li>
     <li> Folkman.mac </li>
     <li> Hindman.mac : DONE </li>
     <li> Ramsey.mac : DONE </li>
    </ol>
   </li>
  </ul>
  
  
  \todo Statistics
  <ul>
   <li> For all generated hypergraphs we need accompanying
   statistic-functions. </li>
   <li> As for clause-set statistics. </li>
  </ul>


  \todo Sudoku
  <ul>
   <li> Create Sudoku-hypergraphs (such that the Sudoku-problems can be
   expressed as strong colouring problems). </li>
   <li> For box dimension p the vertex set should be {1,...,p}^2. </li>
   <li> The 3N hyperedges (N=p^2) just correspond to the rows, columns
   and boxes (the hypergraph is thus N-uniform). </li>
  </ul>

*/
