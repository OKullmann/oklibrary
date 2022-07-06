// Oliver Kullmann, 3.7.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Running M randomsised runs on laMols going down one branch,
  collecting statistics on estlvs and printing them to standard output

BUGS:

1. File access error:

terminate called after throwing an instance of 'std::runtime_error'
  what():  ERROR[Environment::get_content(p)]: Can't open file
  SystemCalls_esystem_out_1657033265110086420_2235
/home/kullmann/oklibrary/Satisfiability/Solvers/Gecode/MOLS/data/SpecsCollection/3MOLS/TAU_diag_7_wdL_ave/./ns: line 2:  2225 Done                    cat $1
      2235 Aborted                 (core dumped) | ./TAUscan 7 ../diag "" count "" wdL t "" cin 100 10 ""
Warning: Evaluation error with point #75981 ( 7 6.0001 0.0001 0.0001 7 )        Evaluation error         : output is empty
Warning: Evaluator returned exit status 34304 for point: ( 7 6.0001 0.0001 0.0001 7 )

 (a) The uniqueness of file-names needs to be checked:
    - A hash-value of the inputs should be part of the filenames created
      (by esystem).

TODOS:

1. Make computations deterministic
    - Instead of "t" (used by the nomad-script) a hash of the weights-string
      should be used (provided by TAUscan itself).
    - Triggered by "hash", which is automatically used when using "cin".

2. We should provide the possibility to specify the solver-path ("./laMols")
   as a command-line argument.
    - Likely best for the experiments to make a local copy, in an
      experiment-directory.
    - So then acutally "./laMols" seems appropriate.

*/

#include <iostream>
#include <string>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <SystemSpecifics/ParSysCalls.hpp>
#include <Numerics/Statistics.hpp>

#include "CommandLine.hpp"
#include "LookaheadBranching.hpp"
#include "Options.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.1",
        "6.7.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TAUscan.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace CommandLine;
  using namespace Options;
  namespace PSC = ParSysCalls;
  namespace LB = LookaheadBranching;

  constexpr int commandline_args_transfer = 9;
  constexpr int commandline_args_own = 3;
  constexpr int commandline_args =
    commandline_args_transfer  + commandline_args_own;
  static_assert(commandline_args_laMols == 14);
  const std::string solver_call = "./laMols";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " has " << commandline_args << " command-line arguments:\n\n"
      " N file_cond file_ps run-type prop-level distance init-seeds la-type"
      " weights"
      "  M threads selection\n\n"
      " - the first " << commandline_args_transfer << " arguments are"
      " transferred to \"" << solver_call << "\"\n"
      "  - init-seeds is the initial seed-sequence for random branching\n"
      " - M         : number of runs (unsigned integer)\n"
      " - threads   : number of threads (unsigned integer)\n"
      " - selection : " << Environment::WRPO<STTS>{} << "\n\n"
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

  const std::string bro = "tprob";
  std::string seed_arg(std::string initseedarg) {
    Environment::remove_spaces(initseedarg);
    std::string res = bro + ";" + initseedarg;
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
  const auto select0 = Environment::read<STTS>(argv[12]);
  if (not select0) {
    std::cerr << error << "Unknown selection-option \"" << argv[12] <<
      "\".\n";
    return 1;
  }
  const STTS select = select0.value();

  const std::string
    branchtypearg_6 = "enu",
    branchorderarg = seed_arg(initseedarg), // running seed appended
    gcdarg_10 = "1",
    threadsarg_11 = "1",
    weightsarg_12 = weights_arg(weightsarg), // cin read
    stoparg_13 = "lvs,0",
    formattingarg_14 = "estlvs,-info,-w,-stop";

  PSC::vargs_t calls; calls.reserve(M);
  for (size_t seed = 0; seed < M; ++seed) {
    const std::string branchorderarg_8 =
      Environment::qu(branchorderarg + std::to_string(seed));
    const std::string argument_list =
      Narg_1 + " " + filecondarg_2 + " " + filepsarg_3 + " " +
      runtypearg_4 + " " + proplevelarg_5 + " " +
      branchtypearg_6 + " " + distancearg_7 + " " +
      branchorderarg_8 + " " + latypearg_9 + " " + gcdarg_10 + " " +
      threadsarg_11 + " " + weightsarg_12 + " " + stoparg_13 + " " +
      formattingarg_14;
    calls.push_back({solver_call + " " + argument_list});
  }
  const auto res = PSC::run_parallel(calls, threads);
  assert(res.size() == M);
  LB::vec_t results; results.reserve(M);
  for (const auto& r : res) {
    if (not r) {
      std::cerr << error << "A " << solver_call << "-run resulted in an"
        " error:\n" << r.value().err << "\n";
      return 1;
    }
    results.push_back(FloatingPoint::to_float80(
      Environment::remove_leadingtrailing_spaces(r.value().out)));
  }
  const GenStats::StdVFourStats stats(results);
  [[maybe_unused]] const auto oldprec =
    FloatingPoint::fullprec_float80(std::cout);
  [[maybe_unused]] const auto oldstate =
    std::cout.setf(std::ios_base::scientific, std::ios_base::floatfield);
  switch (select) {
  case STTS::ave : std::cout << stats.amean; break;
  case STTS::min : std::cout << stats.min; break;
  case STTS::max : std::cout << stats.max; break;
  case STTS::stddev : std::cout << stats.sdc; break;
  case STTS::avepsd : std::cout << stats.amean + stats.sdc; break;
  case STTS::all : std::cout << stats.extract(); }
  std::cout << "\n";
}
