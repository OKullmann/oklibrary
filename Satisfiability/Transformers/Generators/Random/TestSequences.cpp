// Oliver Kullmann, 8.11.2020 (Swansea)
/* Copyright 2020, 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Sequences.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "14.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestSequences.cpp",
        "GPL v3"};

  using namespace RandGen;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {ExpSeq S(3,2,2,true);
   gen_uint_t s = 0, c = 0;
   for (auto i = S.begin(); i != S.end(); ++i, ++c) s += (*i).val;
   assert(s == 17775932923502691692ULL);
   assert(c == 3 * 99 * 2);
   s = 0; c = 0;
   for (auto i = S.begin(); i != S.end(); ++i, ++c) s += (*i).val;
   assert(s == 17775932923502691692ULL);
   assert(c == 3 * 99 * 2);
   auto i_orig = S.begin();
   {auto i0 = i_orig; auto i1(i0);
    assert(i0 != S.end());
    assert(i1 != S.end());
    s = 0; c = 0;
    for (auto i = i0; i != S.end(); ++i, ++c) s += (*i).val;
    assert(s == 17775932923502691692ULL);
    assert(c == 3 * 99 * 2);
    s = 0; c = 0;
    for (auto i = i1; i != S.end(); ++i, ++c) s += (*i).val;
    assert(s == 17775932923502691692ULL);
    assert(c == 3 * 99 * 2);
    i0 = i1;
    s = 0; c = 0;
    for (; i0 != S.end(); ++i0, ++c) s += (*i0).val;
    assert(s == 17775932923502691692ULL);
    assert(c == 3 * 99 * 2);
    assert(i0 == S.end());
    s = 0; c = 0;
    for (; i1 != S.end(); ++i1, ++c) s += (*i1).val;
    assert(s == 17775932923502691692ULL);
    assert(c == 3 * 99 * 2);
    assert(i1 == S.end());
   }
   s = 0; c = 0;
   for (; i_orig != S.end(); ++i_orig, ++c) s += (*i_orig).val;
   assert(s == 17775932923502691692ULL);
   assert(c == 3 * 99 * 2);
   assert(i_orig == S.end());
  }
  {
   for (gen_uint_t E = 1; E <= 4; ++E) {
     for (unsigned S = 1; S <= 3; ++S) {
       gen_uint_t s = std::pow(gen_uint_t(10), S);
       for (gen_uint_t N = 1; N <= 100; N+=10) {
         for (unsigned eps = 0; eps <= 1; ++eps) {
           const ExpSeq es(E,S,N,eps);
           assert(es.main_size() == E * (s-1));
           assert(es.size() == E * (s-1) * N);
           gen_uint_t count = 0;
           for (const auto& x : es) {
             ++count;
             assert(x.s == s);
             assert(x.ge1 == eps);
           }
           assert(count == es.size());
         }
       }
     }
   }
  }
  {const ExpSeq S(4,2,3,false);
   const gen_uint_t size = 3*4*99;
   assert(S.size() == size);
   std::ostringstream ss;
   using namespace FloatingPoint;
   fullprec_float80(ss);
   Environment::out_line_T(ss, S, ExpSeq::translate<float80>);
   assert(ss.str().size() == 30226);
   const auto values1 = to_vec_float80(ss.str(), ' ');
   assert(values1.size() == size);
   std::vector<float80> values2; values2.reserve(size);
   std::ranges::transform(S, std::back_inserter(values2),
                          ExpSeq::translate<float80>);
   assert(values2.size() == size);
   assert(accuracymax(values1, values2) <= 1);
  }

  {UniformVectors V(0,0,{});
   assert(V.a == 0 and V.b == 0 and V.n == 1);
   assert(eqp(V(0), {}));
   assert(eqp(V(1), {0}));
   assert(eqp(V(2), {0,0}));
   assert(eqp(V(3), {0,0,0}));
  }
  {UniformVectors V(0,1,{});
   assert(V.a == 0 and V.b == 1 and V.n == 2);
   assert(eqp(V(0), {}));
   assert(eqp(V(1), {0}));
   assert(eqp(V(2), {0,0}));
   assert(eqp(V(0), {}));
   assert(eqp(V(3), {0,1,1}));
  }
  {UniformVectors V(1,0,{});
   assert(V.a == 0 and V.b == 1 and V.n == 2);
   assert(eqp(V(0), {}));
   assert(eqp(V(1), {0}));
   assert(eqp(V(2), {0,0}));
   assert(eqp(V(0), {}));
   assert(eqp(V(3), {0,1,1}));
  }
  {UniformVectors V(6,13,{77});
   assert(V.a == 6 and V.b == 13 and V.n == 8);
   assert(eqp(V(20), {7,9,7,13,7,6,7,9,13,7,6,10,10,13,12,6,9,12,6,12}));
  }
  {UniformVectors V(1,randgen_max,{});
   assert(V.a == 1 and V.b == randgen_max and V.n == randgen_max);
   assert(eqp(V(10), {835052665647855779ULL,3190053552572815829ULL,4634633302865102306ULL,6117669629961065222ULL,14178379554402451545ULL,16962453898710657878ULL,9687764880492433482ULL,17119310685016112210ULL,9933769879964784476ULL,15552734942315811377ULL}));
  }
  {UniformVectors V(0,randgen_max,{});
   assert(V.a == 0 and V.b == randgen_max and V.n == 0);
   assert(eqp(V(10), {835052665647855778ULL,3190053552572815828ULL,4634633302865102305ULL,6117669629961065221ULL,14178379554402451544ULL,16962453898710657877ULL,9687764880492433481ULL,17119310685016112209ULL,9933769879964784475ULL,15552734942315811376ULL}));
  }

}
