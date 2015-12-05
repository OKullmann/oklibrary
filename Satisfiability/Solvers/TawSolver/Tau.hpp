// Oliver Kullmann, 5.12.2015 (Swansea)
/* Copyright 2015 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Solvers/TawSolver/Tau.hpp
  \brief Components related to tau-computations

*/

#include <limits>
#include <type_traits>

#include <cmath>

#ifndef TAU_vmyxmd
#define TAU_vmyxmd

namespace OKlib {
 namespace Satisfiability {
  namespace Heuristics {
   namespace Projections {

     /*! Computing tau(a,b) with fixed number of Newton-iterations: */
     template <typename D = double, int TAU_IT = 5>
     struct Tau {
       static_assert(std::is_floating_point<D>::value, "Template parameter D must be floating-point.");
       typedef D fp_type;
       constexpr static int iterations = TAU_IT;
       fp_type operator() (const fp_type a, const fp_type b) const {
         fp_type x = std::pow(4,1/(a+b));
         for (int i = 0; i < iterations; ++i) {
           const fp_type pa = std::pow(x,-a), pb = std::pow(x,-b);
           x *= 1 + (pa + pb - 1) / (a*pa + b*pb);
         }
         return x;
       }
     };

     template <typename D> using Tau5 = Tau<D,5>;
     template <int T> using Taud = Projections::Tau<double, T>;
     template <int TAU_IT = 5, typename D>
     D tau(const D a, const D b) { return Tau<D,TAU_IT>()(a,b); }

     template <typename D=double, int TAU_IT=5, typename IND = unsigned int>
     class Min_tau {
     public :
       typedef D fp_type;
       typedef IND index_type;
       constexpr static int iterations = TAU_IT;
       constexpr static fp_type inf_weight = std::numeric_limits<fp_type>::infinity();
     private :
       index_type count_, best;
       fp_type a_, b_;
       fp_type min1, max2;
     public :
       Min_tau() : count_{}, best{}, a_{}, b_{}, min1(inf_weight), max2(0) {}
       index_type index() const { return best; }
       index_type count() const { return count_; }
       fp_type t1() const { return a_; }
       fp_type t2() const { return b_; }
       fp_type min() const { return min1; }
       fp_type max() const { return max2; }

       void operator()(const fp_type a, const fp_type b) {
         assert(a > 0); assert(b > 0);
         ++count_;
         const fp_type chi = std::pow(min1,-a) + std::pow(min1,-b);
         if (chi>1) return;
         const fp_type sum = a + b;
         Projections::Tau<fp_type, iterations> tau;
         if (chi==0) {if ((min1=tau(a,b))==inf_weight and sum<=max2) return;}
         else if (chi==1) { if (sum<=max2) return; }
         else min1=tau(a,b);
         max2=sum;
         best = count_; a_ = a; b_ = b;
       }
     };

   }
  }
 }
}

#endif
