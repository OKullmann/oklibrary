// Oliver Kullmann, 12.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
 

*/

#include <iostream>
#include <array>
#include <map>
#include <set>

#include <ProgramOptions/Environment.hpp>

#include "DirStatistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.7",
        "12.8.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/DirEqual.cpp",
        "GPL v3"};

  using namespace DirStatistics;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " dirname\n\n"
    " computes information on equal cnf's for a QBF2BCC-like corpus in dirname.\n\n"
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
  auto [A, ignored] = all_adir(dirname);
  std::cout << A.size() << " " << ignored << "\n";
  typedef std::array<count_t, 2> par_pair_t;
  typedef std::map<par_pair_t, std::set<count_t>> map_pair_t;
  map_pair_t M0;
  for (const auto& [i,ad] : A) M0[{ad.n, ad.c}].insert(i);
  {std::vector<map_pair_t::iterator> to_remove;
   for (auto it = M0.begin(); it != M0.end(); ++it) {
     const auto [p, S] = *it;
     assert(not S.empty());
     if (S.size() == 1) {
       A.erase(*S.begin());
       to_remove.push_back(it);
     }
   }
   for (const auto it : to_remove) M0.erase(it);
  }
  for (const auto& [p,S] : M0) {
    std::cout << "[" << p[0] << "," << p[1] << "]: ";
    Environment::out_line(std::cout, S);
    std::cout << "\n";
  }
  
}
