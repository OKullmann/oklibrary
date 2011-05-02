// Oliver Kullmann, 2.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProblemInstances/Literals/Basics.hpp
  \brief Basic functionality for literals
*/

#ifndef BASICS_HAHAvr3Wqa10
#define BASICS_HAHAvr3Wqa10

#include <utility>
#include <functional>

#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/var_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/cond_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/var.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/cond.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace ProblemInstances {
      namespace Literals {

        //! Functor for translating a literal x into a pair (var,condition)
        template <typename Lit>
        struct Literal2pair : std::unary_function<const Lit, std::pair<typename OKlib::Literals::traits::var_type<Lit>::type, typename OKlib::Literals::traits::cond_type<Lit>::type> > {
          typedef Lit literal_type;
          typedef typename OKlib::Literals::traits::var_type<Lit>::type var_type;
          typedef typename OKlib::Literals::traits::cond_type<Lit>::type cond_type;
          typedef std::pair<var_type,cond_type> litaspair_type;
          litaspair_type operator()(const literal_type x) const {
            return litaspair_type(OKlib::Literals::var(x),
                                  OKlib::Literals::cond(x));
          }
        };
        template <typename Lit>
        inline typename Literal2pair<Lit>::litaspair_type literal2pair(const Lit x) {
          return Literal2pair<Lit>()(x);
        }
      }
    }
  }
}


#endif
