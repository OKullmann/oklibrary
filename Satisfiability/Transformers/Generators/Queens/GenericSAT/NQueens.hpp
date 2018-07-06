// Oliver Kullmann, 6.7.2018 (Swansea)

#include "ChessBoard.hpp"

namespace NQueens {

  struct BasicACLS {

    typedef ChessBoard::Var Var;
    typedef ChessBoard::Var_uint Var_uint;

    void init(const int argc, const char* const argv[]) {

    }

    bool satisfied() const noexcept { return false; }
    bool falsified() const noexcept { return false; }

    Var_uint n() const noexcept { return 0; }
    Var_uint nset() const noexcept { return 0; }

    void set(const Var v, const bool val) {}

  };

  struct BasicBranching {

    typedef ChessBoard::Var Var;

    const BasicACLS& F;

    BasicBranching(const BasicACLS& F) : F(F) {}

    Var operator()() const noexcept { return Var{0,0}; }

  };
}
