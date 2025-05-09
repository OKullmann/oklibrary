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
#include <stdexcept>
#include <sstream>
#include <array>

#include <cassert>

#include <Numerics/NumTypes.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/Statistics.hpp>
#include <ProgramOptions/Strings.hpp>

#include "Disassemble.hpp"

namespace DirStatistics {

  namespace FP = FloatingPoint;
  namespace DA = Disassemble;

  typedef FP::UInt_t count_t;

  std::vector<std::string> all_members(const std::filesystem::path& dir,
                                       count_t cap) {
    assert(std::filesystem::is_directory(dir));
    std::vector<std::string> res;
    if (cap == 0)
      for (const auto& e : std::filesystem::directory_iterator(dir))
        res.push_back(e.path().filename().string());
    else
      for (const auto& e : std::filesystem::directory_iterator(dir)) {
        res.push_back(e.path().filename().string());
        if (--cap == 0) break;
      }
    return res;
  }

  enum class Error {
    missing_parameters = 1,
    input_directory = 2,
    output_file = 3,
    logging_file = 4,
    bad_instdir = 5,
    cnf_file = 6,
    bad_exceptions = 7,
    col_file = 8,
    minisat_call = 9,
    hash_collision = 11,
    filename_empty = 12,
    repeated_adirs = 13,
    missing_adir = 14,
    different_adir = 15,
    faulty_adir = 16,
  };

  const std::string instdir_suffix = ".B";
  const std::string Adir_prefix = "A_";
  const std::string Adir_suffix = ".d";
  const std::string bipart_file = "tcol";

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
            not dir_entry.path().filename().string().starts_with(".")) {
          assert(is_Aname(dir_entry.path().filename()));
          F(dir_entry.path());
        }
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

  typedef FP::float80 float_t;

  struct adir {
    const std::filesystem::path dir;
    const count_t i=0, n=0, c=0, c2=0;
    const std::string p;

    adir() = default;
    adir(std::filesystem::path dir) :
      dir(dir), i(getu("i")), n(getu("n")),
      c(getu("c")), c2(getu("c2")), p(get("p")) {}

    bool operator ==(const adir& rhs) const noexcept {
      return i==rhs.i and n==rhs.n and c==rhs.c and c2==rhs.c2 and p==rhs.p and
        dir.filename() == rhs.dir.filename() and
        dir.parent_path().filename() == rhs.dir.parent_path().filename();
    }
    friend std::ostream& operator <<(std::ostream& out, const adir& ad) {
      return out << ad.dir << ": " << ad.i << " " << ad.p << " " <<
        ad.n << " " << ad.c << " " << ad.c2;
    }

    struct file_error : std::runtime_error {
      file_error(std::string m) noexcept : std::runtime_error(std::move(m)) {}
    };
    struct number_error : std::runtime_error {
      number_error(std::string m) noexcept : std::runtime_error(std::move(m)) {}
    };
    struct dimacs_error : std::runtime_error {
      dimacs_error(std::string m) noexcept : std::runtime_error(std::move(m)) {}
    };

    std::string get(const std::string& s) const {
      std::string res;
      try { res = Environment::get_content(dir / s); }
      catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::get: Error getting content of\n  "
           << dir / s << "\n   original error is\n  " << e.what() << "\n";
        throw file_error(ss.str());
      }
      return res;
    }
    count_t getu(const std::string& s) const {
      count_t res;
      try { res = FP::to_UInt(get(s)); }
      catch (const file_error&) { throw; }
      catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::getu: "
          "Error getting unsigned integer from\n  "
           << dir / s << "\n   original error is\n  " << e.what() << "\n";
        throw number_error(ss.str());
      }
      return res;
    }
    // returns -1 if file doesn't exist:
    float_t getf(const std::string& s) const {
      if (not std::filesystem::is_regular_file(dir / s)) return -1;
      float_t res;
      try { res = FP::to_float80(get(s)); }
      catch (const file_error&) { throw; }
      catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::getf: "
          "Error getting floating-point number from\n  "
           << dir / s << "\n   original error is\n  " << e.what() << "\n";
        throw number_error(ss.str());
      }
      return res;
    }
    DimacsTools::DimacsClauseList getcnf() const {
      const auto path = dir / "cnf";
      std::ifstream file(path);
      if (not file) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::getcnf: CNF " << path <<
           "can not be opened for reading.\n";
        throw file_error(ss.str());
      }
      DimacsTools::DimacsClauseList res;
      try { res = DimacsTools::read_strict_Dimacs(file); }
      catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::getcnf: "
          "Error reading strict DIMACS file\n  "
           << path << "\n   original error is\n  " << e.what() << "\n";
        throw dimacs_error(ss.str());
      }
      return res;
    }
    DimacsTools::DimacsClauseList getcnf2() const {
      const auto path = dir / "cnf2";
      std::ifstream file(path);
      if (not file) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::getcnf2: CNF2 " << path <<
           "can not be opened for reading.\n";
        throw file_error(ss.str());
      }
      DimacsTools::DimacsClauseList res;
      try { res = DimacsTools::read_strict_Dimacs(file); }
      catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::getcnf2: "
          "Error reading strict DIMACS file\n  "
           << path << "\n   original error is\n  " << e.what() << "\n";
        throw dimacs_error(ss.str());
      }
      return res;
    }
    DimacsTools::MDimacsClauseList getmcnf2() const {
      const auto path = dir / "cnf2";
      std::ifstream file(path);
      if (not file) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::getmcnf2: CNF2 " << path <<
           "can not be opened for reading.\n";
        throw file_error(ss.str());
      }
      const auto mpath = dir / "mul2";
      std::ifstream mfile(mpath);
      if (not mfile) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::getmcnf2: MUL2 " << mpath <<
           "can not be opened for reading.\n";
        throw file_error(ss.str());
      }
      DimacsTools::MDimacsClauseList res;
      try { res = DimacsTools::read_strict_MDimacs(file, mfile); }
      catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "DirStatistics::adir::getmcnf2: "
          "Error reading strict M-DIMACS file\n  "
           << path << "\n  " << mpath <<
          "\n   original error is\n  " << e.what() << "\n";
        throw dimacs_error(ss.str());
      }
      return res;
    }

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
    static constexpr count_t N = 13;
    static std::string header() noexcept {
      return " i p1 p2 p3 p4 n c c2 E E2 cE cE2 tcol";
    }
    count_t i;
    std::array<count_t, 4> p;
    count_t n, c, c2, E, E2;
    float_t cE, cE2, tcol; // value -1 encodes NA

    AData(const adir& a) : i(a.i), p(extrpath(a)), n(a.n), c(a.c), c2(a.c2),
                           E(a.getu("E")), E2(a.getu("E2")),
                           cE(a.getf("cE")), cE2(a.getf("cE2")),
                           tcol(a.getf(bipart_file)) {}

    typedef std::array<float_t, N> extract_t;
    extract_t extract() const noexcept {
      using t = float_t;
      return {t(i), t(p[0]), t(p[1]), t(p[2]), t(p[3]), t(n), t(c), t(c2),
          t(E), t(E2), cE, cE2, tcol};
    }

    struct parse_error : std::runtime_error {
      parse_error(std::string m) noexcept : std::runtime_error(std::move(m)) {}
    };
    static std::array<count_t, 4> extrpath(const adir& a) {
      std::vector<count_t> res;
      try { res = FloatingPoint::to_vec_unsigned<count_t>(a.p, '/'); }
      catch (const std::exception& e) {
        std::ostringstream ss;
        ss << "DirStatistics::AData::extrpath: "
          "Error getting vector of 64-bit unsigned int from\n  "
           << a.dir / "p" << " = \"" << a.p << "\"\n   original error is\n  "
           << e.what() << "\n";
        throw parse_error(ss.str());
      }
      if (res.size() != 4) {
        std::ostringstream ss;
        ss << "DirStatistics::AData::extrpath: "
          "Not exactly 4 components in\n  "
           << a.dir / "p" << " = " << a.p << "\n";
        throw parse_error(ss.str());
      }
      return {res[0], res[1], res[2], res[3]};
    }

    friend std::ostream& operator <<(std::ostream& out, const AData& a) {
      out << a.i << " ";
      Environment::out_line(out, a.p);
      out << " " << a.n << " " << a.c << " "
          << a.c2 << " " << a.E << " " << a.E2 << " "
          << a.cE << " " << a.cE2 << " " << a.tcol;
      return out;
    }
  };

  void formatted_output(std::ostream& out, std::vector<AData>& V) {
    const auto header =
      Environment::split(Environment::transform_spaces(AData::header()), ' ');
    assert(header.size() == AData::N);
    std::vector<AData::extract_t> M; M.reserve(V.size());
    for (const auto& d : V) M.push_back(d.extract());
    Environment::print2dformat(out, M, 1, header);
  }

}

#endif
