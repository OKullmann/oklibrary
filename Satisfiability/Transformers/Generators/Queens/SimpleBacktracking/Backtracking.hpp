// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The backtracking-algorithm for the 2-sweep scheme

TODOS:

1. Eliminate the recursion in function count(Board<R>& B):
    - One big loop, with a two-dimensional movement, "vertically" (recursive
      call or backtracking), and "horizontally" (within the current row).
    - An array of size N contains the state of the current board. which is
      a triple (start-board, current state of row, board after ucp) concerning
      the board.
    - The current statistics is the fourth data-member of those stack-
      elements.

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

}

#endif
