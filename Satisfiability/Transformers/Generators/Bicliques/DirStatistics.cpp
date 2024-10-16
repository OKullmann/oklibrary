// Oliver Kullmann, 26.6.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Collecting the statistics in the instance-directories as given by the
  .stats-subdirectories:

   - first the statistics on the number of non-trivial and non-biclique
     components in the instance-directories (.B-directories)
   - then for the cnf-entries (A_-directories) summary-statistics on
     number of variables and number of clauses.

  The input is dirname:

   - recursively all sub-directories containing some .B-directory are
     sought;
   - it is assumed then that there all directory-entries are such
     instance-directories.

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "DirStatistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
        "12.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/DirStatistics.cpp",
        "GPL v3"};

  using namespace DirStatistics;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " dirname\n\n"
    " computes statistics for a QBF2BCC-like corpus in dirname.\n\n"
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

  const std::string dirname = argv[1];
  components_stats S;
  for_each_instdir(dirname, S);
  std::cout << "#non-trivial components:\n" << S.Snt << "\n";
  std::cout << "#non-biclique components:\n" << S.Snb << "\n";
  std::cout << "n-values:\n" << S.Mn << "\n";
  std::cout << "c-values:\n" << S.Mc << "\n";
}
