// Oliver Kullmann, 8.11.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Sequences.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "8.11.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestSequences.cpp",
        "GPL v3"};

  using namespace RandGen;

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
}
