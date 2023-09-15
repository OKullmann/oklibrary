// Oliver Kullmann, 3.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For input-CNF F, compute symmetry-breaking data

*/

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Numerics/NumInOut.hpp>

#include "Bicliques.hpp"
#include "DimacsTools.hpp"
#include "ConflictGraphs.hpp"
#include "Bicliques2SAT.hpp"

#include "BCC2SAT.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.3",
        "7.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/CNFBCC.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " sb-rounds seeds\n\n"
    " sb-rounds      : " << "default is " <<
      Bicliques2SAT::default_sb_rounds << "\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"\n\n"
    " reads a cnf from standard input, and computes symmetry-breaking data :\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3) {
    std::cerr << error <<
      "Exactly two arguments (sb-rounds, seeds)"
      " needed, but " << argc-1 << " provided.\n";
    return int(BCC2SAT::Error::missing_parameters);
  }

  const BCC2SAT::var_t sb_rounds =
    BCC2SAT::read_var_t(argv[1], Bicliques2SAT::default_sb_rounds);
  const RandGen::vec_eseed_t seeds = RandGen::extract_seeds(argv[2]);

  const auto F = DimacsTools::read_strict_Dimacs(std::cin);
  std::cerr << F.first;
  const auto G = ConflictGraphs::conflictgraph(F);
  std::cerr << G.n() << " " << G.m() << "\n";
  const auto [stats, E] = Bicliques::bccom_degree_stats<1>(G);
  FloatingPoint::fullprec_float80(std::cerr);
  std::cerr << stats << "\n";
  std::cerr << E << "\n";

}
