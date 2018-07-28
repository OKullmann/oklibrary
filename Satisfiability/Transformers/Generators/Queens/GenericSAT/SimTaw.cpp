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
  typedef NQueens::AmoAlo_board::Ranks Ranks;
  typedef NQueens::AmoAlo_board::Board Board;

  const ChessBoard::coord_t N = interprete(argc, argv);
  Board board;
  Ranks r_rank;
  Ranks c_rank;
  Ranks ad_rank;
  Ranks d_rank;
  NQueens::AmoAlo_board Fq(N,board,r_rank,c_rank,ad_rank,d_rank);

  const auto cFq = Backtracking_q()(Fq);

  std::cout << cFq << "\n";

}
