// Oliver Kullmann, 28.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file PartialAssignments/plans/MultivaluedPartialAssignments.hpp
  \brief Plans regarding set-valued partial assignments

  \todo Implementation OKlib::PartialAssignments::MultiPASS
   - Reflect on the concept as discussed in Concepts/plans/PartialAssignments.hpp,
     and update accordingly.
   - Use literals from module Literals.
   - Perhaps instead of the <Values, Var> there should be one template parameter Literal?
     Seems sensible.
   - The static assert should be easier to formulate (since such concept tests will be used
     at many places; is some macro already in OKlib?).

   \todo Test OKlib::PartialAssignments::MultiPASS

*/

