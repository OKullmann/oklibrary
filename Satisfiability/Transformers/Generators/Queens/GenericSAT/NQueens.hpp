// Oliver Kullmann, 6.7.2018 (Swansea)

#include "ChessBoard.hpp"

namespace NQueens {

  // The prototype:
  struct BasicACLS {

    typedef ChessBoard::Var Var;
    typedef ChessBoard::Var_uint Var_uint;

    const ChessBoard::coord_t N;

    explicit BasicACLS(const ChessBoard::coord_t N) : N(N) {}

    bool satisfied() const noexcept { return false; }
    bool falsified() const noexcept { return false; }

    Var_uint n() const noexcept { return N; }
    Var_uint nset() const noexcept { return 0; }

    void set(const Var v, const bool val) {}

  };

  // A concrete instance of BasicACLS:
  class AmoAlo_board {

  public :

    typedef ChessBoard::Var Var;
    typedef ChessBoard::Var_uint Var_uint;

    const ChessBoard::coord_t N;

    explicit AmoAlo_board(const ChessBoard::coord_t N) : N(N) {}

    bool satisfied() const noexcept { return false; }
    bool falsified() const noexcept { return false; }

    Var_uint n() const noexcept { return N; }
    Var_uint nset() const noexcept { return 0; }

    void set(const Var v, const bool val) {}

    // XXX

  };


  // The prototype:
  struct BasicBranching {

    typedef ChessBoard::Var Var;

    const BasicACLS& F;

    BasicBranching(const BasicACLS& F) : F(F) {}

    Var operator()() const noexcept { return Var{0,0}; }

  };

  // A concrete instance of BasicBranching:
  struct GreedyAmo {

    typedef ChessBoard::Var Var;

    const AmoAlo_board& F;

    GreedyAmo(const AmoAlo_board& F) : F(F) {}

    Var operator()() const noexcept { return Var{0,0}; }

    // XXX

  };

}
