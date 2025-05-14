// Oliver Kullmann, 16.8.2023 (Swansea)
/* Copyright 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/Statistics.hpp>

#include "RandomGraphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "14.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestRandomGraphs.cpp",
        "GPL v3"};

  using namespace RandomGraphs;
  namespace FP = FloatingPoint;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }


  // Counting frequency of edges, antilexicographically:
  std::vector<size_t> edge_freqs(const size_t n, const RandGen::Prob64 p,
                                 const bool no_loops, const size_t T) {
    const size_t m = Graphs::Sizes::max_m(n, Graphs::GT::und, not no_loops);
    Combinatorics::CoLexicographic C{Graphs::as_graph({Graphs::GT::und, not no_loops})};
    RandGen::RandGen_t g;
    std::vector<size_t> res(m);
    for (size_t t=0; t < T; ++t) {
      const auto E = binomial_rgr(n, p, g, no_loops).alledges();
      for (const auto& e : E) ++res[C(e)];
    }
    return res;
  }
  std::vector<size_t> edge_freqs(const size_t n, const size_t m,
                                 const bool no_loops, const size_t T) {
    const size_t mm = Graphs::Sizes::max_m(n, Graphs::GT::und, not no_loops);
    Combinatorics::CoLexicographic C{Graphs::as_graph({Graphs::GT::und, not no_loops})};
    RandGen::RandGen_t g;
    std::vector<size_t> res(mm);
    for (size_t t=0; t < T; ++t) {
      const auto E = uniform_rgr(n, m, g, no_loops).alledges();
      assert(E.size() == m);
      for (const auto& e : E) ++res[C(e)];
    }
    return res;
  }

  // Counting graphs, via colex_index, so that on average ave are expected:
  std::vector<size_t> graph_freqs(const size_t n,
                                 const bool no_loops, const size_t ave) {
    const size_t mm = Graphs::Sizes::max_m(n, Graphs::GT::und, not no_loops);
    const size_t num_graphs = std::pow(size_t(2), mm);
    RandGen::RandGen_t g;
    std::vector<size_t> res(num_graphs);
    for (size_t m = 0; m <= mm; ++m) {
      const size_t mul = FP::binomial_coeff(mm, m);
      for (size_t t = 0; t < mul * ave; ++t) {
        const AdjVecUInt G = uniform_rgr(n,m,g, no_loops);
        ++res[colex_index(G, not no_loops)];
      }
    }
    return res;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {eqp(edge_freqs(0,{1,2}, false, 10), {});
   eqp(edge_freqs(0,{1,2}, true, 10), {});
   eqp(edge_freqs(1,{1,2}, false, 1000), {522});
   eqp(edge_freqs(1,{1,2}, false, 1000), {0});
   eqp(edge_freqs(2,{1,2}, false, 1000), {519,507,475});
   eqp(edge_freqs(2,{1,2}, true, 1000), {522});
   eqp(edge_freqs(3,{1,100}, false, 1000), {4,8,14,12,6,11});
   eqp(edge_freqs(3,{1,100}, true, 1000), {13,6,9});
   eqp(edge_freqs(10,{1,10}, false, 1000), {109,121,118,109,94,100,105,107,101,108,113,103,106,95,104,96,100,102,96,89,102,100,112,107,98,96,108,81,103,105,108,111,100,113,111,97,85,96,109,88,110,101,110,99,101,124,102,91,93,94,101,98,86,116,9});
   eqp(edge_freqs(10,{1,10}, true, 1000), {112,111,102,99,79,107,108,88,104,100,107,88,102,104,108,95,107,109,110,97,108,107,83,130,86,106,87,96,114,108,101,102,109,101,86,119,84,102,110,111,90,100,94,92,108});
  }
  {RandGen::RandGen_t g{1,2,4};
   assert(eqp(binomial_rgr(5, {1,2}, g).alledges(),
              {{0,1},{0,4},{1,3},{1,4},{2,3},{2,4}}));
   assert(eqp(binomial_rgr(5, {1,2}, g, false).alledges(),
              {{0,0},{0,2},{0,4},{1,2},{1,3},{1,4},{2,2},{2,3},{3,4},{4,4}}));
  }

  {assert(eqp(edge_freqs(0,0, false, 10), {}));
   assert(eqp(edge_freqs(1,0, false, 10), {0}));
   assert(eqp(edge_freqs(1,1, false, 10), {10}));
   assert(eqp(edge_freqs(2,1, true, 10), {10}));
   assert(eqp(edge_freqs(2,1, false, 100), {33,30,37}));
   assert(eqp(edge_freqs(5,3, false, 100), {25,19,18,20,21,18,25,16,23,19,22,25,17,13,19}));
   assert(eqp(edge_freqs(10,6, true, 100), {14,13,14,15,10,18,12,17,13,13,18,10,16,13,11,10,12,18,15,19,13,9,17,13,10,15,16,7,13,10,15,18,8,10,11,21,17,13,4,9,15,8,18,15,14}));
  }
  {/* // Roughly 60s runtime:
   const auto F = graph_freqs(4, false, 1000);
   const GenStats::StdFourStats S(F);
   assert(S.N == 1024); assert(S.min == 913); assert(S.max == 1101); 
   assert(S.amean == 1000); assert(S.sdc == 30.7971638003033892604L);
   */
   const auto F = graph_freqs(4, true, 1000);
   const GenStats::StdFourStats S(F);
   assert(S.N == 64); assert(S.min == 946); assert(S.max == 1059); 
   assert(S.amean == 1000); assert(S.sdc == 26.0311169107099132768L);
   assert(eqp(F, {1000,1039,1003,992,954,995,961,1009,1000,1023,988,1041,1039,986,1026,992,1003,1016,976,1059,1023,997,960,962,1009,946,1029,1012,976,1030,971,990,1001,966,953,1009,1043,969,1006,974,990,975,1006,1012,1016,976,1007,981,1026,1006,997,986,975,1049,957,1000,1012,1023,1043,992,1006,1015,1022,1000}));
  }

}
