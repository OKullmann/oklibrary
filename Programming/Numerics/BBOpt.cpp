// Oliver Kullmann, 20.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE

*/

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Optimisation.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "20.12.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/BBOpt.cpp",
        "GPL v3"};

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> "<<proginfo.prg<< " M R S T optFILE [parameters for opt]\n\n"
 ;
    return true;
  }

  struct Func {
    void init(const int, const char* const []) {}
    Optimisation::y_t func(const Optimisation::vec_t& v) {
      assert(v.size() == 2);
      using namespace FloatingPoint;
      const float80 x = v[0], y = v[1];
      return sq(1.5L-x+x*y) + sq(2.25L-x+x*sq(y)) + sq(2.625L-x+x*cb(y));
    }
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  FloatingPoint::fullprec_float80(std::cout);
  std::cout << Optimisation::bbopt_rounds_app(argc, argv, Func()) << "\n";
}
