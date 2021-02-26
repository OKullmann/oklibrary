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
#include <set>
#include <stdexcept>
#include <iomanip>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <Transformers/Generators/Random/Statistics.hpp>
#include <SystemSpecifics/SystemCalls.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
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
    remove_out = 5,
    remove_err = 6,
    os_error = 7,
    wrong_code = 8,
    system_call = 9,
  };

  using FloatingPoint::UInt_t;
  using FloatingPoint::float80;

  constexpr UInt_t N_default = 20;

  typedef std::set<int> codes_t;
  codes_t read_codes(const std::string arg) {
    codes_t res;
    if (arg.empty()) {res.insert(0); return res; }
    const auto list = Environment::split(arg, ',');
    for (const std::string x : list) {
      try { res.insert(std::stoi(x)); }
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

  float80 mb(const UInt_t Kb) { return float80(Kb) / 1024; }

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

  using Stats = RandGen::StatsStore<float80, float80>;
  Stats user, elapsed, system, usage, memory;

  const std::string out =
    SystemCalls::system_filename(proginfo.prg + "_stdout");
  const std::string err =
    SystemCalls::system_filename(proginfo.prg + "_stderr");
  const std::filesystem::path pout(out), perr(err);
  for (UInt_t i = 0; i < N; ++i) {
    try {
      const auto t = SystemCalls::tsystem(command, out, err);
      if (not codes.contains(t.x)) {
        std::cerr << error << "Return-code " << t.x << " not in given list:";
        for (const auto c : codes) std::cerr << " " << c;
        std::cerr << ".\n";
        return int(Error::wrong_code);
      }
      user += t.u; elapsed += t.e; system += t.s; usage += t.p;
      memory += mb(t.m);
      constexpr int width = 10;
      const auto w = std::setw(width);
      std::cout << w << i << w << t.u << w << t.e << w << t.s << w << t.p
                << w << mb(t.m) << "\n";
    }
    catch (const std::runtime_error& e) {
      std::cerr << error << "System call yields error:\n" << e.what();
      return int(Error::system_call);
    }
  }
  std::cout
    << N << "  " << user.min() << " " << user.amean() << " " << user.max()
    << "  " << elapsed.min() << " " << elapsed.amean() << " " << elapsed.max()
    << "  " << system.min() << " " << system.amean() << " " << system.max()
    << "  " << usage.min() << " " << usage.amean() << " " << usage.max()
    << "  " << memory.min() << " " << memory.amean() << " " << memory.max()
    << "\n";

  try {
    if (not std::filesystem::remove(pout)) {
      std::cerr << error << "File for removal does not exist:\n" << pout
                << "\n";
      return int(Error::remove_out);
    }
    if (not std::filesystem::remove(perr)) {
      std::cerr << error << "File for removal does not exist:\n" << perr
                << "\n";
      return int(Error::remove_err);
    }
  }
  catch (const std::filesystem::filesystem_error& e) {
    std::cerr << error << "OS-error when removing auxiliary files\n"
              << pout << "\n" << perr << "\n" << e.what();
    return int(Error::os_error);
  }

}
