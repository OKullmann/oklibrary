// Oliver Kullmann, 26.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/Timing.cpp
  \brief Running a program many times, and measure performance

*/

#include <iostream>
#include <filesystem>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <SystemSpecifics/SystemCalls.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "26.2.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/InputOutput/Timing.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " command-string [options] [N]\n"
 ;
    return true;
  }

  enum class Error {
    pnumber = 1,
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3) {
    std::cerr << error << "Exactly two input parameters are required:\n"
      " - the Program (to be tested),\n"
      " - the Directory with the test-cases.\n";
    return int(Error::pnumber);
  }

}
