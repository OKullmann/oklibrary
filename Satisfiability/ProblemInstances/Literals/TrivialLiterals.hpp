// Oliver Kullmann, 9.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file  ProblemInstances/Literals/TrivialLiterals.hpp
  \brief Typedefs for models of concept Literals given by signed integral types
*/

#ifndef TRIVIALLITERALS_jdjNNqpqoh7u5
#define TRIVIALLITERALS_jdjNNqpqoh7u5

#include <cstdlib>

#include <OKlib/Concepts/traits/concept_tag.hpp>

#include <OKlib/Concepts/Variables.hpp>
#include <OKlib/Concepts/Literals.hpp>
#include <OKlib/Concepts/JoinedConceptTags.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Variables/TrivialVariables.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Conditions/AtomicConditions/TrivialAtomicConditions.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/var_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/cond_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/var.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/cond.hpp>


namespace OKlib {
  namespace Literals {

    typedef signed char Literals_signed_char;
    typedef short int Literals_short_int;
    typedef int Literals_int;
    typedef long int Literals_long_int;

    namespace traits {

      template <>
      struct var_type<Literals_signed_char> { typedef OKlib::Variables::Variables_signed_char type; };
      template <>
      struct var_type<Literals_short_int> { typedef OKlib::Variables::Variables_short_int type; };
      template <>
      struct var_type<Literals_int> { typedef OKlib::Variables::Variables_int type; };
      template <>
      struct var_type<Literals_long_int> { typedef OKlib::Variables::Variables_long_int type; };

      template <>
      struct cond_type<Literals_signed_char> { typedef OKlib::AtomicConditions::AC_bool type; };
      template <>
      struct cond_type<Literals_short_int> { typedef OKlib::AtomicConditions::AC_bool type; };
      template <>
      struct cond_type<Literals_int> { typedef OKlib::AtomicConditions::AC_bool type; };
      template <>
      struct cond_type<Literals_long_int> { typedef OKlib::AtomicConditions::AC_bool type; };

    }

    template <>
    inline traits::var_type<Literals_signed_char>::type var(const Literals_signed_char& x) { return std::abs(x); }
    template <>
    inline traits::var_type<Literals_short_int>::type var(const Literals_short_int& x) { return std::abs(x); }
    template <>
    inline traits::var_type<Literals_int>::type var(const Literals_int& x) { return std::abs(x); }
    template <>
    inline traits::var_type<Literals_long_int>::type var(const Literals_long_int& x) { return std::abs(x); }

    template <>
    inline traits::cond_type<Literals_signed_char>::type cond(const Literals_signed_char& x) { return x > 0; }
    template <>
    inline traits::cond_type<Literals_short_int>::type cond(const Literals_short_int& x) { return x > 0; }
    template <>
    inline traits::cond_type<Literals_int>::type cond(const Literals_int& x) { return x > 0; }
    template <>
    inline traits::cond_type<Literals_long_int>::type cond(const Literals_long_int& x) { return x > 0; }

    template <>
    inline void set_cond(Literals_signed_char& x, const traits::cond_type<Literals_signed_char>::type& cond) {
      if (cond) x = std::abs(x);
      else x = - std::abs(x);
    }
    template <>
    inline void set_cond(Literals_short_int& x, const traits::cond_type<Literals_short_int>::type& cond) {
      if (cond) x = std::abs(x);
      else x = - std::abs(x);
    }
    template <>
    inline void set_cond(Literals_int& x, const traits::cond_type<Literals_int>::type& cond) {
      if (cond) x = std::abs(x);
      else x = - std::abs(x);
    }
    template <>
    inline void set_cond(Literals_long_int& x, const traits::cond_type<Literals_long_int>::type& cond) {
      if (cond) x = std::abs(x);
      else x = - std::abs(x);
    }

  }

  namespace Concepts {
    namespace traits {
      
      // Since Literals::Literals_signed_char is the same type as Variables::Variables_signed_char, the definitions from Variables/TrivialVariables.hpp must be used here.

    }
  }
}

#endif
