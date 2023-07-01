// Oliver Kullmann, 1.7.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for analysing qdimacs

*/

#ifndef QDIMACSSYNTAX_s4ZN5nxQ7J
#define QDIMACSSYNTAX_s4ZN5nxQ7J

#include <utility>

#include <cstdlib>

#include <ProgramOptions/Strings.hpp>
#include <Numerics/NumTypes.hpp>

namespace QDimacsSyntax {

  typedef Environment::tokens_t tokens_t;
  typedef FloatingPoint::uint_t level_t;
  typedef FloatingPoint::UInt_t count_t;
  typedef FloatingPoint::float80 float_t;

  enum class Error {
    missing_parameters = 1,
    input_file_error = 2,
  };

  const std::string is_incorrect = "ERROR";
  void syntax_error() noexcept {
    std::cout << is_incorrect << std::endl;
    std::exit(0);
  }

  // Line with first non-c-line, returns true iff c-lines have syntax-error:
  std::pair<count_t, bool> analyse_comments(const tokens_t& F) noexcept {
    count_t first_nonc = 0;
    for (; first_nonc < F.size(); ++first_nonc) {
      const auto& L = F[first_nonc];
      if (not L.starts_with("c")) break;
      if (not L.starts_with("c ")) return {first_nonc, true};
    }
  return {first_nonc, false};
  }

}

#endif
