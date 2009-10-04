// Oliver Kullmann, 7.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/Generators/plans/Hindman.hpp
  \brief Plans on generators for Hindman-hypergraphs


  \todo Class Generators::Hindman_k2
  <ul>
   <li> We need to use the new test system. </li>
   <li> Since these hypergraphs could be very big, we should produce a
   "virtual" hypergraph, as a range of ranges:
    <ol>
     <li> We need a simple concept of hypergraphs, which allows to output
     them as hypergraphs or as clause-sets. </li>
     <li> See Hypergraphs/concepts/plans/general.hpp. </li>
     <li> Likely we best employ the newest version of the Boost range-library;
     but then we need to eliminate first the Boost-related problems. </li>
    </ol>
   </li>
   <li> We should also have such a range-of-ranges hypergraph in some form
   at the Maxima/Lisp level. </li>
   <li> However, first we supply a non-virtual version. </li>
   <li> See the todos in
   OKlib/Combinatorics/Hypergraphs/Generators/Hindman.hpp. </li>
  </ul>

*/
