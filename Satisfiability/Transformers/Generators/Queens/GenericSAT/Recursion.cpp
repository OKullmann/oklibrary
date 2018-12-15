// Oliver Kullmann, 19.8.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <string>
#include <limits>

#include <cstdlib>

#include "InOut.hpp"
#include "Recursion.hpp"

namespace {

const std::string version = "0.3";
const std::string date = "15.12.2018";
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
  using std::cout;
  std::cout << "Arithmetic:\n";
  std::cout << " floating_t: digits=" << limitfloat::digits << ", digits10=" << limitfloat::digits10 << ", epsilon=" << limitfloat::epsilon() << ", max=" << limitfloat::max() << ", size=" << sizeof(floating_t) << "\n";
  std::cout << " ChessBoard::Var_uint: size=" << sizeof(ChessBoard::Var_uint) << "\n";
  const auto approx_count = strong_conjecture(N);
  const auto exact_count = exact_value(N);
  std::cout << "N-Queens:\n";
  std::cout << " N=" << N << ", N^2=" << (ChessBoard::Var_uint)(N) * N << "\n";
  std::cout << " Count: exact=" << exact_count << ", strong conjecture=" << approx_count << ", exact/approx=" << exact_count/approx_count << std::endl;

  {const CountLeaves<Sfact> Fact(N);
   const auto fact = factorial(N), stirling = Sfactorial(N);
   std::cout << "Analysis N!:\n";
   std::cout << " Exact factorial / Stirling-approx = " << fact / stirling << "\n";
   std::cout << " Factorial: ltau=" << Fact.B.lt << ", d=" << Fact.B.d0 << ", stirling/approx=";
   std::cout.flush();
   std::cout << stirling / Fact() << std::endl;}
  std::cout << "Analysis strong_conjecture:\n";
  {const CountLeaves<Nstrconj> StrCon(N);
   std::cout << " ltau=" << StrCon.B.lt << ", d=" << StrCon.B.d0 << ", strong_conj/approx=";
   std::cout.flush();
   std::cout << approx_count / StrCon() << std::endl;}
  {const CountLeaves<NAstrconj> R(N);
   std::cout << " d_left=" << R.B.dl << ", d_right=" << R.B.d0 << ", strong_conj/approx=";
   std::cout.flush();
   std::cout << approx_count / R() << std::endl;}
}
