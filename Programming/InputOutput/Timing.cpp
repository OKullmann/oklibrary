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
#include <string>
#include <vector>
#include <stdexcept>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <SystemSpecifics/SystemCalls.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.2",
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
    "> " << proginfo.prg << " command-string [expected-codes] [N]\n"
 ;
    return true;
  }

  enum class Error {
    pnumber = 1,
    empty_command = 2,
    invalid_code = 3,
    invalid_N = 4,
  };

  using FloatingPoint::UInt_t;
  using FloatingPoint::float80;

  constexpr UInt_t N_default = 20;

  typedef std::vector<int> codes_t;
  codes_t read_codes(const std::string arg) {
    if (arg.empty()) return {0};
    const auto list = Environment::split(arg, ',');
    codes_t res; res.reserve(list.size());
    for (const std::string x : list) {
      try { res.push_back(std::stoi(x)); }
      catch (const std::invalid_argument& e) {
        std::cerr << error << "The code \"" << x <<
        "\" is not a valid int.\n";
        std::exit(int(Error::invalid_code));
      }
    }
    return res;
  }

  UInt_t read_N(const std::string arg) {
    if (arg.empty()) return N_default;
    try { return FloatingPoint::toUInt(arg); }
    catch (const std::invalid_argument& e) {
        std::cerr << error << "The N-value \"" << arg <<
        "\" does not convert to a valid 64-bit unsigned int.\n";
        std::exit(int(Error::invalid_N));
    }
    catch (const std::domain_error& e) {
        std::cerr << error << "The N-value has trailing characters:\n"
                  << e.what() << "\n";
        std::exit(int(Error::invalid_N));
    }
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc < 2) {
    std::cerr << error << "At least one parameter is required:\n"
      " - the string with the command to be run.\n";
    return int(Error::pnumber);
  }

  const std::string command =
    Environment::remove_leadingtrailing_spaces(argv[1]);
  if (command.empty()) {
    std::cerr << error << "Empty command-string.\n";
    return int(Error::empty_command);
  }

  const codes_t codes = read_codes(argc == 2 ? "" : argv[2]);

  const UInt_t N = read_N(argc <= 3 ? "" : argv[3]);

}
