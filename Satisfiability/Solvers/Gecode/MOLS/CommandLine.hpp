// Oliver Kullmann, 27.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading the command-line arguments

*/

#ifndef COMMANDLINE_hcOzkjbxbQ
#define COMMANDLINE_hcOzkjbxbQ

#include <exception>
#include <sstream>

#include <cassert>

#include <Numerics/NumInOut.hpp>

#include "Conditions.hpp"
#include "Encoding.hpp"
#include "Options.hpp"

namespace CommandLine {

  namespace CD = Conditions;
  namespace EC = Encoding;
  namespace OP = Options;

  typedef CD::size_t size_t;

  size_t read_N(const int argc, const char* const argv[]) {
    assert(argc >= 2);
    const size_t N = FloatingPoint::to_UInt(argv[1]);
    if (not EC::EncCond::valid(N)) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_N]: value N=" << N << " is not allowed.";
      throw std::runtime_error(ss.str());
    }
    return N;
  }

}

#endif
