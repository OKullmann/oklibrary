// Oliver Kullmann, 24.11.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file PhilosophicalCpp/Part02_ThreadParallelism/Sum.cpp
  \brief Parallel summation

  Usage:

> Recursion [N=100] [mode=1] [num_threads=available] [max_reps=1000] [seed=0] [base=6]

The other mode is 0, without parallelism.

*/

#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <type_traits>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>

#include <cmath>
#include <cstdint>
#include <cassert>

#include "RandGen.hpp"

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
  constexpr RecMode recmode_default = RecMode::par;
  RecMode to_RecMode(const std::string& in) {
    if (in == "0") return RecMode::nonpar;
    else return recmode_default;
  }

  typedef std::uint32_t NumThreads_t;
  constexpr NumThreads_t tasks_default = 100;


  typedef std::uint_fast64_t Result_t;
  constexpr Result_t base_default = 1'000'000;
  typedef std::uint8_t Exponent_t;
  constexpr Exponent_t exp_default = 6;
  static_assert(std::pow(10,6) == base_default);

  class Task {
    static Result_t base;
    const Result_t reps;
  public :
    static void set_base(const Exponent_t e = exp_default) noexcept {
      base = std::pow(10,e);
    }
    static Result_t base_value() noexcept { return base; }
    Task() = default;
    constexpr Task(const Result_t reps) noexcept : reps(reps) {}
    constexpr Result_t repetitions() const noexcept { return reps; }
    Result_t operator()() const noexcept {
      Result_t sum = 0;
      const Result_t N = base * reps;
      for (Result_t i = 1; i <= N; ++i) sum += i;
      return sum;
    }
    Result_t fast() const noexcept {
      const Result_t N = base * reps;
      return (N * (N+1)) / 2;
    }
  };
  Result_t Task::base = base_default;

  inline constexpr bool operator <(const Task t1, const Task t2) noexcept {
    return t1.repetitions() < t2.repetitions();
  }

  static_assert(std::is_pod<Task>::value);

  typedef std::vector<std::pair<const Task, Result_t>> TaskVector;
  typedef TaskVector::pointer TaskPointer;
  struct CompTaskPointer {
    bool operator()(const TaskPointer p1, const TaskPointer p2) const noexcept {
      return p1->first < p2->first;
    }
  };
  typedef std::priority_queue<TaskPointer, std::vector<TaskPointer>, CompTaskPointer> TaskQueue;


  using seed_t = RandGen::seed_t;
  constexpr seed_t seed_default = 0;
  constexpr Result_t reps_default = 1000;
  TaskVector create_experiment(const NumThreads_t tasks, const Result_t max_reps, const seed_t s) {
    TaskVector v; v.reserve(tasks);
    RandGen::vec_seed_t sv{s};
    std::seed_seq seq(sv.begin(), sv.end());
    RandGen::randgen_t g(seq);
    RandGen::Uniform U(g, max_reps);
    for (NumThreads_t i = 0; i < tasks; ++i) v.emplace_back(U(),0);
    return v;
  }
  inline Result_t fast_evaluation(const TaskVector& v) noexcept {
    Result_t sum = 0;
    for (const auto& x : v) sum += x.first.fast();
    return sum;
  }
  inline Result_t slow_evaluation(const TaskVector& v) noexcept {
    Result_t sum = 0;
    for (const auto& x : v) sum += x.first();
    return sum;
  }


  TaskQueue Q;
  NumThreads_t running;
  std::mutex mQ;
  std::condition_variable finished;

  inline void create_queue(const TaskVector& v) {
    const auto begin = const_cast<TaskPointer>(v.data());
    const auto end = v.data() + v.size();
    for (TaskPointer p = begin; p != end; ++p) Q.push(p);
  }

  struct WrapTask {
    const Task t;
    Result_t& r;
    TaskQueue& Q;
    NumThreads_t& run;
    std::mutex& mQ;
    std::condition_variable& f;
    WrapTask(const TaskPointer p, TaskQueue& Q, NumThreads_t& run, std::mutex& mQ, std::condition_variable& f) noexcept : t(p->first), r(p->second), Q(Q), run(run), mQ(mQ), f(f) {}
    void operator()() {
      r = t();
      mQ.lock();
      if (--run == 0) { f.notify_one(); return; }
      if (Q.empty()) {mQ.unlock(); return;}
      const TaskPointer p = Q.top(); Q.pop();
      mQ.unlock();
      std::thread(WrapTask(p,Q,run,mQ,f)).detach();
    }
  };

  Result_t recombine(const TaskVector& tasks) noexcept {
    Result_t sum = 0;
    for (const auto& x : tasks) sum += x.second;
    return sum;
  }

  Result_t parallel_evaluation(TaskVector& tasks, const NumThreads_t num_threads) {
    create_queue(tasks);
    const NumThreads_t N = tasks.size();
    mQ.lock();
    running = N;
    const NumThreads_t run_now = std::min(num_threads, N);
    for (NumThreads_t i = 0; i < run_now; ++i) {
      const TaskPointer p = Q.top(); Q.pop();
      std::thread(WrapTask(p,Q,running,mQ,finished)).detach();
    }
    mQ.unlock();
    {std::mutex m; std::unique_lock l(m); finished.wait(l);}
    return recombine(tasks);
  }

}

int main(const int argc, const char* const argv[]) {
  const NumThreads_t num_tasks = (argc > 1) ? std::stoul(argv[1]) : tasks_default;
  const RecMode recmode = (argc > 2) ? to_RecMode(argv[2]) : recmode_default;
  const NumThreads_t num_threads = (argc > 3) ? std::stoul(argv[3]) : std::thread::hardware_concurrency();
  const Result_t max_reps = (argc > 4) ? std::stoull(argv[4]) : reps_default;
  const seed_t seed = (argc > 5) ? std::stoul(argv[5]) : seed_default;
  if (argc > 6) {
    const Exponent_t exp = std::stoul(argv[6]);
    Task::set_base(exp);
  }

  TaskVector tasks = create_experiment(num_tasks, max_reps, seed);
  const Result_t total_sum = fast_evaluation(tasks);

  if (recmode == RecMode::nonpar) {
    const Result_t result = slow_evaluation(tasks);
    if (result != total_sum) {
      std::cerr << "Error: Summation (non-parallel) is " << result << ", but should be " << total_sum << "\n";
      return code(Error::nonpar);
    }
    return 0;
  }

  const Result_t result = parallel_evaluation(tasks, num_threads);
  if (result != total_sum) {
    std::cerr << "Error: Summation (parallel) is " << result << ", but should be " << total_sum << "\n";
    return code(Error::par);
  }

}
