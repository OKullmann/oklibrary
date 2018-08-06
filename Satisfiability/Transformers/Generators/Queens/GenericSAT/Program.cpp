// Oliver Kullmann, 6.7.2018 (Swansea)

#include <iostream>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "NQueensPartial.hpp"

namespace {

  ChessBoard::coord_t interprete(const int argc, const char* const argv[]) noexcept {
    return 0;
  }
}

int main(const int argc, const char* const argv[]) {
  typedef Backtracking::CountSat<NQueens::BasicACLS, NQueens::BasicBranching> Backtracking_q;

  const ChessBoard::coord_t N = interprete(argc, argv);
  NQueens::BasicACLS Fq(N);
  NQueensPartial::BasicACLS Fqp(N);

  const auto cFq = Backtracking_q()(Fq);

  std::cout << cFq << "\n";

}
