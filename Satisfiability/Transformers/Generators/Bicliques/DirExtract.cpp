// Oliver Kullmann, 14.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Extracting the data from a QBF2BCC-like file-database

  Extracting:

   - i
   - p1, p2, p3, p4

     the CNF:
   - n
   - c (orignal number of clauses)
   - c2 (number of clauses after removal of duplicated clauses)

     the conflict-graph
   - E (number of edges)

     the biclique-compatibility-graph
   - cE (-1, if not existing, and nan if declared "non-computable")
   - cE2
   - tcol (-1, if not existing)

USAGE:

  DirExtract dirname

extracts the data from directory dirname into dirname.R.

*/

#include <iostream>
#include <fstream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumTypes.hpp>
#include <Numerics/Statistics.hpp>

#include "DirStatistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.2",
        "22.10.2023",
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
      " extracts the content of the QBF2BCC-corpus in dirname"
      " to \"dirname.R\".\n\n"
;
    return true;
  }

  std::string header() {
    std::string res;
    res += proginfo.prg; res += " "; res += proginfo.vrs; res += " ";
    res += Environment::current_datetime();
    return res;
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

  const std::string info = header();
  std::cout << info << "\n\"" << outputname << "\"" << std::endl;
  output << "# " << info << std::endl;

  const auto [A, ignored] = all_adir(dirname);
  std::cout << A.size() << " " << ignored << std::endl;
  std::vector<AData> ad; ad.reserve(A.size());
  for (const auto& [i,a] : A) ad.emplace_back(a);

  GenStats::StdStatsStore sn, sc, sc2, sE, sE2, scE, scE2;
  count_t cE_NA = 0, cE_nan = 0, cE2_NA = 0, cE2_nan = 0,
    tcol = 0, tcol_NA = 0;
  for (const auto& d : ad) {
    sn += d.n; sc += d.c; sc2 += d.c2;
    sE += d.E; sE2 += d.E2;
    const auto cE = d.cE;
    if (cE == -1) ++cE_NA;
    else if (FP::isnan(cE)) ++cE_nan;
    else scE += cE;
    const auto cE2 = d.cE2;
    if (cE2 == -1) ++cE2_NA;
    else if (FP::isnan(cE2)) ++cE2_nan;
    else scE2 += cE2;
    if (d.tcol == 1) ++tcol;
    else if (d.tcol == -1) ++tcol_NA;
  }
  std::cout << "n: " << sn << "\n";
  std::cout << "c: " << sc << "\n";
  std::cout << "c2: " << sc2 << "\n";
  std::cout << "E: " << sE << "\n";
  std::cout << "E2: " << sE2 << "\n";
  std::cout << "cE: " << scE << "\n  NA=" << cE_NA
            << " nan=" << cE_nan << "\n";
  std::cout << "cE2: " << scE2 << "\n  NA=" << cE2_NA
            << " nan=" << cE2_nan << "\n";
  std::cout << "tcol: " << tcol << "\n  NA=" << tcol_NA << std::endl;

  FloatingPoint::fullprec_float80(output);
  formatted_output(output, ad);
}
