// Oliver Kullmann, 26.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the size of biclique-compatibility-graphs for the cnf2's

*/

#include <iostream>
#include <fstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include "DirStatistics.hpp"
#include "ConflictGraphs.hpp"
#include "Bicliques.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "25.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/DirEbccom2.cpp",
        "GPL v3"};

  using namespace DirStatistics;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " dirname [threads=1]\n\n"
    " computes cE2 for all cnf2's in dirname.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 2 and argc != 3) {
    std::cerr << error <<
      "Exactly one or two arguments (dirname, [threads])"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const std::filesystem::path dirname(argv[1]);
  if (not std::filesystem::is_directory(dirname)) {
    std::cerr << error <<
      "Input-directory " << dirname << " is not an (existing) directory.\n";
    return int(Error::input_directory);
  }
  const count_t threads = argc==2 ? 1 : FloatingPoint::to_UInt(argv[2]);

  const auto [A, aig] = all_adir(dirname);
  std::cout << A.size() << " " << aig << std::endl;

  count_t ignored = 0, copied = 0, computed = 0;
  for (const auto& [i,a] : A) {

    const auto output = a.dir / "cE2";

    if (std::filesystem::is_regular_file(output)) {
      ++ignored; continue;
    }
    if (a.c == a.c2) {
      std::filesystem::copy_file(a.dir / "cE", output);
      ++copied; continue;
    }

    const auto F = a.getcnf2();
    assert(F.first.n == a.n and F.first.c == a.c2);
    const auto G = ConflictGraphs::conflictgraph(F);
    const count_t cE2 = threads == 1 ?
      Bicliques::bccom_degree_stats<3>(G).second :
      Bicliques::bccom_degree_stats_parallel<3>(G, threads).second;
    Environment::put_content(output, std::to_string(cE2));
    ++computed;

  }

  std::cout << "ignored : " << ignored << "\n"
            << "copied  : " << copied << "\n"
            << "computed: " << computed << "\n";

}
