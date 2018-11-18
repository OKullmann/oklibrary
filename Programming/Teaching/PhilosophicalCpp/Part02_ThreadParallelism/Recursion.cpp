// Oliver Kullmann, 18.11.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Part02_ThreadParallelism/Recursion.cpp
  \brief Parallel recursion for Fibonacci-numbers

  Usage:

> Recursion [N=42] [num_threads=20]

Prints the observed maximal number of parallel threads.

*/

#include <string>
#include <iostream>
#include <future>
#include <atomic>
#include <algorithm>

#include <cstdint>

namespace {

  typedef std::uint_fast64_t Result_t;
  typedef std::uint32_t Argument_t;
  constexpr Argument_t default_N = 42;

  typedef std::uint32_t CountThreads_t;
  constexpr CountThreads_t default_threads = 20;
  typedef std::atomic<CountThreads_t> AtomicCountThreads_t;
  static_assert(AtomicCountThreads_t::is_always_lock_free, "AtomicCountThreads is not lock-free.");

  constexpr inline Result_t fibo_direct(Argument_t n) noexcept {
    if (n <= 1) return n;
    Result_t a0=0, a1=1;
    while (n-- > 1) {
      const Result_t sum = a0 + a1;
      a0 = a1; a1 = sum;
    }
    return a1;
  }
  static_assert(fibo_direct(20) == 6765);

  // Now the recursive function; inefficiently, each invocation queries the
  // atomic counter cnt, so that we can compute observed_max -- in a real
  // implementation, cnt should be guarded by for example an depth-guard:
  constexpr Argument_t threshold = 5;
  Result_t fibo_rec(const Argument_t n, const CountThreads_t max, AtomicCountThreads_t& cnt, CountThreads_t& observed_max, const bool new_thread) noexcept {
    // The following just exemplifies how to avoid creating too small threads:
    if (n <= threshold) { if (new_thread) --cnt; return fibo_direct(n); }

    const CountThreads_t num_threads = cnt;
    observed_max = std::max(observed_max, num_threads);
    if (num_threads < max) {
      ++cnt;
      auto handle1 = std::async(std::launch::async, fibo_rec, n-1, max, std::ref(cnt), std::ref(observed_max), true);
      const Result_t res0 = fibo_rec(n-2, max, cnt, observed_max, false);
      const Result_t res1 = handle1.get();
      if (new_thread) --cnt;
      return res0 + res1;
    }
    else {
      const Result_t res0 = fibo_rec(n-2, max, cnt, observed_max, false);
      const Result_t res1 = fibo_rec(n-1, max, cnt, observed_max, false);
      if (new_thread) --cnt;
      return res0 + res1;
    }
  }

}

int main(const int argc, const char* const argv[]) {
  const Argument_t N = (argc > 1) ? std::stoul(argv[1]) : default_N;
  const Result_t F = fibo_direct(N);
  const CountThreads_t num_threads = (argc > 2) ? std::stoul(argv[2]) : default_threads;

  AtomicCountThreads_t cnt_threads{1};
  CountThreads_t observed_max = 0;
  const Result_t F2 = fibo_rec(N, num_threads, cnt_threads, observed_max, false);

  if (F != F2) {
    std::cerr << "Error: Recursive result is " << F2 << ", but should be " << F << "\n";
    return 1;
  }
  if (cnt_threads != 1) {
    std::cerr << "Error: cnt_threads = " << cnt_threads << ".\n";
    return 1;
  }
  std::cout << observed_max << "\n";

}
