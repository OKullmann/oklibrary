// Oliver Kullmann, 23.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Applying rotation/reflections about 45, 135 degrees
*/

#include <iostream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "BasicLatinSquares.hpp"
#include "LSOptions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "23.4.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/LSrotation.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 1;

  using namespace BasicLatinSquares;
  using namespace LSOptions;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " rot\n\n"
      " - rot            : " << Environment::WRPO<SR>{} << "\n\n"
      "reads squares from standard input, and applies the transformation"
      " to standard output.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc !=  commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed (rot), but the real number is " << argc-1 << ".\n";
    return 1;
  }

  const SR sr = [argv]{const auto sr0 = Environment::read<SR>(argv[1]);
                       if (not sr0) {
                         std::cerr << error << "rotation not readable from \""
                                   << argv[1] << "\".\n";
                         std::exit(1);}
                       return sr0.value();}();
  bool first = true;
  do {
    ls_t S;
    do S = in_strictls(std::cin); while (S.empty() and std::cin);
    if (S.empty()) return 0;
    if (not sqprop(S)) {
      std::cerr << error << "Square not square-shaped or -valued:\n";
      out(std::cerr, S);
      return 1;
    }
    switch (sr) {
    case SR::t : transpositionm(S); break;
    case SR::at : antitranspositionm(S); break;
    case SR::d : S = moddiags2rows(S); break;
    case SR::ad : S = modantidiags2rows(S); break;
    }
    rstandardisem(S);
    if (not first) std::cout << "\n";
    else first = false;
    out(std::cout, S);
  } while (std::cin);
}
