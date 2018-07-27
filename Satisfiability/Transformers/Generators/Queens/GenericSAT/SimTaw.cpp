// Oliver Kullmann, 23.7.2018 (Swansea)

#include <iostream>

#include "Backtracking.hpp"
#include "NQueens.hpp"

namespace {

  ChessBoard::coord_t interprete(const int argc, const char* const argv[]) noexcept {
    return atoi(argv[1]);
  }
}

int main(const int argc, const char* const argv[]) {
  typedef Backtracking::CountSat<NQueens::AmoAlo_board, NQueens::GreedyAmo, ChessBoard::Count_t> Backtracking_q;

  const ChessBoard::coord_t N = interprete(argc, argv);
  NQueens::AmoAlo_board Fq(N);

  const auto cFq = Backtracking_q()(Fq);

  std::cout << cFq << "\n";

}
