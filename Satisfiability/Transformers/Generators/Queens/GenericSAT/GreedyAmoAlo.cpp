// Oliver Kullmann, 12.8.2018 (Swansea)

#include <iostream>
#include <string>
#include <stdexcept>

#include "Backtracking.hpp"
#include "NQueens.hpp"

namespace {

const std::string version = "0.3";
const std::string date = "12.8.2018";
const std::string program = "Greedybacktracking"
#ifndef NDEBUG
  "_debug"
#endif
;
const std::string error = "ERROR[" + program + "]: ";

enum class Error {
  missing_argument=1,
  conversion=2,
  too_big=3,
  too_small=4
};
/* Extracting the underlying code of enum-classes (scoped enums) */
template <typename EC>
inline constexpr int code(const EC e) noexcept {return static_cast<int>(e);}

ChessBoard::coord_t interprete(const int argc, const char* const argv[]) noexcept {
  if (argc == 1) {
    std::cerr << error << "The argument N is needed.\n";
    std::exit(code(Error::missing_argument));
  }
  const std::string arg1 = argv[1];
  unsigned long N;
  try { N = std::stoul(arg1); }
  catch (const std::invalid_argument& e) {
    std::cerr << error << "The argument \"" << arg1 << "\" is not a valid integer.\n";
    std::exit(code(Error::conversion));
  }
  catch (const std::out_of_range& e) {
    std::cerr << error << "The argument \"" << arg1 << "\" is too big for unsigned long.\n";
    std::exit(code(Error::too_big));
  }
  if (N > ChessBoard::max_coord) {
    std::cerr << error << "The argument can be at most " << ChessBoard::max_coord << ", but is " << arg1 << ".\n";
    std::exit(code(Error::too_big));
  }
  if (N == 0) {
    std::cerr << error << "The argument is 0.\n";
    std::exit(code(Error::too_small));
  }
  return N;
}

}

int main(const int argc, const char* const argv[]) {

  typedef Backtracking::CountSat<NQueens::AmoAlo_board, NQueens::GreedyAmoAloBranching> Backtracking_q;

  const ChessBoard::coord_t N = interprete(argc, argv);

  NQueens::AmoAlo_board Fq(N);

  Backtracking_q B;
  const auto cFq = B(Fq);

  std::cout << cFq << " " << B.stats.nodes << "\n";

}
