// Oliver Kullmann, 6.7.2018 (Swansea)

/* A stub for compilation-testing. */

#include <iostream>
#include <string>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "NQueensPartial.hpp"

namespace {

  ChessBoard::coord_t interprete(const int argc, const char* const argv[]) noexcept {
    return (argc > 1) ? std::stoul(argv[1]) : 1;
  }
}

int main(const int argc, const char* const argv[]) {
  typedef Backtracking::CountSat<NQueens::BasicACLS, NQueens::BasicBranching> Backtracking_q;

  const ChessBoard::coord_t N = interprete(argc, argv);
  NQueens::BasicACLS Fq(N);
  NQueensPartial::BasicACLS Fqp(N);

  Backtracking_q B;
  const auto cFq = B(Fq);

  std::cout << cFq << "\n" << B.stats;
}
