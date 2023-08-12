// Oliver Kullmann, 26.6.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General definitions for processing the corpus QBF2BCC

*/

#ifndef DIRSTATISTICS_gDRJRMWsir
#define DIRSTATISTICS_gDRJRMWsir

#include <filesystem>
#include <string>

#include <cassert>

#include <Numerics/NumTypes.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/Statistics.hpp>
#include <ProgramOptions/Strings.hpp>

#include "Disassemble.hpp"

namespace DirStatistics {

  namespace FP = FloatingPoint;
  namespace DA = Disassemble;

  enum class Error {
    missing_parameters = 1,
  };

  const std::string leaf_ending = ".B";

  bool is_instancelevel_QBF2BCC(const std::filesystem::path& p) {
    assert(std::filesystem::is_directory(p));
    for (const auto& dir_entry : std::filesystem::directory_iterator(p))
      if (std::filesystem::is_directory(dir_entry))
        return dir_entry.path().string().ends_with(leaf_ending);
    return false;
  }

  // Applying F to all leaves:
  template <class FUNC>
  void for_each_leaf(const std::filesystem::path& p, FUNC& F) {
    assert(std::filesystem::is_directory(p));
    if (is_instancelevel_QBF2BCC(p)) {
      for (const auto& dir_entry : std::filesystem::directory_iterator(p))
        if (std::filesystem::is_directory(dir_entry))
          F(dir_entry.path());
    }
    else {
      for (const auto& dir_entry : std::filesystem::directory_iterator(p))
        if (std::filesystem::is_directory(dir_entry))
          for_each_leaf(dir_entry.path(), F);
    }
  }

  typedef FP::UInt_t count_t;
  typedef FP::float80 float_t;
  typedef GenStats::FreqStats<count_t, float_t> fstats_t;

  struct nlvs {
    count_t n = 0;
    void operator ()(const std::filesystem::path&) noexcept { ++n; }
  };
  struct components_stats {
    fstats_t Snt, Snb, Mn, Mc;
    void operator ()(const std::filesystem::path& p) noexcept {
      assert(std::filesystem::is_directory(p));
      const auto f = p / DA::statsdirname;
      Snt += FP::to_UInt(Environment::get_content(f / DA::ntccfile));
      Snb += FP::to_UInt(Environment::get_content(f / DA::nbccfile));
      {const auto [L,all_filled] =
         Environment::get_items<count_t,2>(f / DA::nmapfile);
       assert(all_filled);
       for (const auto [n,count] : L) Mn.add(n, count);
      }
      {const auto [L,all_filled] =
         Environment::get_items<count_t,2>(f / DA::cmapfile);
       assert(all_filled);
       for (const auto [c,count] : L) Mc.add(c, count);
      }
    }
  };

}

#endif
