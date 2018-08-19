// Oliver Kullmann, 19.8.2018 (Swansea)

#include <iostream>
#include <string>

#include <cstdlib>

#include "InOut.hpp"
#include "Recursion.hpp"

namespace {

const std::string version = "0.1";
const std::string date = "19.8.2018";
const std::string program = "Recursion"
#ifndef NDEBUG
  "_debug"
#endif
;
const std::string error = "ERROR[" + program + "]: ";

void version_information() noexcept {
  std::cout << program << ":\n"
   " Version: " << version << "\n"
   " Last change date: " << date << "\n"
#ifdef NDEBUG
   " Compiled with NDEBUG\n"
#else
   " Compiled without NDEBUG\n"
#endif
#ifdef __OPTIMIZE__
   " Compiled with optimisation options\n"
#else
   " Compiled without optimisation options\n"
#endif
   " Compilation date: " __DATE__ " " __TIME__ "\n"
#ifdef __GNUC__
   " Compiler: g++, version " __VERSION__ "\n"
#else
   " Compiler not gcc\n"
#endif
  ;
  std::exit(0);
}

}

int main(const int argc, const char* const argv[]) {

  if (argc == 2 and std::string(argv[1]) == "-v") version_information();
  const ChessBoard::coord_t N = InOut::interprete(argc, argv, error);
  Recursion::CountNodes<Recursion::SQOne> R1(N);
  Recursion::CountNodes<Recursion::NOne> R2(N);
  std::cout << R1() << " " << R2() << "\n";
}
