// Oliver Kullmann, 19.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/Basics.hpp
  \brief Plans regarding the basic notions for hypergraphs


  \todo Naming conventions
  <ul>
   <li> "hg" for hypergraph, "ghg" for general hypergraph, and "ohg", "oghg"
   for the ordered versions. </li>
   <li> "mhg" for multi-hypergraph, and "omhg" for ordered multi-hypergraph.
   </li>
   <li> Conversions then as "hg2ghg", "ohg2hg" etc. </li>
   <li> Connections to graphs (see
   ComputerAlgebra/Graphs/Lisp/plans/general.hpp):
    <ol>
     <li> g, gl <= hg </li>
     <li> mug, mugl <= mhg </li>
     <li> gg <= ghg </li>
     <li> og, ogl <= ohg </li>
     <li> omug, omugl <= omhg </li>
     <li> ogg <= oghg </li>
    </ol>
   </li>
  </ul>

*/

