// Oliver Kullmann, 19.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General definitions regarding disassembly

*/

#ifndef DISASSEMBLE_Ao2I2cdXjQ
#define DISASSEMBLE_Ao2I2cdXjQ

#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <utility>

#include <cstdlib>

#include <ProgramOptions/Strings.hpp>
#include <Numerics/NumInOut.hpp>

#include "Bicliques2SAT.hpp"
#include "DimacsTools.hpp"

namespace Disassemble {

  typedef Bicliques2SAT::GlobRepl::size_t size_t;
  typedef Bicliques2SAT::GlobRepl::dimacs_pars dimacs_pars;


  constexpr char special = '|';
  bool special_dirname(const std::string& dirname) noexcept {
    return dirname.empty() or dirname[0] == special;
  }
  bool doublespecial(const std::string& dirname) noexcept {
    assert(not dirname.empty() and dirname[0] == special);
    return dirname.size() >= 2 and dirname[1] == special;
  }
  std::pair<std::filesystem::path, bool>
  extract_dir_path(const std::string& filename,
                   const std::string& dirname) {
    if (not special_dirname(dirname)) return {dirname, false};
    const std::filesystem::path path(filename), stem = path.stem(),
      root = path.parent_path();
    if (dirname.empty()) return {root / stem, false};
    const bool sanitising = doublespecial(dirname);
    const std::string addition = dirname.substr(sanitising ? 2 : 1);
    std::filesystem::path res = root;
    if (sanitising)
      res /= Environment::str2corename(stem.string());
    else
      res /= stem;
    res += addition;
    return {res, true};
  }

  std::pair<bool, size_t> extract_restriction(const int argc,
                                              const char* const argv[]) {
    if (argc == 3) return {false, 0};
    assert(argc == 4);
    const std::string s = argv[3];
    if (s.empty()) return {true, 0};
    return {true,FloatingPoint::toUInt(s)};
  }


  const std::string statsdirname = ".stats";
  const std::string ntccfile = "ntcc";
  const std::string nbccfile = "nbcc";
  const std::string nmapfile = "nm";
  const std::string cmapfile = "cm";

  std::filesystem::path statsdir_path(const std::filesystem::path& dir) {
    std::filesystem::path res(dir);
    res /= statsdirname;
    return res;
  }

  std::filesystem::path E0(const std::filesystem::path& dir) {
    return dir / "E0";
  }

  std::string par_part(const dimacs_pars dp, const size_t i) {
    std::stringstream ss;
    ss << dp.n << "_" << dp.c << "_" << i+1;
    return ss.str();
  }
  std::filesystem::path E(const std::filesystem::path& dir,
                          const dimacs_pars dp, const size_t i) {
    return dir / ("E_" + par_part(dp, i));
  }
  std::filesystem::path A(const std::filesystem::path& dir,
                          const dimacs_pars dp, const size_t i) {
    return dir / ("A_" + par_part(dp, i));
  }


  enum class Error {
    missing_parameters = 1,
    input_file_error = 2,
    syntax_error = 3,
    reading_error = 4,
    output_directory_exists = 5,
    output_directory_creation = 6,
    output_statsdir_creation = 7,
    output_E0_error = 8,
    output_E_error = 9,
    output_A_error = 10,
    output_stats_error = 11
  };


  DimacsTools::GslicedCNF read_GslicedCNF(std::ifstream& S,
                                          const std::string& es) {
    DimacsTools::GslicedCNF res;
    try { res = DimacsTools::read_strict_GslicedCNF(S); }
    catch (const std::invalid_argument& e) {
      std::cerr << es << "Invalid-argument thrown when reading input:\n   "
                << e.what() << "\n";
      std::exit(int(Error::syntax_error));
    }
    catch (const std::exception& e) {
      std::cerr << es << "Standard-xception thrown when reading input:\n   "
                << e.what() << "\n";
      std::exit(int(Error::reading_error));
    }
    catch (...) {
      std::cerr << es << "Exception thrown when reading input.\n";
      std::exit(int(Error::reading_error));
    }
    return res;
  }

  template <class X>
  void write_item(const X& x,
                  const std::string& filename,
                  const std::filesystem::path& dir,
                  const std::string& es) {
    const auto path = dir / filename;
    std::ofstream file(path);
    if (not file) {
      std::cerr << es << "Can not create statistics-file " << path << ".\n";
      std::exit(int(Error::output_stats_error));
    }
    file << x;
    if (not file) {
      std::cerr << es << "Can not write to statistics-file " << path << ".\n";
      std::exit(int(Error::output_stats_error));
    }
  }
  template <class M>
  void write_map(const M& m,
                  const std::string& filename,
                  const std::filesystem::path& dir,
                  const std::string& es) {
    const auto path = dir / filename;
    std::ofstream file(path);
    if (not file) {
      std::cerr << es << "Can not create statistics-file " << path << ".\n";
      std::exit(int(Error::output_stats_error));
    }
    Environment::out_pairs(file, m);
    if (not file) {
      std::cerr << es << "Can not write to statistics-file " << path << ".\n";
      std::exit(int(Error::output_stats_error));
    }
  }

}

#endif
