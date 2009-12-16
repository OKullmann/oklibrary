// Oliver Kullmann, 8.7.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Hypergraphs/plans/general.hpp
  \brief Components for hypergraph algorithms


  \todo Update namespaces. DONE


  \todo Input and output
  <ul>
   <li> We consider hypergraphs as special clause-sets (namely positive
   clause-sets). </li>
   <li> So it seems that we don't need special input and output? </li>
   <li> Perhaps for the input we should check positivity of all literals? </li>
   <li> And for the input perhaps we add some standardised comment before the
   parameter-line, indicating that here we have a hypergraph? </li>
  </ul>

*/

/*!
  \namespace OKlib::Combinatorics::Hypergraphs
  \brief Supermodule for dedicated hypergraph algorithms

  Alias "Hyp".
*/


namespace OKlib {
  namespace Combinatorics {
    namespace Hypergraphs {
    }
    namespace Hyp = Hypergraphs;
  }
}

