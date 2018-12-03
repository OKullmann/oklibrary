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
  1 <= multiplier is 10^exponent.
  Using num_threads-many threads for the summations
  (plus the  waiting master-thread). If a thread is finished, the replacement
  is taking from the priority-queue of waiting tasks, choosing a task with
  maximal reps.

  Usage:

> Recursion [N=100] [mode=1] [num_threads=available] [max_reps=1000] [seed=0] [exponent=6]

The other mode is 0, without parallelism.

The results shown are performed via arithmetic in
  typedef std::uint_fast64_t Result_t;
and are correct modulo 2^n for n >= 64 the number of bits of Result_t.


TODOS:

1. Timings:

csltok:

> git checkout 8aa1ade5657bd5ee6650dbe70345cb0153a18a37 Sum.cpp
> make
> time ./Sum
N=100, mode=par(1), num_threads=4, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m6.638s
user    0m26.114s
sys     0m0.020s
> time ./Sum 100 0
N=100, mode=nonpar(0), (num_threads=4), max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m20.284s
user    0m20.233s
sys     0m0.003s
> time ./Sum 100 1 2
N=100, mode=par(1), num_threads=2, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m10.804s
user    0m21.515s
sys     0m0.015s

> git checkout e6c2307e1814975dcacfb07aa1a676c1c6799ccf Sum.cpp
> time ./Sum
N=100, mode=par(1), num_threads=4, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m6.357s
user    0m25.014s
sys     0m0.023s
> time ./Sum 100 0
N=100, mode=nonpar(0), (num_threads=4), max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m20.961s
user    0m20.910s
sys     0m0.002s
> time ./Sum 100 1 1
N=100, mode=par(1), num_threads=1, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m19.524s
user    0m19.458s
sys     0m0.022s
> time ./Sum 100 1 2
N=100, mode=par(1), num_threads=2, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m10.343s
user    0m20.609s
sys     0m0.019s

> git checkout 0ef3e32b639cb957977b203efd9dfc3b94add67e Sum.cpp
> time ./Sum
N=100, mode=par(1), num_threads=4, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
range = (2, 516.94, 1000), sd = 283.575
real    0m6.343s
user    0m24.993s
sys     0m0.007s
> time ./Sum 100 0
N=100, mode=nonpar(0), (num_threads=4), max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
range = (2, 516.94, 1000), sd = 283.575
real    0m21.004s
user    0m20.954s
sys     0m0.002s
> time ./Sum 100 1 1
N=100, mode=par(1), num_threads=1, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
range = (2, 516.94, 1000), sd = 283.575
real    0m19.636s
user    0m19.586s
sys     0m0.002s
> time ./Sum 100 1 2
N=100, mode=par(1), num_threads=2, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
range = (2, 516.94, 1000), sd = 283.575
real    0m10.292s
user    0m20.522s
sys     0m0.007s

(A bit strange that now the non-parallel computation got a bit slower.)

On csverify:

$ git checkout e6c2307e1814975dcacfb07aa1a676c1c6799ccf Sum.cpp
$ time ./Sum
N=100, mode=par(1), num_threads=12, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m0.631s
user    0m6.882s
sys     0m0.000s
$ time ./Sum 100 0
N=100, mode=nonpar(0), (num_threads=12), max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m6.613s
user    0m6.609s
sys     0m0.004s
$ time ./Sum 100 1 1
N=100, mode=par(1), num_threads=1, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m3.487s
user    0m3.486s
sys     0m0.001s
$ time ./Sum 100 1 2
N=100, mode=par(1), num_threads=2, max_reps=1000, seed=0, multiplier=1000000=10^6, result=17503680526003500000
real    0m1.765s
user    0m3.527s
sys     0m0.000s

The -march=native options has improved the runtime by nearly 100%.
But peculiar, that the direct computation improved much less, and is now
slower than just using one thread (which is the same computation).
Same effect also on csltok, but much weaker.
Again a weak compilation.


2. Running the default values, but with various seeds, reveals a large
variation in runtime, nearly 30%, which is surprising.

That seems to just directly correleated with the average reps-value.


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
#include <numeric>

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
    inline static Result_t multiplier = multiplier_default;
    inline static Result_t md2 = multiplier / 2;
    const Result_t reps;
  public :
    static void set_multiplier(const Exponent_t e = exp_default) noexcept {
      assert(e >= 1);
      multiplier = std::pow(10,e); md2 = multiplier / 2;
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
      assert(multiplier % 2 == 0);
      const Result_t Nd2 = md2 * reps;
      const Result_t Np1 = Nd2 * 2 + 1;
      return Nd2 * Np1;
    }
  };

  inline constexpr bool operator <(const Task t1, const Task t2) noexcept {
    return t1.repetitions() < t2.repetitions();
  }

  static_assert(std::is_pod_v<Task>);


  /* ****** The parallel computation ****** */

  // The vector of (task, slot for result):
  typedef std::vector<std::pair<const Task, Result_t>> TaskVector;
  inline Result_t direct_evaluation(const TaskVector& v) noexcept {
    Result_t sum = 0;
    for (const auto& x : v) sum += x.first.direct();
    return sum;
  }
  inline Result_t nonparallel_evaluation(const TaskVector& v) noexcept {
    // Once g++ has it, one can give it a try:
    //return std::transform_reduce(v.begin(), v.end(), Result_t(0),
    //  std::plus<>(), [](const auto& x) noexcept { return x.first(); });
    Result_t sum = 0;
    for (const auto& x : v) sum += x.first();
    return sum;
  }
  inline Result_t recombine(const TaskVector& tasks) noexcept {
    Result_t sum = 0;
    for (const auto& x : tasks) sum += x.second;
    return sum;
  }

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
  struct Statistics {
    double min, max, mean, sd;
    Statistics(const TaskVector& v) noexcept : min(v.front().first.repetitions()), max(min), mean(0), sd(0) {
      assert(not v.empty());
      for (auto i = v.begin()+1; i != v.end(); ++i) {
        const auto val = i->first.repetitions();
        if (val > max) max = val;
        else if (val < min) min = val;
        mean += val;
      }
      mean /= v.size();
      for (auto i = v.begin(); i != v.end(); ++i) {
        const auto val = i->first.repetitions();
        const auto diff = val - mean;
        sd += diff*diff;
      }
      sd = std::sqrt(sd/(v.size() - 1));
    }
  };
  std::ostream& operator <<(std::ostream& out, const Statistics& s) {
    return out << "range = (" << s.min << ", " << s.mean << ", " << s.max << "), sd = " << s.sd;
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
    TaskPointer p;
  public :
    WrapTask(const TaskPointer p) noexcept : p(p) {}

    void operator()() noexcept {
      while (true) {
        p->second = (p->first)();
        std::lock_guard g(mQ);
        if (--running == 0) { finished.notify_one(); return; }
        if (Q.empty()) return;
        p = Q.toppop();
      }
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
       std::thread(WrapTask(p)).detach();
     }
    } mQ.unlock();
    {std::unique_lock l(mQ); finished.wait(l, []{return running == 0;});}
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
  std::cout << Statistics(tasks) << "\n";

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
