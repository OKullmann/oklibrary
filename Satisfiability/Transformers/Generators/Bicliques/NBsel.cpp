// Oliver Kullmann, 10.6.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Selection of a ramdom sub-CNF of input in NOBOCONF format,
  according to the uniform or binomial model

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include <Transformers/Generators/Random/GenClauseSets.hpp>

#include "NBsel.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.8",
        "11.6.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/NBsel.cpp",
        "GPL v3"};

  using namespace NBsel;

  namespace RG = RandGen;
  namespace GCS = GenClauseSets;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " [[+-]k|p] [seeds]\n\n"
    " k|p        : number k of clauses or probability p = \"num/denom\"\n"
    " seeds      : "; RG::explanation_seeds(std::cout, 12);
    std::cout <<
    "\n\n"
    " selects k clauses resp. with probability p from standard input:\n\n"
    " - Arguments \"\" (the empty string) yield also the default-values.\n"
    " - The default for the first argument is p=1/2.\n"
    " - Numbers are unsigned 64-bit integers, however the input allows to\n"
    "   give them as 80-floating-point values (which will be rounded).\n"
    " - The optional \"-\" means \"exclusion\" (inclusion otherwise).\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const auto [k, p, uniform, inclusion] = read_sel_arg(
   argc>1 ? std::string(argv[1]) : std::string(""), error);
  const auto [seeds, basic_size] =
    all_seeds(k, p, uniform, inclusion,
              argc>2 ? std::string(argv[2]) : std::string(""));
  std::cout << GCS::comchar << " ";
  if (uniform) std::cout << "k=" << k;
  else std::cout << "p=" << p;
  std::cout << "\n" << GCS::comchar << " seeds: ";
  Environment::out_line(std::cout, seeds);
  std::cout << std::endl;
}
