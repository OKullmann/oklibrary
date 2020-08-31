// Oliver Kullmann, 31.8.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef SYMMETRYBREAKING_Ho91SyFVmW
#define SYMMETRYBREAKING_Ho91SyFVmW

#include <vector>
#include <future>

#include <cassert>

#include "Dimensions.hpp"
#include "Statistics.hpp"
#include "Board.hpp"
#include "Backtracking.hpp"

namespace SymmetryBreaking {

  constexpr Statistics::count_t width_branching() noexcept {
    constexpr Statistics::count_t N = Dimensions::N;
    if constexpr (N % 2 == 1) {
      if constexpr (N <= 5) return 0;
      else return ((N-1) * (N-3)) / 8 + (N/2 - 1);
    }
    else {
      if constexpr (N <= 2) return 0;
      else if constexpr (N == 4) return 1;
      else return N/2;
    }
  }

  typedef std::vector<std::future<Statistics::NodeCounts>> job_v;
  typedef std::vector<Statistics::AnnotatedNodeCount> result_v;

  void branching(job_v& jobs, result_v& results, Statistics::NodeCounts& res) noexcept {
    using namespace Dimensions;

    if constexpr (N % 2 == 1) {
      const sizet mid = N/2;
      if (N <= 3) {
        for (sizet i = 0; i <= mid; ++i) {
          const Board::square_v branch{{0,i}};
          Board::DoubleSweep B(branch);
          Statistics::NodeCounts s(false);
          if (i != mid) s.set_duplication(2);
          if (not B.ucp(s)) {
            jobs.push_back(std::async(std::launch::async,
                                      Backtracking::count_init<bt>, B));
            results.emplace_back(s, branch);
          }
          else res += s;
        }
      }
      else {
        assert(N >= 5);
        for (sizet i = 0; i < mid-1; ++i)
          for (sizet j = i+1; j < mid; ++j) {
            const Board::square_v branch{{mid,i},{j,mid}};
            Board::DoubleSweep B(branch);
            assert(not B.completed());
            Statistics::NodeCounts s(false);
            s.set_duplication(8);
            if (not B.ucp(s)) {
              jobs.push_back(std::async(std::launch::async,
                                        Backtracking::count_init<bt>, B));
              results.emplace_back(s, branch);
            }
            else res += s;
          }
        for (sizet j = 0; j < mid-1; ++j) {
          const Board::square_v branch{{mid,mid},{mid-1,j}};
          Board::DoubleSweep B(branch);
          assert(not B.completed());
          Statistics::NodeCounts s(false);
          s.set_duplication(2);
          if (not B.ucp(s)) {
            jobs.push_back(std::async(std::launch::async,
                                      Backtracking::count_init<bt>, B));
            results.emplace_back(s,branch);
          }
          else res += s;
        }
      }
    } else {
      assert(N % 2 == 0);
      for (sizet i = 0; i < N/2; ++i) {
        const Board::square_v branch{{0,i}};
        Board::DoubleSweep B(branch);
        Statistics::NodeCounts s(false);
        s.set_duplication(2);
        if (not B.ucp(s)) {
          jobs.push_back(std::async(std::launch::async,
                                    Backtracking::count_init<bt>, B));
          results.emplace_back(s,branch);
        }
        else res += s;
      }
    }

    assert(jobs.size() == results.size());
    assert(N > 3 or jobs.empty());
    assert(results.size() == width_branching());

  }

}

#endif
