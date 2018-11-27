// Oliver Kullmann, 24.11.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file PhilosophicalCpp/Part02_ThreadParallelism/Sum.cpp
  \brief Parallel summation

  Computes N sums
    1 + ... + reps*multiplier,
  where 0 <= reps <= max_reps is pseudo-random, and the default for
  multiplier is 10^exponent.
  Using num_threads-many threads for the summations
  (plus the  waiting master-thread). If a thread is finished, the replacement
  is taking from the priority-queue of waiting tasks, choosing a task with
  maximal reps.

  Usage:

> Recursion [N=100] [mode=1] [num_threads=available] [max_reps=1000] [seed=0] [exponent=6]

The other mode is 0, without parallelism.

TODOS:

1. Currently, in the callback of WrapTask, if one task is finished, and there
   is a task on the queue, then a new detached thread is created.
   Instead it should be more efficient, and easy to do, to have such a
   wrapped task running a loop, where then the new task is run in the
   same thread.

   One needs first to establish the efficiency of the current method, and then
   with the new method, to measure the difference.

*/

#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <type_traits>
#include <vector>
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
  RecMode to_RecMode(const std::string& in) noexcept {
    if (in == "0") return RecMode::nonpar;
    else return recmode_default;
  }
  std::ostream& operator <<(std::ostream& out, const RecMode r) {
    switch (r) {
      case RecMode::nonpar : return out << "nonpar";
      default : return out << "par";
    }
  }


  /* ****** The tasks ****** */

  typedef std::uint_fast64_t Result_t;
  constexpr Result_t multiplier_default = 1'000'000;
  typedef std::uint8_t Exponent_t;
  constexpr Exponent_t exp_default = 6;
  static_assert(std::pow(10,6) == multiplier_default);

  // Functors for performing the summation 1 + ... + multiplier*reps:
  class Task {
    static Result_t multiplier;
    const Result_t reps;
  public :
    static void set_multiplier(const Exponent_t e = exp_default) noexcept {
      multiplier = std::pow(10,e);
    }
    static Result_t multiplier_value() noexcept { return multiplier; }
    Task() = default;
    constexpr Task(const Result_t reps) noexcept : reps(reps) {}
    constexpr Result_t repetitions() const noexcept { return reps; }
    Result_t operator()() const noexcept {
      Result_t sum = 0;
      const Result_t N = multiplier * reps;
      for (Result_t i = 1; i <= N; ++i) sum += i;
      return sum;
    }
    Result_t direct() const noexcept {
      const Result_t N = multiplier * reps;
      return (N * (N+1)) / 2;
    }
  };
  Result_t Task::multiplier = multiplier_default;

  inline constexpr bool operator <(const Task t1, const Task t2) noexcept {
    return t1.repetitions() < t2.repetitions();
  }

  static_assert(std::is_pod_v<Task>);


  /* ****** The parallel computation ****** */

  // The vector of (task, slot for result):
  typedef std::vector<std::pair<const Task, Result_t>> TaskVector;
  typedef TaskVector::pointer TaskPointer;

  typedef std::uint32_t NumThreads_t;
  constexpr NumThreads_t tasks_default = 100;

  using seed_t = RandGen::seed_t;
  constexpr seed_t seed_default = 0;
  constexpr Result_t reps_default = 1000;

  // Creating tasks many Task(i), for 0 <= i < max_reps, pseudo-random:
  TaskVector create_experiment(const NumThreads_t tasks, const Result_t max_reps, const seed_t s) {
    TaskVector v; v.reserve(tasks);
    RandGen::vec_seed_t sv{s};
    std::seed_seq seq(sv.begin(), sv.end());
    RandGen::randgen_t g(seq);
    RandGen::Uniform U(g, max_reps);
    for (NumThreads_t i = 0; i < tasks; ++i) v.emplace_back(U(),0);
    return v;
  }

  inline Result_t direct_evaluation(const TaskVector& v) noexcept {
    Result_t sum = 0;
    for (const auto& x : v) sum += x.first.direct();
    return sum;
  }
  inline Result_t nonparallel_evaluation(const TaskVector& v) noexcept {
    Result_t sum = 0;
    for (const auto& x : v) sum += x.first();
    return sum;
  }
  inline Result_t recombine(const TaskVector& tasks) noexcept {
    Result_t sum = 0;
    for (const auto& x : tasks) sum += x.second;
    return sum;
  }

  template <typename T>
  class TaskQueue {
    typedef std::vector<T> vec_t;
    vec_t q;
    typedef typename vec_t::const_pointer pointer_t;
    pointer_t top = nullptr, begin = nullptr;
    bool empty_ = true;
  public :
    typedef T value_type;
    TaskQueue() = default;
    TaskQueue(const TaskQueue&) = delete;
    template <class V>
    void initialise(const V& v) {
      assert(not v.empty());
      const auto N = v.size();
      q.reserve(N);
      const auto v_begin = const_cast<T>(v.data());
      const auto v_end = v.data() + N;
      for (T p = v_begin; p != v_end; ++p) q.push_back(p);
      std::sort(q.begin(), q.end(), [](const T p1, const T p2) noexcept {return p1->first < p2->first;});
      begin = q.data();
      empty_ = false;
      top = begin + N - 1;
    }
    bool empty() const noexcept { return empty_; }
    T toppop() noexcept {
      assert(not empty_);
      const T res = *top;
      if (top == begin) empty_ = true; else --top;
      return res;
    }
  };
  using TQ = TaskQueue<TaskPointer>;
  TQ Q;

  NumThreads_t running; // counter from num_threads to 0
  // Guard for parallel access to variables Q, running:
  std::mutex mQ;

  std::condition_variable finished; // channel for communicating with the master thread

  /*
    Wrapping a task, so that it can be executed in parallel (as detached
    thread); delivers the result into its slot, finishes parallel_evaluation
    once the last task is completed, and upon exit, runs a new wrapped task
    in a new detached thread from the queue, if the queue is not empty:
  */
  class WrapTask {
    const Task t;
    Result_t& r;
    TQ& Q;
    NumThreads_t& running;
    std::mutex& mQ;
    std::condition_variable& finished;
  public :
    WrapTask(const TaskPointer p, TQ& Q, NumThreads_t& r, std::mutex& mQ, std::condition_variable& f) noexcept : t(p->first), r(p->second), Q(Q), running(r), mQ(mQ), finished(f) {}

    void operator()() {
      r = t();
      mQ.lock();
      if (--running == 0) { finished.notify_one(); return; }
      if (Q.empty()) {mQ.unlock(); return;}
      const TaskPointer p = Q.toppop();
      mQ.unlock();
      std::thread(WrapTask(p,Q,running,mQ,finished)).detach();
    }
  };

  Result_t parallel_evaluation(TaskVector& tasks, const NumThreads_t num_threads) {
    Q.initialise(tasks);
    const NumThreads_t N = tasks.size();
    mQ.lock();
    {running = N;
     const NumThreads_t run_now = std::min(num_threads, N);
     for (NumThreads_t i = 0; i < run_now; ++i) {
       const TaskPointer p = Q.toppop();
       std::thread(WrapTask(p,Q,running,mQ,finished)).detach();
     }
    } mQ.unlock();
    {std::mutex dummy; std::unique_lock l(dummy); finished.wait(l);}
    return recombine(tasks);
  }


  void output_commandline(const NumThreads_t N, const RecMode r, const NumThreads_t T, const Result_t mr, const seed_t s, const Result_t res) {
    std::cout << "N=" << N << ", mode=" << r << "(" << code(r) << "), ";
    if (r == RecMode::par) std::cout << "num_threads=" << T;
    else std::cout << "(num_threads=" << T << ")";
    std::cout << ", max_reps=" << mr << ", seed=" << s << ", multiplier=";
    const auto mult = Task::multiplier_value();
    const Result_t e = std::round(std::log10(mult));
    std::cout << mult << "=10^" << e << ", result=" << res << "\n";
  }
}

int main(const int argc, const char* const argv[]) {
  const NumThreads_t num_tasks = (argc > 1) ? std::stoul(argv[1]) : tasks_default;
  assert(num_tasks >= 1);
  const RecMode recmode = (argc > 2) ? to_RecMode(argv[2]) : recmode_default;
  const NumThreads_t num_threads = (argc > 3) ? std::stoul(argv[3]) : std::thread::hardware_concurrency();
  assert(num_threads >= 1);
  const Result_t max_reps = (argc > 4) ? std::stoull(argv[4]) : reps_default;
  const seed_t seed = (argc > 5) ? std::stoul(argv[5]) : seed_default;
  if (argc > 6) {
    const Exponent_t exp = std::stoul(argv[6]);
    Task::set_multiplier(exp);
  }

  TaskVector tasks = create_experiment(num_tasks, max_reps, seed);
  const Result_t direct_sum = direct_evaluation(tasks);
  output_commandline(num_tasks, recmode, num_threads, max_reps, seed, direct_sum);

  if (recmode == RecMode::nonpar) {
    const Result_t result = nonparallel_evaluation(tasks);
    if (result != direct_sum) {
      std::cerr << "Error: Summation (non-parallel) is " << result << ", but should be " << direct_sum << "\n";
      return code(Error::nonpar);
    }
    return 0;
  }
  else {
    const Result_t result = parallel_evaluation(tasks, num_threads);
    if (result != direct_sum) {
      std::cerr << "Error: Summation (parallel) is " << result << ", but should be " << direct_sum << "\n";
      return code(Error::par);
    }
  }
}
