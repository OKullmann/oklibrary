// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The backtracking-algorithm for the 2-sweep scheme

TODOS:

*/

#ifndef BACKTRACKING_Lr3X60Dlry
#define BACKTRACKING_Lr3X60Dlry

#include <type_traits>

#include "Statistics.hpp"
#include "Rows.hpp"
#include "Board.hpp"

namespace Backtracking {

  Statistics::NodeCounts count(const Board::DoubleSweep& B) noexcept {
    typedef Rows::Row Row;
    typedef Board::DoubleSweep board_t;
    static_assert(std::is_trivially_copyable_v<board_t>);
    Statistics::NodeCounts res(true);
    for (const Row new_row : B.cbr()) {
      board_t Bj(B);
      Bj.set_cbr(new_row);
      if (not Bj.ucp(res)) res += count(Bj);
    }
    return res;
  }


  // The nonrecursive version:

  struct State {
    typedef Board::DoubleSweep board;
    typedef Statistics::NodeCounts stats;
    typedef typename Rows::Row::iterator iterator;
    board b;
    stats s;
    iterator it;
  };
  static_assert(std::is_trivially_copyable_v<State>);

  constexpr Dimensions::sizet max_size_stack = Dimensions::N-1;
  using Stack = std::array<State, Dimensions::N>;

  Statistics::NodeCounts countnr(const Board::DoubleSweep& B) noexcept {
    assert(Dimensions::N >= 4);
    typedef typename State::stats stats_t;
    typedef typename State::iterator iterator_t;

    Stack S{{B, stats_t(true), B.cbr().begin()}};
    assert(S.size() == max_size_stack+1);
    for (Dimensions::sizet i = 0;;) {
      assert(i < max_size_stack);
      assert(S[i].it != iterator_t());
      State& current = S[i];
      State& next = S[i+1];
      next.b = current.b;
      next.b.set_cbr(*current.it);
      ++current.it;
      if (not next.b.ucp(current.s)) {
        ++i;
        assert(i < max_size_stack);
        next.s = stats_t(true);
        next.it = next.b.cbr().begin();
      }
      else {
        while (not (S[i].it != iterator_t())) {
          if (i == 0) return S[0].s;
          --i;
          S[i].s += S[i+1].s;
        }
      }
    }
  }

}

#endif
