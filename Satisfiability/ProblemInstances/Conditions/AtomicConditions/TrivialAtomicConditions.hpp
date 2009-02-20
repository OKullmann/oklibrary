// Oliver Kullmann, 7.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AtomicConditions/TrivialAtomicConditions.hpp
  \brief Typedefs for trivial models of concept AtomicConditions

  \todo Test AC_bool semantically.
*/

#ifndef TRIVIALATOMICCONDITIONS_jBffRql9
#define TRIVIALATOMICCONDITIONS_jBffRql9

#include <OKlib/Satisfiability/ProblemInstances/Conditions/AtomicConditions/traits/value_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Conditions/AtomicConditions/eval.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Conditions/AtomicConditions/complement.hpp>

namespace OKlib {
  namespace AtomicConditions {

    /*!
      \typedef AC_bool
      \brief Boolean values as atomic conditions
    */

    typedef bool AC_bool;

    namespace traits {

      template <>
      struct value_type<AC_bool> {
        typedef bool type;
      };
    }

    template <>
    inline bool eval(const AC_bool& ac, const bool& b) {
      return ac == b;
    }

    template <>
    inline void complement(AC_bool& ac) { ac = !ac; }

  }

  namespace Concepts {
    namespace traits {
      
      template <>
      struct concept_tag<OKlib::AtomicConditions::AC_bool> {
        typedef OKlib::Concepts::AtomicCondition_tag type;
      };

    }
  }
}

#endif
