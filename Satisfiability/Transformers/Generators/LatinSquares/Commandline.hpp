// Oliver Kullmann, 20.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef COMMANDLINE_gGSDChiUMd
#define COMMANDLINE_gGSDChiUMd

#include <string>
#include <iostream>
#include <exception>
#include <utility>

#include <cstdlib>

#include "Statistics.hpp"
#include "Errors.hpp"

namespace Commandline {

  using dim_t = Statistics::dim_t;
  typedef std::pair<dim_t, bool> dim_plus_info_t;

  // Boolean is true iff leading plus:
  dim_plus_info_t read_dim(const std::string arg, const std::string error) {
    using Errors::Error;
    if (arg.empty()) {
      std::cerr << error << "N-argument is empty.\n";
      std::exit(int(Error::conversion));
    }
    const bool leading_plus = arg[0] == '+';
    unsigned long d;
    std::size_t converted;
    try { d = std::stoul(arg, &converted); }
    catch (const std::invalid_argument& e) {
      std::cerr << error << "The argument \"" << arg
                << "\" is not a valid integer.\n";
      std::exit(int(Error::conversion));
    }
    catch (const std::out_of_range& e) {
      std::cerr << error << "The argument \"" << arg
                << "\" is too big for unsigned long.\n";
      std::exit(int(Error::too_big));
    }
    if (converted != arg.size()) {
      std::cerr << error << "The argument \"" << arg
                << "\" contains trailing characters: \""
        << arg.substr(converted) << "\".\n";
      std::exit(int(Error::conversion));
    }
    if (d == 0) {
      std::cerr << error << "An argument is 0.\n";
      std::exit(int(Error::too_small));
    }
    const dim_t cd = d;
    if (cd != d) {
      std::cerr << error << "The argument \"" << arg
                << "\" is too big for dim_t (max 65535).\n";
      std::exit(int(Error::too_big));
    }
    return {cd, leading_plus};
  }

}

#endif
