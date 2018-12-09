// Oliver Kullmann, 19.8.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <string>

#include <cstdlib>

#include "InOut.hpp"
#include "Recursion.hpp"

namespace {

const std::string version = "0.1.2";
const std::string date = "9.12.2018";
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
  using namespace Recursion;
  std::cout << "N=" << N << ", N^2=" << (ChessBoard::Var_uint)(N) * N << "\n";
  std::cout << "Count approx=" << strong_conjecture(N) << ", exact=" << exact_values[N] << std::endl;
  {const CountLeaves<Nfact> Fact(N);
   std::cout << "Factorial: d=" << Fact.B.d << ", factorial=" << ChessBoard::Var_uint(factorial(N)) << ", factorial/approx=";
   std::cout.flush();
   std::cout << factorial(N) / Fact() << std::endl;}
  {const CountLeaves<Nstrconj> StrCon(N);
   std::cout << "Strong conjecture: d=" << StrCon.B.d << ", strongval/approx=";
   std::cout.flush();
   std::cout << double(strong_conjecture(N)) / StrCon() << std::endl;}
  {const CountLeaves<NTwo> R(N);
   std::cout << "d_left=" << R.B.dl << ", d_right=" << R.B.dr << ", value=";
   std::cout.flush();
   std::cout << R() << std::endl;}
  {CountLeaves<NN> R(N);
   std::cout << "d=" << R.B.d << ", value=";
   std::cout.flush();
   std::cout << R() << std::endl;}
}
