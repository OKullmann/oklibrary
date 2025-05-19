// Oliver Kullmann, 18.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Experimentation with known wrong algorithm for creating random
  permutations of length N, computing the frequency table.

  That is, in Algorithms::shuffle always choose the index for
  swapping (wrongly) from the whole range.

  See
    https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
     - Naive method
     - Similarly, always selecting j from the entire range of valid array
       indices on every iteration also produces a result which is biased, ...

  This wrong algorithm does not only not give each of the N! permutations the
  same frequency for N >= 3 (which is obvious since N-1 divides N!, but
  not N^N), but even fails the simpler frequency-test for the distributions
  of values to the places of the permutation.

See docus/WrongRandomPermutation for output, and see
  WrongRandomPermutationFixed.cpp
for a faster implementation, with hard-coded
N.

EXAMPLES:

Random> ./WrongRandomPermutation 3
  3 27 81
0  1  2
9 10  8
9  8 10
9  9  9

First line is
 - N
 - N^N (the number of permutations created)
 - N^(N+1) (the total number of counts).

Second line is the header for the table, the digit whose frequency is
counted (for the permutations created), while then we have the lines for
positions 0, ..., N-1 (all indices are 0-based).

It is true in general that digit 0 is always equally distributed (thus
the first column is constant 9 above), and the last position is correctly
random (and thus the last row is constant 9 above).
Each row and each column adds up to N^N.

Above, digit 1 occur in 10/27 case in the first place, while digit 2 occurs
in 8/27 cases.
These 8 cases for digit 2 come from the following trial-vectors (trial_t
below), which is from {0,1,2}^3:
 - 2^2 for {2} x {1,2}^2
 - 2   for {1} x {0,1} x {2}
 - 2   for {0} x {0,1} x {2} .

In general, the occurrence of digit N-1 at position 0 is strictly smallest
overall for N >= 4.

*/

#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

#include <cstdint>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumBasicFunctions.hpp>
#include <Numerics/NumInOut.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "18.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/WrongRandomPermutation.cpp",
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

  typedef std::uint16_t base_t;
  using FP::UInt_t;

  struct PerformCount {
    const base_t N;
    typedef std::vector<std::vector<UInt_t>> counts_t;
    counts_t counts;
    // counts[i][v] says how often position i got value v
    const Environment::tokens_t header;

    static counts_t init(const base_t N) {
      return counts_t(N, std::vector<UInt_t>(N));
    }
    static Environment::tokens_t cheader(const base_t N) {
      Environment::tokens_t res(N);
      for (base_t i = 0; i < N; ++i) res[i] = std::to_string(i);
      return res;
    }

    typedef std::vector<base_t> trial_t;
    void evaluate(const trial_t& t, const base_t current) {
      assert(t.size() == N);
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

    PerformCount(const base_t N) :  N(N), counts(init(N)), header(cheader(N)) {
      evaluate(
               std::ranges::to<trial_t>(std::views::iota(base_t(0), N)),
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

  if (argc != 2) {
    std::cerr << error <<
      "Exactly one argument (N)"
      " needed, but " << argc-1 << " provided.\n";
    return 1;
  }

  const base_t N = FP::to_unsigned<base_t>(argv[1]);

  std::cout
    << "  " << N << " " << FP::pow(N,N) << " " << FP::pow(N,N+1) << std::endl
    << PerformCount(N);
}
