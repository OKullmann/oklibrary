// Oliver Kullmann, 18.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  See WrongRandomPermutation.cpp for the description.
*/

#include <iostream>
#include <algorithm>
#include <array>

#include <cstdint>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumBasicFunctions.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "18.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/WrongRandomPermutationFixed.cpp",
        "GPL v3"};

  namespace FP = FloatingPoint;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " N\n\n"
    " prints the NxN matrix of counts.\n\n"
;
    return true;
  }

  typedef std::uint_fast16_t base_t;
  const base_t N = 11;
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
