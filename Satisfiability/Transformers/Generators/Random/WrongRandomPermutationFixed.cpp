// Oliver Kullmann, 18.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  See WrongRandomPermutation.cpp for the general description.

  Must be compiled with the value for N (size of the permutation)
  defined via macro NVALUE, that is, using the flag (here for N=11)
    -DNVALUE=11
  With the given Makefile, that can be done via CXXFLAGS :

Random> CXXFLAGS="-DNVALUE=11" make -B WrongRandomPermutationFixed

  Note the forced compilation via "-B" (just setting a different NVALUE
  will not be noticed via the Makefile).
  Also note that above not the debug-version was compiled.


*/

#include <iostream>
#include <algorithm>
#include <array>

#include <cstdint>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumBasicFunctions.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.0",
        "19.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/WrongRandomPermutationFixed.cpp",
        "GPL v3"};

  namespace FP = FloatingPoint;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  typedef std::uint_fast16_t base_t;
#ifndef NVALUE
#  define NVALUE 10
#endif
  constexpr base_t N = NVALUE;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << "\n\n"
    " prints the NxN matrix of counts for the hardcoded N=" << N << ".\n\n"
;
    return true;
  }

  using FP::UInt_t;

  template <base_t N>
  struct PerformCount {
    typedef std::array<std::array<UInt_t, N>, N> counts_t;
    counts_t counts{};
    const Environment::tokens_t header;

    static Environment::tokens_t cheader() {
      Environment::tokens_t res(N);
      for (base_t i = 0; i < N; ++i) res[i] = std::to_string(i);
      return res;
    }

    typedef std::array<base_t, N> trial_t;
    constexpr void evaluate(const trial_t& t, const base_t current) {
      assert(current <= N);
      if (current == N) {
        for (base_t i = 0; i < N; ++i) {
          const base_t v = t[i];
          assert(v < N);
          ++counts[i][v];
        }
      }
      else {
        for (base_t v = 0; v < N; ++v) {
          trial_t tnew(t);
          std::swap(tnew[current], tnew[v]);
          evaluate(tnew, current+1);
        }
      }
    }

    constexpr PerformCount() :  header(cheader()) {
      evaluate(
               []{trial_t res; for (base_t i = 0; i<N; ++i) res[i] = i;
                 return res;}(),
               0);
    }

    friend std::ostream& operator <<(std::ostream& out, const PerformCount& P) {
      Environment::print2dformat(out, P.counts, 1, P.header);
      return out;
    }
  };

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::cout
    << "  " << N << " " << FP::pow(N,N) << " " << FP::pow(N,N+1) << std::endl
    << PerformCount<N>();
}
