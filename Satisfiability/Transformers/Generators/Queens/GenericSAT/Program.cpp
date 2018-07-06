// Oliver Kullmann, 6.7.2018 (Swansea)

#include <iostream>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "NQueensPartial.hpp"

namespace {

}

int main(const int argc, const char* const argv[]) {
  NQueens::BasicACLS Fq;
  NQueensPartial::BasicACLS Fqp;

  typedef Backtracking::CountSat<NQueens::BasicACLS, NQueens::BasicBranching, ChessBoard::Count_t> Backtracking_q;

  Fq.init(argc, argv);
  Fqp.init(argc, argv);

  const auto cFq = Backtracking_q()(Fq);

  std::cout << cFq << "\n";

}
