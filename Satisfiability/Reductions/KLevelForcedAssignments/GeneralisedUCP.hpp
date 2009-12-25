// Oliver Kullmann, 20.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Reductions/KLevelForcedAssignments/GeneralisedUCP.hpp
  \brief Components for r_k-reductions

*/

#ifndef GENERALISEDUCP_HvPi4erD
#define GENERALISEDUCP_HvPi4erD

#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <cassert>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/utility.hpp>

#include <OKlib/Satisfiability/Reductions/UnitClausePropagation/ClsAdaptorUCP.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace Reductions {
      namespace KLevelForcedAssignments {

        /*!
          \class CLSAdaptorKUcp
          \brief Transferring a (boolean) clause-set into a clause-list, and then performing r_k-reduction

          For the Maxima-specification see generalised_ucp_cs in
          ComputerAlgebra/Satisfiability/Lisp/Reductions/GeneralisedUCP.mac.

          Requirements:
          <ul>
           <li> the literal type is a signed integral type </li>
          </ul>

          \todo Complete implementation

          \todo Write unit-tests

        */

        template <class WatchedClauses, class Assignment>
        struct CLSAdaptorKUcp : OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcpW<WatchedClauses,Assignment> {
          typedef OKlib::Satisfiability::Reductions::UnitClausePropagation::CLSAdaptorUcpW<WatchedClauses,Assignment> base_class;

          // return true iff a contradiction was found
          bool perform_kucp(const typename base_class::size_type k) {
            if (this -> empty_cl) return true;
            if (k == 0) return false;
            if (this -> contradicting_ucl) return true;
            if (k == 1) { return this -> perform_ucp(); }
            // XXX to be implemented
            return false;
          }

        };

      }
    }
  }
}



#endif
