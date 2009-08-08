// Oliver Kullmann, 8.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/Operations/plans/general.hpp
  \brief Plans in general regarding operations of groupoids


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Algebra/Lisp/Groupoids/Actions/plans/general.hpp
   for actions of sets on structures. </li>
  </ul>


  \todo Notions and notations
  <ul>
   <li> "opr_grdset" for the operation of a groupoid on a set. </li>
   <li> Or perhaps "grd" would be the default? Seems too special to devote
   a default for this case, so better not. </li>
   <li> Different from actions, here we need to have the structure which
   operates as part of the name, since it could also be for example a ring.
   </li>
   <li> Perhaps this should be a triple [V,X,opr], and so the current
   "opr_grdset_p(opr,V,X)" should instead be "opr_grdset_p([V,X,opr])". </li>
   <li> While a V-operations (which in case of a group V would also be called
   a "V-set") would be a pair [X,opr]. </li>
  </ul>
  
*/
