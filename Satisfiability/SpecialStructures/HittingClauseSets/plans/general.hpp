// Oliver Kullmann, 24.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file HittingClauseSets/plans/general.hpp
  \brief General plans for the module on hitting clause-sets


  \todo Update namespaces.


  \todo Given a rooted tree, a corresponding SMUSAT_{delta=1} needs to
  to be constructed. Special cases for the full tree and the input tree.
  Perhaps this should go to a submodule RegularHittingClauseSets.


  \todo More generally, for an arbitrary rooted tree and a labelling of inner
  nodes with variables, such that no variable occurs twice on any path from
  the root to some leaf, construct the corresponding (tree-)hitting clause-set.


  \todo Likely, in this module also multi-hitting clause-sets should
  be considered (with non-boolean variables).
  
*/

/*!
  \namespace OKlib::HittingClauseSets
  \brief Module for components regarding hitting clause-sets
*/

namespace OKlib {
  namespace HittingClauseSets {
  }
}

