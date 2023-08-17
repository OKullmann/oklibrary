// Oliver Kullmann, 16.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Determining whether the A-cnfs are bipartite

*/

#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include "DimacsTools.hpp"
#include "Graphs.hpp"
#include "ConflictGraphs.hpp"
#include "DirStatistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "17.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/DirBipart.cpp",
        "GPL v3"};

  using namespace DirStatistics;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " dirname [exceptions]\n\n"
    " runs a 2-colouring-algorithms on the A-cnfs in dirname.\n\n"
;
    return true;
  }

  std::set<count_t> read_exceptions(const int argc, const char* const argv[]) {
    if (argc == 2) return {};
    assert(argc >= 3);
    std::string arg(argv[2]);
    Environment::mremove_spaces(arg);
    std::set<count_t> res;
    for (const auto& x : Environment::split(arg, ','))
      res.insert(FloatingPoint::to_UInt(x));
    return res;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 2 and argc != 3) {
    std::cerr << error <<
      "Exactly one or two arguments (dirname, [exceptions])"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const std::filesystem::path dirname(argv[1]);
  if (not std::filesystem::is_directory(dirname)) {
    std::cerr << error <<
      "Input-directory " << dirname << " is not an (existing) directory.\n";
    return int(Error::input_directory);
  }
  const auto exceptions = read_exceptions(argc, argv);

  const auto [A, ignored] = all_adir(dirname);
  std::cout << A.size() << " " << ignored << std::endl;
  for (const auto i : exceptions)
    if (not A.contains(i)) {
      std::cerr << error <<
        "Exceptions " << i << " not present.\n";
      return int(Error::bad_exceptions);
    }

  count_t count = 0, found_sat = 0;
  for (const auto& [i,ad] : A) {
    assert(ad.c >= 1);
    if (exceptions.contains(i)) continue;
    const auto outputpath = ad.dir / bipart_file;
    if (std::filesystem::is_regular_file(outputpath)) continue;
    ++count;
    std::cout << " " << i; std::cout.flush();
    std::ifstream cnf(ad.dir / "cnf");
    if (not cnf) {
      std::cerr << error <<
        "File " << (ad.dir / "cnf") << " not readable.\n";
      return int(Error::cnf_file);
    }
    const auto G =
      ConflictGraphs::conflictgraph(DimacsTools::read_strict_Dimacs(cnf));
    cnf.close();
    const auto res = bipart_0comp(G);
    std::ofstream colfile(outputpath);
    if (not colfile) {
      std::cerr << error <<
        "File " << outputpath << " not writable.\n";
      return int(Error::col_file);
    }
    if (res.result().empty()) colfile << 0;
    else {
      assert(res.size() == ad.n);
      ++found_sat; colfile << 1;
    }
    colfile.close();
  }

  std::cout << "\n" << count << " done\n";
  std::cout << found_sat << " SAT\n";

}
