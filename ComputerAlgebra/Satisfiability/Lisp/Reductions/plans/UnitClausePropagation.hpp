// Oliver Kullmann, 24.12.2008 (Schwalbach)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Lisp/Reductions/plans/UnitClausePropagation.hpp
  \brief Plans for Maxima-components regarding UCP


  \todo The most basic form of UCP
  <ul>
   <li> DONE
   ucp_0_cs is also needed in the form returning a partial assignment.
   </li>
   <li> And we need the forms for formal clause-sets and clause-lists. </li>
   <li> Currently we have two forms of ucp:
    <ol>
     <li> If the input F is found unsatisfiable, then {{}} is returned. </li>
     <li> The other extreme is that as soon as the empty clause has been
     created, the reduction process stops. </li>
    </ol>
    Likely we should indicate this in the names: "ucp_full, ucp_min" ?
   </li>
  </ul>

*/

