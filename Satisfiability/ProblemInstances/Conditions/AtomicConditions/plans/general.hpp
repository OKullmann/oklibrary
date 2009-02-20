// Oliver Kullmann, 6.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AtomicConditions/plans/general.hpp
  \brief Plans for atomic conditions


  \todo Move concepts from OKlib/Concepts here.


  \todo Update code w.r.t. new namespaces.
  <ul>
   <li> Perhaps the module should be just called "Atomic" ? (Since the "Conditions"
   in "AtomicConditions" is superfluous.) </li>
  </ul>


  \todo Further trivial atomic conditions
  <ul>
   <li> Perhaps Mono<int n>, where n >= 1. Contains an integer m,
   which in case m >= 1 means the singleton {m}, while
   in case m <= -1 the co-singleton {1,...,n} - {-m} is meant.
   Invariant -n <= m <= n. </li>
  </ul>


  \todo First prototype of general atomic condition
  <ul>
   <li> Very first prototype using std::set (for an arbitrary value-type)
   or std::vector<bool> (for value-type {1,...,n}). </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::ProblemInstances::Conditions::AtomicConditions
  \brief Components for atomic conditions (corresponding to characteristic functions of sets)

  Alias "ACn".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace ProblemInstances {
      namespace Conditions {
        namespace AtomicConditions {
        }
        namespace ACn = AtomicConditions;
      }
    }
  }
}
