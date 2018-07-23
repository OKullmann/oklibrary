// Oliver Kullmann, 6.7.2018 (Swansea)

#include "ChessBoard.hpp"

namespace NQueensPartial {

  struct BasicACLS {

    const ChessBoard::coord_t N;

    explicit BasicACLS(const ChessBoard::coord_t N) : N(N) {}

  };

  struct BasicBranching {

  };

}
