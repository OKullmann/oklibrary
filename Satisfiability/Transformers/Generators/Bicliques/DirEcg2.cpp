// Oliver Kullmann, 25.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the size of conflict-graph for the cnf2's

*/

#include <iostream>
#include <fstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "DirStatistics.hpp"
#include "DimacsTools.hpp"
#include "ConflictGraphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.4",
        "25.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/DirEcg2.cpp",
        "GPL v3"};

  using namespace DirStatistics;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " dirname\n\n"
    " computes E2 for all cnfs's in dirname.\n\n"
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

    const auto F = a.getcnf2();
    assert(F.first.n == a.n and F.first.c == a.c2);

    const auto mpath = a.dir / "mul2";
    std::ifstream mfile(mpath);
    if (not mfile) {
      std::cerr << error <<
      "CNF " << mpath << " can not be opened for reading.\n";
      return int(Error::cnf_file);
    }

    const count_t E2 = 0;

    const auto opath = a.dir / "E2";
    Environment::put_content(opath, std::to_string(E2));
    
  }

}
