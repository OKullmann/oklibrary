// Oliver Kullmann, 14.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the sizes of conflict-graph and derived
  biclique-compatibility graph

  Extracting:
  i
  p
  n
  c
  E
  cE (NA, if not existing, and nan if declared "non-computable")
  2col (NA, if not existing)

*/

#include <iostream>
#include <fstream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumTypes.hpp>
#include <Numerics/Statistics.hpp>

#include "DirStatistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "16.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/DirExtract.cpp",
        "GPL v3"};

  using namespace DirStatistics;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " dirname\n\n"
    " extracts the content of the QBF2BCC-corpus in dirname to dirname.R.\n\n"
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
  const std::string outputname = output_filename(dirname);
  std::ofstream output(outputname);
  if (not output) {
    std::cerr << error <<
      "Can not open output-file " << outputname << ".\n";
    return int(Error::output_file);
  }
  std::cout << "\"" << outputname << "\"" << std::endl;

  const auto [A, ignored] = all_adir(dirname);
  std::cout << A.size() << " " << ignored << std::endl;
  std::vector<AData> ad; ad.reserve(A.size());
  for (const auto& [i,a] : A) ad.emplace_back(a);

  GenStats::StdStatsStore sn, sc, sE, scE;
  count_t cE_NA = 0, cE_nan = 0, tcol = 0;
  for (const auto& d : ad) {
    sn += d.n; sc += d.c; sE += d.E;
    const auto cE = d.cE;
    if (cE == -1) ++cE_NA;
    else if (FP::isnan(cE)) ++cE_nan;
    else scE += cE;
    if (d.tcol == 1) ++tcol;
  }
  std::cout << "n: " << sn << "\n";
  std::cout << "c: " << sc << "\n";
  std::cout << "E: " << sE << "\n";
  std::cout << "cE: " << scE << "\n  NA=" << cE_NA
            << " nan=" << cE_nan << "\n";
  std::cout << "tcol: " << tcol << std::endl;

  FloatingPoint::fullprec_float80(output);
  output << AData::header() << "\n";
  Environment::out_line(output, ad, "\n");
}
