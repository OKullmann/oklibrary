// Oliver Kullmann, 4.7.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Components for parallel system calls (on POSIX systems)

  Namespace ParSysCalls, abbreviated "PSC".

  The main function is
    run_parallel(A, T),
  where A is a vector of commandline-calls, and T is the number of threads.
  Returned is a vector the same size as A, with the results of the
  system-calls.

*/

#ifndef PARSYSCALLS_l7FmxWXpDs
#define PARSYSCALLS_l7FmxWXpDs

#include <string>
#include <optional>
#include <vector>
#include <exception>
#include <algorithm>
#include <thread>

#include <cassert>
#include <cstdint>

// Guaranteed to be included:
#include "SystemCalls.hpp"

namespace ParSysCalls {

  struct EsystemArgs {
    std::string command, cin;
    bool cinexec = false;
  };

  typedef std::optional<SystemCalls::EReturnValue> res_t;
  typedef res_t* res_pt;

  struct SysCallComp {
    const EsystemArgs args;
    const res_pt target;

    SysCallComp(EsystemArgs args, const res_pt target) noexcept :
      args(args), target(target), next(nullptr) { assert(target); }

    void set_next(SysCallComp* const n) noexcept { assert(n); next = n; }

    void operator()() const {
      target->
        emplace(SystemCalls::esystem(args.command, args.cin, args.cinexec));
      if (next) next->operator()();
    }

  private :
    SysCallComp* next;
  };

  typedef std::vector<EsystemArgs> vargs_t;
  typedef std::vector<res_t> vres_t;

  typedef std::uint64_t count_t;

  vres_t run_parallel(const vargs_t& A, count_t T) {
    if (A.empty()) return {};
    if (T == 0)
      throw std::runtime_error("ParSysCalls::run_parallel: T = 0");
    const count_t N = A.size();
    T = std::min(T, N);
    vres_t res(N);
    std::vector<SysCallComp> computations; computations.reserve(N);
    for (count_t i = 0; i < N; ++i)
      computations.emplace_back(A[i], &res[i]);
    for (count_t i = 0; i + T < N; ++i)
      computations[i].set_next(&computations[i+T]);
    std::vector<std::thread> threads; threads.reserve(T);
    for (count_t i = 0; i < T; ++i)
      threads.emplace_back(computations[i]);
    for (std::thread& t : threads) {
      assert(t.joinable()); t.join();
    }
    return res;
  }

}

#endif
