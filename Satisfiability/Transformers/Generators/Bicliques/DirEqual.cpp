// Oliver Kullmann, 12.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the cnf's (resp. their indices) which are file-equal.

  Output:
   1. line : #cnf's-found, duplications of indices (should be 0).
   2. The blocks of equals, each block on a line, separated by spaces.
   3. The number of cnf's to be eliminated, and the new total count.

*/

#include <iostream>
#include <array>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>

#include <ProgramOptions/Environment.hpp>

#include "DirStatistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.5",
        "14.8.2023",
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
         << " dirname [DEL]\n\n"
    " dirname        : a string\n"
    " DEL            : a string; if present, duplications are removed\n\n"

    " computes file-equal cnf's for a QBF2BCC-like corpus (in dirname):\n\n"
    "  - DEL is the filename for logging of removal.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 2 and argc != 3) {
    std::cerr << error <<
      "Exactly one or two arguments (dirname, [del-logging])"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const std::string dirname = argv[1];
  const bool with_removal = argc == 3;

  auto [A, ignored] = all_adir(dirname);
  const count_t Aorig = A.size();
  std::cout << Aorig << " " << ignored << "\n";
  typedef std::array<count_t, 2> par_pair_t;
  typedef std::map<par_pair_t, std::set<count_t>> map_pair_t;
  map_pair_t M0;
  for (const auto& [i,ad] : A) M0[{ad.n, ad.c}].insert(i);
  {std::vector<map_pair_t::iterator> to_remove;
   for (auto it = M0.begin(); it != M0.end(); ++it) {
     const auto& [p, S] = *it;
     assert(not S.empty());
     if (S.size() == 1) {
       A.erase(*S.begin());
       to_remove.push_back(it);
     }
   }
   for (const auto it : to_remove) M0.erase(it);
  }

  count_t reduced = 0;
  using Environment::get_content;
  std::set<std::set<count_t>> equals;
  {std::vector<map_pair_t::iterator> to_remove;
   for (auto it = M0.begin(); it != M0.end(); ++it) {
     const auto& [p, S] = *it;
     assert(S.size() >= 2);
     if (S.size() == 2) {
       to_remove.push_back(it);
       auto i = S.begin();
       const count_t a = *i++, b = *i;
       if (get_content(A[a].dir / "cnf") == get_content(A[b].dir / "cnf")) {
         equals.insert({a,b});
         ++reduced;
       }
       else { A.erase(a); A.erase(b); }
     }
   }
   for (const auto it : to_remove) M0.erase(it);
  }

  for (const auto& [p, S] : M0) {
    typedef std::map<count_t, std::set<count_t>> inv_img_t;
    inv_img_t inv_img;
    for (const count_t i : S)
      inv_img[Environment::hash(get_content(A[i].dir / "cnf"))].insert(i);
    for (const auto& [h, Sh] : inv_img) {
      assert(not Sh.empty());
      if (Sh.size() == 1) { A.erase(*Sh.begin()); continue; }
      auto it = Sh.begin(); const auto end = Sh.end();
      const count_t i0 = *it++;
      const std::string F0 = get_content(A[i0].dir / "cnf");
      do {
        if (get_content(A[*it].dir / "cnf") != F0) {
          std::cerr << error << "Cnf's with indices " << i0 << ", " <<
            *it << " yield the same hash " << h << ".\n";
          return int(Error::hash_collision);
        }
      } while (++it != end);
      equals.insert(Sh);
      reduced += Sh.size() - 1;
    }
  }
  std::cout << "\n";
  for (count_t i = 1; const auto& S : equals) {
    std::cout << i << ":";
    for (const count_t j : S) std::cout << " " << A[j].p;
    std::cout << "\n"; ++i;
  }
  std::cout << "\n" << equals.size() << " " << reduced << " "
            << Aorig - reduced << std::endl;
  assert(A.size() == reduced + equals.size());

  if (with_removal) {
    const std::string loggingname = argv[2];
    std::ofstream logging(loggingname);
    if (not logging) {
      std::cerr << error <<
        "Can not open logging-file " << loggingname << ".\n";
      return int(Error::logging_file);
    }
    std::vector<std::filesystem::path> parentdirs;
    for (const auto& S : equals) {
      assert(S.size() >= 2);
      for (auto it = ++S.begin(); it != S.end(); ++it) {
        const auto path = A[*it].dir;
        logging << path << "\n";
        parentdirs.push_back(path.parent_path());
        [[maybe_unused]] const auto removed =
          std::filesystem::remove_all(path);
        assert(removed >= 1 + 4); // dir + i,p,n,c
      }
    }
    bool first = true;
    for (const auto& path : parentdirs) {
      auto members = all_members(path, 3);
      const auto size = members.size();
      if (size <= 1) {
        std::cerr << error <<
          "Irregular instance-dir " << path << " with only " <<
          size << " members.\n";
        return int(Error::bad_instdir);
      }
      if (size >= 3) continue;
      std::ranges::sort(members);
      if (members != std::vector<std::string>{".qi", ".stats"}) {
        std::cerr << error <<
          "Instance-dir " << path << " has unexpected members \"" <<
          members[0] << "\", \"" << members[1] << "\".\n";
        return int(Error::bad_instdir);
      }
      if (first) {first = false; logging << "\n";}
      logging << path << "\n";
      [[maybe_unused]] const auto removed = std::filesystem::remove_all(path);
      assert(removed == 3);
    }
  }
}
