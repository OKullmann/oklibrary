// Oliver Kullmann, 15.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Sorting all cnf's in a QBF2BCC-corpus

   - Strict Dimacs is assumed, and that all clauses are already sorted and
     don't have duplications and are non-tautological.

*/

#include <iostream>
#include <fstream>
#include <algorithm>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "DirStatistics.hpp"
#include "DimacsTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "15.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/DirStdCNF.cpp",
        "GPL v3"};

  using namespace DirStatistics;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " dirname\n\n"
    " sorts all cnf's in dirname.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 2) {
    std::cerr << error <<
      "Exactly one argument (dirname)"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const std::filesystem::path dirname(argv[1]);
  if (not std::filesystem::is_directory(dirname)) {
    std::cerr << error <<
      "Input-directory " << dirname << " is not an (existing) directory.\n";
    return int(Error::input_directory);
  }

  const auto [A, ignored] = all_adir(dirname);
  std::cout << A.size() << " " << ignored << std::endl;
  for (const auto& [i,a] : A) {
    const auto path = a.dir / "cnf";
    std::ifstream ifile(path);
    if (not ifile) {
      std::cerr << error <<
      "CNF " << path << " can not be opened for reading.\n";
    return int(Error::cnf_file);
    }
    auto F = DimacsTools::read_strict_Dimacs(ifile);
    ifile.close();
    assert(F.first.n == a.n and F.first.c == a.c);
    std::ranges::sort(F.second);
    std::ofstream ofile(path);
    if (not ofile) {
      std::cerr << error <<
      "CNF " << path << " can not be opened for writing.\n";
    return int(Error::cnf_file);
    }
    ofile << F;
    ofile.close();
  }

}
