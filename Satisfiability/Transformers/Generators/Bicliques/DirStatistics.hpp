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
#include <vector>
#include <map>
#include <utility>

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
    input_directory = 2,
    output_file = 3,
    hash_collision = 11,
  };

  const std::string instdir_suffix = ".B";
  const std::string Adir_prefix = "A_";
  const std::string Adir_suffix = ".d";

  // Assuming that p is a directory, and all entries which are directories
  // end with instdir_suffix (above), or none does:
  bool is_instancelevel_QBF2BCC(const std::filesystem::path& p) {
    assert(std::filesystem::is_directory(p));
    for (const auto& dir_entry : std::filesystem::directory_iterator(p))
      if (std::filesystem::is_directory(dir_entry))
        return dir_entry.path().string().ends_with(instdir_suffix);
    return false;
  }
  bool is_Aname(const std::string& s) noexcept {
    return s.starts_with(Adir_prefix) and s.ends_with(Adir_suffix);
  }
  bool is_Alevel_QBF2BCC(const std::filesystem::path& p) {
    assert(std::filesystem::is_directory(p));
    for (const auto& dir_entry : std::filesystem::directory_iterator(p)) {
      const std::string file = dir_entry.path().filename().string();
      if (not file.starts_with(".") and
          std::filesystem::is_directory(dir_entry))
        return is_Aname(file);
    }
    return false;
  }


  // Applying F to all leaves (that is, directories with instdir_suffix):
  template <class FUNC>
  void for_each_instdir(const std::filesystem::path& p, FUNC& F) {
    assert(std::filesystem::is_directory(p));
    if (is_instancelevel_QBF2BCC(p)) {
      for (const auto& dir_entry : std::filesystem::directory_iterator(p))
        if (std::filesystem::is_directory(dir_entry))
          F(dir_entry.path());
    }
    else {
      for (const auto& dir_entry : std::filesystem::directory_iterator(p))
        if (std::filesystem::is_directory(dir_entry))
          for_each_instdir(dir_entry.path(), F);
    }
  }
  template <class FUNC>
  void for_each_Adir(const std::filesystem::path& p, FUNC& F) {
    assert(std::filesystem::is_directory(p));
    if (is_Alevel_QBF2BCC(p)) {
      for (const auto& dir_entry : std::filesystem::directory_iterator(p))
        if (std::filesystem::is_directory(dir_entry) and
            not dir_entry.path().filename().string().starts_with("."))
          F(dir_entry.path());
    }
    else {
      for (const auto& dir_entry : std::filesystem::directory_iterator(p))
        if (std::filesystem::is_directory(dir_entry))
          for_each_Adir(dir_entry.path(), F);
    }
  }

  std::vector<std::filesystem::path>
  all_instdir(const std::filesystem::path& p) {
    typedef std::vector<std::filesystem::path> res_t;
    struct pb {
      res_t& res; pb(res_t& res) noexcept : res(res) {}
      void operator ()(std::filesystem::path p) { res.push_back(p); }
    };
    res_t res; pb F(res);
    for_each_instdir(p, F);
    return res;
  }

  typedef FP::UInt_t count_t;
  struct adir {
    const std::filesystem::path dir;
    const count_t i=0, n=0, c=0;
    const std::string p;
    adir() = default;
    adir(std::filesystem::path dir) : dir(dir), i(getn("i")), n(getn("n")),
                                      c(getn("c")), p(get("p")) {}
  private :
    std::string get(const std::string& s) const {
      return Environment::get_content(dir / s);
    }
    count_t getn(const std::string& s) const { return FP::to_UInt(get(s)); }
  };
  // The second component is the number of ignored A-dirs (due to
  // repeated indices):
  std::pair<std::map<count_t, adir>, count_t>
  all_adir(const std::filesystem::path& p) {
    typedef std::pair<std::map<count_t, adir>, count_t> res_t;
    struct pb {
      res_t& res; pb(res_t& res) noexcept : res(res) {}
      void operator ()(std::filesystem::path p) {
        adir A(p); const count_t i = A.i;
        const auto [it,found] = res.first.insert(std::pair(i, std::move(A)));
        res.second += not found;
      }
    };
    res_t res{}; pb F(res);
    for_each_Adir(p, F);
    return res;
  }


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


  // Taking the name of the directory dir, and appending .R :
  std::string output_filename(const std::filesystem::path& dir) {
    if (dir.empty()) return {};
    auto it = --dir.end();
    if (it->empty())
      if (it == dir.begin()) return {};
      else -- it;
    return it->string() + ".R";
  }

  struct AData {
    static std::string header() noexcept { return " i p n c E cE"; }
    count_t i;
    std::string p;
    count_t n, c, E;
    FP::float80 cE;
    AData(const adir& a) : i(a.i), p(a.p), n(a.n), c(a.c),
                           E(getu(a.dir, "E")), cE(getf(a.dir, "cE")) {}
    static std::pair<std::string, bool>
    get(const std::filesystem::path& dir, const std::string& s) {
      const auto p = dir / s;
      if (not std::filesystem::is_regular_file(p)) return {};
      return {Environment::get_content(dir / s), true};
    }
    static count_t getu(const std::filesystem::path& dir, const std::string& s) {
      const auto [res, status] = get(dir, s);
      assert(status);
      return FP::to_UInt(res);
    }
    static FP::float80 getf(const std::filesystem::path& dir, const std::string& s) {
      const auto [res, status] = get(dir, s);
      if (not status) return FP::NaN;
      else return FP::to_float80(res);
    }

  };
  std::ostream& operator <<(std::ostream& out, const AData& a) {
    return out << a.i << " " << a.p << " " << a.n << " " << a.c << " " << a.E << " " << a.cE;
  }

}

#endif
