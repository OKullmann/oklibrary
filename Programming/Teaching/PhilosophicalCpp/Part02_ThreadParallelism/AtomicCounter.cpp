// Oliver Kullmann, 17.11.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Part02_ThreadParallelism/AtomicCounter.cpp
  \brief Counting in parallel, and determining the current and the maximum
         number of parallel threads

  Usage:

> AtomicCounter [num_threads=20] [num_iterations=1000000]

Performs in each thread num_iterations many increments of a global counter.
Prints the maximum number of threads which did run in parallel.

*/

#include <string>
#include <cstdint>
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

namespace {

  typedef std::vector<std::thread> Thread_vt;

  typedef std::uint_fast64_t Count_t;
  typedef std::uint32_t CountThreads_t;

  constexpr CountThreads_t default_threads = 20;
  constexpr Count_t default_iterations = 1'000'000;

  typedef std::atomic<Count_t> AtomicCount_t;
  static_assert(AtomicCount_t::is_always_lock_free, "AtomicCount not lock-free.");

  class MaxThreads {
    CountThreads_t c=0, m=0;
  public :
    void inc() noexcept { if (++c > m) m = c; }
    void dec() noexcept { --c; }
    CountThreads_t count() const noexcept { return c; }
    CountThreads_t max() const noexcept { return m; }
  };
  inline MaxThreads inc(MaxThreads m) noexcept { m.inc(); return m; }
  inline MaxThreads dec(MaxThreads m) noexcept { m.dec(); return m; }
  typedef std::atomic<MaxThreads> AtomicMaxThreads;
  static_assert(AtomicMaxThreads::is_always_lock_free, "AtomicMaxThreads not lock-free.");


  inline void count(const Count_t N, AtomicCount_t& counter, AtomicMaxThreads& max) noexcept {
    for (Count_t i = 0; i < N;
         counter.fetch_add(1, std::memory_order_relaxed), ++i);
    for (auto m = max.load(); not max.compare_exchange_strong(m, dec(m)););
  }

}

int main(const int argc, const char* const argv[]) {
  const CountThreads_t num_threads = (argc > 1) ? std::stoul(argv[1]) : default_threads;
  const Count_t num_iterations = (argc > 2) ? std::stoull(argv[2]) : default_iterations;

  AtomicCount_t counter{0};
  AtomicMaxThreads max;
  Thread_vt v; v.reserve(num_threads);
  for (CountThreads_t i = 0; i < num_threads; ++i) {
    v.emplace_back(count, num_iterations, std::ref(counter), std::ref(max));
    for (auto m = max.load(); not max.compare_exchange_strong(m, inc(m)););
  }
  for (auto& t : v) t.join();

  const Count_t final = num_threads * num_iterations;
  if (counter != final) {
    std::cerr << "Error: counter = " << counter << " != final = " << final << ".\n";
    return 1;
  }
  if (max.load().count() != 0) {
    std::cerr << "Error: max.load().count() = " << max.load().count() << ".\n";
    return 1;
  }
  std::cout << max.load().max() << "\n";
}
