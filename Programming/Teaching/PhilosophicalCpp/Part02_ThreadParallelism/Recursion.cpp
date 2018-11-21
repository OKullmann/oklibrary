// Oliver Kullmann, 18.11.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file PhilosophicalCpp/Part02_ThreadParallelism/Recursion.cpp
  \brief Parallel recursion for Fibonacci-numbers, as simple and efficient as possible

  Usage:

> Recursion [N=50] [mode=1] [max_depth=calculated]

The other mode is 0, without parallelism.

In case of parallel computation, prints N and max_depth.

*/

#include <string>
#include <iostream>
#include <future>

#include <cmath>
#include <cstdint>

namespace {

  enum class Error {
    nonpar=1,
    par=2
  };
  template <typename EC>
  inline constexpr int code(const EC e) noexcept {return static_cast<int>(e);}

  enum class RecMode {
    nonpar=0,
    par=1
  };
  constexpr RecMode default_recmode = RecMode::par;
  RecMode to_RecMode(const std::string& in) {
    if (in == "0") return RecMode::nonpar;
    else return default_recmode;
  }

  typedef std::uint_fast64_t Result_t;
  typedef std::uint32_t Argument_t;
  constexpr Argument_t default_N = 50;

  constexpr Argument_t add_depth = 3;
  inline constexpr Argument_t depth(const Argument_t num_threads) noexcept {
    if (num_threads <= 1) return 0; else
    return std::ceil(std::log2(num_threads)) + add_depth;
  }

  inline constexpr Result_t fibo_direct(Argument_t n) noexcept {
    if (n <= 1) return n;
    Result_t a0=0, a1=1;
    for (--n; n != 0; --n) { const auto old_a1 = a1; a1 += a0; a0 = old_a1; }
    return a1;
  }
  static_assert(fibo_direct(20) == 6765);

  // Now the recursive function:
  constexpr Argument_t threshold = 5;
  static_assert(threshold >= 1, "threshold too small");
  inline constexpr Result_t fibo_rec0(const Argument_t n) noexcept {
    return (n <= threshold) ? fibo_direct(n) : fibo_rec0(n-2) + fibo_rec0(n-1);
  }
  static_assert(fibo_rec0(21) == 10946);
  inline Result_t fibo_rec(const Argument_t n, Argument_t max_depth) noexcept {
    if (n <= threshold) return fibo_direct(n);
    if (max_depth-- == 0) return fibo_rec0(n);
    auto future = std::async(std::launch::async, fibo_rec, n-2, max_depth);
    const Result_t res = fibo_rec(n-1, max_depth);
    return future.get() + res;
  }

}

int main(const int argc, const char* const argv[]) {
  const Argument_t N = (argc > 1) ? std::stoul(argv[1]) : default_N;
  const Result_t F = fibo_direct(N);
  const RecMode recmode = (argc > 2) ? to_RecMode(argv[2]) : default_recmode;

  if (recmode == RecMode::nonpar) {
    const Result_t F2 = fibo_rec0(N);
    if (F != F2) {
      std::cerr << "Error: Recursive result (non-parallel) is " << F2 << ", but should be " << F << "\n";
      return code(Error::nonpar);
    }
    std::cout << N << "\n";
    return 0;
  }

  const Argument_t max_depth = (argc > 3) ? std::stoul(argv[3]) : depth(std::thread::hardware_concurrency());
  const Result_t F2 = fibo_rec(N, max_depth);

  if (F != F2) {
    std::cerr << "Error: Recursive result (parallel) is " << F2 << ", but should be " << F << "\n";
    return code(Error::par);
  }
  std::cout << N << " " << max_depth << "\n";

}
