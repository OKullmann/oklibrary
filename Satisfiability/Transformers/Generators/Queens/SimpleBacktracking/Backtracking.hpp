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

#include "Statistics.hpp"
#include "Board.hpp"

namespace Backtracking {

  template <class R, template <class> class ER>
  Statistics::NodeCounts count(const Board::DoubleSweep<R>& B) noexcept {
    Statistics::NodeCounts res(true);
    for (const R new_row : B.cbr()) {
      Board::DoubleSweep<R> Bj(B);
      Bj.set_cbr(new_row);
      Bj.template ucp<ER>(res);
      if (not Bj.satisfied() and not Bj.falsified()) res += count<R,ER>(Bj);
    }
    return res;
  }


  // The nonrecursive version:

  template <class R>
  struct State {
    typedef Board::DoubleSweep<R> board;
    typedef Statistics::NodeCounts stats;
    typedef typename R::iterator iterator;
    board b;
    stats s;
    iterator it;
  };

  constexpr Dimensions::size_t max_size_stack = Dimensions::N-1;
  template <class R>
  using Stack = std::array<State<R>, max_size_stack>;

  template <class R, template <class> class ER>
  Statistics::NodeCounts countnr(const Board::DoubleSweep<R>& B) noexcept {
    assert(Dimensions::N >= 4);
    typedef State<R> state_t;
    typedef typename state_t::stats stats_t;
    typedef typename state_t::board board_t;
    typedef typename state_t::iterator iterator_t;

    Stack<R> S{{B, stats_t(true), B.cbr().begin()}};
    assert(S.size() == max_size_stack);
    for (Dimensions::size_t i = 0;;) {
      assert(i < max_size_stack);
      assert(S[i].it != iterator_t());
      board_t b(S[i].b);
      b.set_cbr(*S[i].it);
      b.template ucp<ER>(S[i].s);
      ++S[i].it;
      if (not b.satisfied() and not b.falsified()) {
        ++i;
        assert(i < max_size_stack);
        S[i] = {b, stats_t(true), b.cbr().begin()};
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
