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

#include <cstdlib>

#include "Bicliques2SAT.hpp"
#include "DimacsTools.hpp"

namespace Disassemble {

  typedef Bicliques2SAT::GlobRepl::size_t size_t;
  typedef Bicliques2SAT::GlobRepl::dimacs_pars dimacs_pars;


  std::filesystem::path extract_dir_path(const std::string& filename,
                                         const std::string& dirname) {
    if (dirname.empty()) {
      const std::filesystem::path p(filename);
      std::filesystem::path res = p.parent_path();
      res /= p.stem();
      return res;
    }
    else
      return dirname;
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
    output_directory_error = 5,
    output_E0_error = 6,
    output_E_error = 7,
    output_A_error = 8
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

}

#endif
