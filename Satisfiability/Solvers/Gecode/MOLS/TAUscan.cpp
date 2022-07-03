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
        "0.1.0",
        "3.7.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TAUscan.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace CommandLine;

  constexpr int commandline_args_transfer = 9;
  constexpr int commandline_args = commandline_args_transfer  + 2;
  static_assert(commandline_args_laMols == 14);

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " has " << commandline_args << " command-line arguments:\n"
      " N file_cond file_ps run-type prop-level distance init-seeds la-type weights"
      "  M threads\n\n"
      " - the first " << commandline_args_transfer << " arguments are"
      " transferred to laMols\n"
      "  - where init-seeds is the initial seed-sequence for random branching\n"
      " - M        : number of runs (unsigned integer)\n"
      " - threads  : number of threads (unsigned integer).\n\n"
;
    return true;
  }


  // With quotes if empty:
  std::string que(std::string s) {
    if (not s.empty()) return s;
    else return Environment::qu(s);
  }

  size_t read_M(const std::string& arg) {
    return FloatingPoint::to_UInt(arg);
  }
  size_t read_threads(const std::string& arg) {
    return FloatingPoint::to_UInt(arg);
  }

  std::string seed_arg(std::string initseedarg) {
    Environment::remove_spaces(initseedarg);
    std::string res = "rand;" + initseedarg;
    const char last = res.back();
    if (last != ';' and last != ',') res.push_back(',');
    return res;
  }

  std::string weights_arg(std::string weightsarg) {
    using namespace Environment;
    if (weightsarg != "cin") {
      mremove_spaces(weightsarg); return que(weightsarg);
    }
    else
      return que(transform_spaces(get_content(std::cin), ','));
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

  using Environment::qu;
  const std::string
    Narg_1 = argv[1],
    filecondarg_2 = qu(argv[2]),
    filepsarg_3 = qu(argv[3]),
    runtypearg_4 = que(argv[4]),
    proplevelarg_5 = que(argv[5]),
    distancearg_7 = que(argv[6]),
    initseedarg = argv[7],
    latypearg_9 = que(argv[8]),
    weightsarg = argv[9];
  const std::string Marg = argv[10], threadsarg = argv[11];
  
  const size_t
    M = read_M(Marg),
    threads = read_threads(threadsarg);

  const std::string
    branchtypearg_6 = "enu",
    branchorderarg = seed_arg(initseedarg), // running seed appended
    gcdarg_10 = "1",
    threadsarg_11 = "1",
    weightsarg_12 = weights_arg(weightsarg), // cin read
    stoparg_13 = "lvs,0",
    formattingarg_14 = "estlvs,-info,-w,-stop";

  for (size_t seed = 0; seed < M; ++seed) {
    const std::string branchorderarg_8 = branchorderarg + std::to_string(seed);
    const std::string argument_list =
      Narg_1 + " " + filecondarg_2 + " " + filepsarg_3 + " " +
      runtypearg_4 + " " + proplevelarg_5 + " " +
      branchtypearg_6 + " " + distancearg_7 + " " +
      branchorderarg_8 + " " + latypearg_9 + " " + gcdarg_10 + " " +
      threadsarg_11 + " " + weightsarg_12 + " " + stoparg_13 + " " +
      formattingarg_14;

    std::cerr << argument_list << "\n";
  }
}
