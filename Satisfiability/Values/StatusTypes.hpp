// Oliver Kullmann, 12.12.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Values/StatusTypes.hpp
  \brief Types related to states

  <ul>
   <li> Type Values::Assignment_status is a role-model for other such
   status-types. </li>
   <li> We use specialised types also in case one could re-use another type,
   whose naming however does not reflect the use-case. </li>
   <li> Special values are "first", "last", and "exceptional". </li>
  </ul>

*/

#ifndef STATUSTYPES_brqpaGt41
#define STATUSTYPES_brqpaGt41

#include <OKlib/Satisfiability/Values/traits/first_value.hpp>
#include <OKlib/Satisfiability/Values/traits/last_value.hpp>
#include <OKlib/Satisfiability/Values/traits/exceptional_value.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace Values {

      /*!
        \enum Assignment_status
        \brief Status of a boolean variable (false, true, open)

        For a status-value s by -s we denote negation.
      */

      enum Assignment_status { val0 = 0, val1 = 1, unassigned = 2 };
      namespace traits {
        template <> struct first_value<Assignment_status> {
          enum { value = val0 };
        };
        template <> struct last_value<Assignment_status> {
          enum { value = val1 };
        };
        template <> struct exceptional_value<Assignment_status> {
          enum { value = unassigned };
        };
      }
      inline Assignment_status operator -(const Assignment_status a) {
        switch (a) {
        case val0 : return val1;
        case val1 : return val0;
        default : return unassigned;
        }
      }

      enum Sat_status { falsified = 0, satisfied = 1, open = 2 };
      namespace traits {
        template <> struct first_value<Sat_status> {
          enum { value = falsified };
        };
        template <> struct last_value<Sat_status> {
          enum { value = open };
        };
      }

    }
  }
}



#endif

