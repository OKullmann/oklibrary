// Oliver Kullmann, 19.8.2018 (Swansea)

/* An attempt to find (simple) recursions which simulate the node-counts
   of backtracking-algorithms.
*/

/* TODOS

1. Add parallelisation

   Similar to Backtracking.hpp.

*/

#include <cassert>
#include <cmath>

#include "ChessBoard.hpp"

namespace Recursion {

  template <class Branching>
  struct CountNodes {
    typedef Branching branching_t;
    using coord_t = ChessBoard::coord_t;
    using Var_uint = ChessBoard::Var_uint;
    using Count_t = ChessBoard::Count_t;
    const coord_t N;
    branching_t B;
    const Var_uint n0 = N*N;
    CountNodes(const coord_t N) noexcept : N(N), B{N} {}
    Count_t operator()() const noexcept {
      return operator()(n0);
    }
    Count_t operator()(const Var_uint n) const noexcept {
      if (n == 0) return 1;
      const Var_uint l = B.left(n);
      assert(l <= n);
      const Count_t lc = operator()(n-l);
      const Var_uint r = B.right(n);
      assert(r <= n);
      const Count_t rc = operator()(n-r);
      return 1 + lc + rc;
    }
  };

  // sqrt(n) left, 1 right:
  struct SQOne {
    using coord_t = ChessBoard::coord_t;
    using Var_uint = ChessBoard::Var_uint;
    SQOne(coord_t) noexcept {}
    static Var_uint left(const Var_uint n) noexcept {
      return Var_uint(std::sqrt(n));
    }
    static Var_uint right(const Var_uint) noexcept {
      return 1;
    }
  };

  // N left, 1 right:
  struct NOne {
    using coord_t = ChessBoard::coord_t;
    using Var_uint = ChessBoard::Var_uint;
    const coord_t N;
    NOne(const coord_t N) : N(N) {}
    Var_uint left(const Var_uint n) const noexcept {
      return (n > N) ? N : n;
    }
    static Var_uint right(const Var_uint) noexcept {
      return 1;
    }
  };


}
