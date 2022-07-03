// Oliver Kullmann, 3.7.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Running M randomsised runs on laMols going down one branch,
  collecting statistics estlvs and printing them to standard output

*/

#include <iostream>
#include <string>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include "CommandLine.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.3",
        "3.7.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TAUscan.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace CommandLine;

  constexpr int commandline_args_transfer = 8;
  constexpr int commandline_args = commandline_args_transfer  + 2;
  static_assert(commandline_args_laMols == 14);

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " has " << commandline_args << " command-line arguments:\n"
      " N file_cond file_ps run-type prop-level distance la-type weights"
      "  M threads\n\n"
      " - the first " << commandline_args_transfer << " arguments are"
      " transferred to laMols\n"
      " - M        : number of runs (unsigned integer)\n"
      " - threads  : number of threads (unsigned integer).\n\n"
;
    return true;
  }

  size_t read_M(const std::string& arg) {
    return FloatingPoint::to_UInt(arg);
  }
  size_t read_threads(const std::string& arg) {
    return FloatingPoint::to_UInt(arg);
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc !=  commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed, but the real number is " << argc-1 << ".\n";
    return 1;
  }

  const std::string
    Narg_1 = argv[1],
    filecondarg_2 = argv[2],
    filepsarg_3 = argv[3],
    runtypearg_4 = argv[4],
    proplevelarg_5 = argv[5],
    distancearg_7 = argv[6],
    latypearg_9 = argv[7],
    weightsarg_12 = argv[8];
  const std::string Marg = argv[9], threadsarg = argv[10];
  
  const size_t
    M = read_M(Marg),
    threads = read_threads(threadsarg);

  const std::string
    branchtypearg_6 = "enu",
    branchorderarg_8 = "rand;", // XXX
    gcdarg_10 = "1",
    threadsarg_11 = "1",
    stoparg_13 = "lvs,0",
    formattingarg_14 = "estlvs,-info,-w,-stop";

}
