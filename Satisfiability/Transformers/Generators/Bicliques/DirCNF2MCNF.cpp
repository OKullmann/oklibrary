// Oliver Kullmann, 23.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Replacing all cnf's by multi-clause-sets (no repetitions, with
  multiplicities).

   - Strict Dimacs is assumed, and that all clause-sets are already fully
     sorted
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
        "0.0.8",
        "24.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/DirCNF2MCNF.cpp",
        "GPL v3"};

  using namespace DirStatistics;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " dirname\n\n"
    " replaces all cnf's by mcnf's in dirname.\n\n"
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
    auto F = DimacsTools::read_strict_MDimacs(ifile);
    ifile.close();
    assert(F.F.first.n == a.n and F.tc == a.c);
    const auto c2 = F.F.first.c;
    assert(c2 != 0);

    {const auto path2 = a.dir / "cnf2";
     std::ofstream ofile(path2);
     if (not ofile) {
       std::cerr << error <<
         "MCNF " << path2 << " can not be opened for writing.\n";
       return int(Error::cnf_file);
     }
     ofile << F.F;
    }
    {const auto path2 = a.dir / "mul2";
     std::ofstream ofile(path2);
     if (not ofile) {
       std::cerr << error <<
         "MCNF " << path2 << " can not be opened for writing.\n";
       return int(Error::cnf_file);
     }
     Environment::out_line(ofile, F.M, "\n");
     ofile << "\n";
    }
    Environment::put_content(a.dir / "c2", std::to_string(c2));

  }

}
