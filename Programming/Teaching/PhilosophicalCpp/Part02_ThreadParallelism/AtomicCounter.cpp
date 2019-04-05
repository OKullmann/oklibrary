// Oliver Kullmann, 17.11.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
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
#include <type_traits>

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.0",
        "5.4.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Teaching/PhilosophicalCpp/Part02_ThreadParallelism/AtomicCounter.cpp",
        "GPL v3"};

  typedef std::vector<std::thread> Thread_vt;

  typedef std::uint_fast64_t Count_t;
  typedef std::uint32_t CountThreads_t;

  constexpr CountThreads_t default_threads = 20;
  constexpr Count_t default_iterations = 1'000'000;

  typedef std::atomic<CountThreads_t> AtomicCountThreads_t;
  static_assert(AtomicCountThreads_t::is_always_lock_free);

  typedef std::atomic<Count_t> AtomicCount_t;
  static_assert(AtomicCount_t::is_always_lock_free);

  class MaxThreads {
    CountThreads_t c=0, m=0;
  public :
    void inc() noexcept { if (++c > m) m = c; }
    void dec() noexcept { --c; }
    CountThreads_t count() const noexcept { return c; }
    CountThreads_t max() const noexcept { return m; }
  };
  static_assert(std::is_trivially_copyable_v<MaxThreads>);
  static_assert(not std::is_pod_v<MaxThreads>);
  inline MaxThreads inc(MaxThreads m) noexcept { m.inc(); return m; }
  inline MaxThreads dec(MaxThreads m) noexcept { m.dec(); return m; }
  typedef std::atomic<MaxThreads> AtomicMaxThreads;
  static_assert(AtomicMaxThreads::is_always_lock_free);


  inline void count(const Count_t N, AtomicCount_t& counter, AtomicCountThreads_t& cnt_threads, AtomicMaxThreads& max) noexcept {
    for (Count_t i = 0; i < N;
         counter.fetch_add(1, std::memory_order_relaxed), ++i);
    cnt_threads.fetch_add(1, std::memory_order_relaxed);
    for (auto m = max.load(); not max.compare_exchange_strong(m, dec(m)););
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;

  const CountThreads_t num_threads = (argc > 1) ? std::stoul(argv[1]) : default_threads;
  const Count_t num_iterations = (argc > 2) ? std::stoull(argv[2]) : default_iterations;

  AtomicCount_t counter{0};
  AtomicCountThreads_t cnt_threads{0};
  AtomicMaxThreads max;

  Thread_vt v; v.reserve(num_threads);
  for (CountThreads_t i = 0; i < num_threads; ++i) {
    v.emplace_back(count, num_iterations, std::ref(counter), std::ref(cnt_threads), std::ref(max));
    cnt_threads.fetch_sub(1, std::memory_order_relaxed);
    for (auto m = max.load(); not max.compare_exchange_strong(m, inc(m)););
  }
  for (auto& t : v) t.join();

  const Count_t final = num_threads * num_iterations;
  if (counter != final) {
    std::cerr << "Error: counter = " << counter << " != final = " << final << ".\n";
    return 1;
  }
  if (cnt_threads != 0) {
    std::cerr << "Error: cnt_threads = " << cnt_threads << ".\n";
    return 1;
  }
  if (max.load().count() != 0) {
    std::cerr << "Error: max.load().count() = " << max.load().count() << ".\n";
    return 1;
  }
  std::cout << max.load().max() << "\n";
}
