// Oliver Kullmann, 3.7.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Running M randomsised runs on laMols going down one branch,
  collecting statistics on estlvs (or uestlvs), and printing them to
  standard output

Examples:

With "all" we get all statistics, and the single results in a file
(here 10 probes and 10 threads):

MOLS> time ./TAUscan 10 data/SpecsCollection/3MOLS/symmb "" enu wdL h 0.1 10 10 all
10 : 2.86182157749415107420e+31 1.96759786066486124029e+34 1.27797960477944986434e+35; 4.19209525125229165856e+34
TS_10_10_1696012083070680529.R
real	0m45.490s
user	5m57.432s
sys	0m23.539s
MOLS> cat TS_10_10_1696012083070680529.R
# TAUscan 0.9.1 8b6df47156c79c4f4e1e378fb260a04c3791fd21 laMols=0.102.3
# "./TAUscan" "10" "data/SpecsCollection/3MOLS/symmb" "" "enu" "wdL" "h" "0.1" "10" "10" "all"
 estlvs
1 5.7559749953485999621e+34
2 3.5061446370603864919e+33
3 5.2944474814249570665e+33
4 7.4055336436209241046e+32
5 5.3416039821393925621e+32
6 1.0478888160218900889e+33
7 1.2779796047794498643e+35
8 1.7982881721044819227e+32
9 2.8618215774941510742e+31
10 7.0433904986482943963e+31


Above "tauprob" (tprob) was used, now "uniform random" (rand):

MOLS> time ./TAUscan 10 data/SpecsCollection/3MOLS/symmb "" enu,rand wdL h 0.1 10 10 all
10 : 2.34871484448138854400e+26 7.74379309173471536293e+33 3.55044512900560671082e+34; 1.38965740094092635693e+34
TS_10_10_1696012202917905314.R
real	0m40.298s
user	5m30.395s
sys	0m20.131s
MOLS> cat TS_10_10_1696012202917905314.R
# TAUscan 0.9.1 8b6df47156c79c4f4e1e378fb260a04c3791fd21 laMols=0.102.3
# "./TAUscan" "10" "data/SpecsCollection/3MOLS/symmb" "" "enu,rand" "wdL" "h" "0.1" "10" "10" "all"
 uestlvs
1 7.0132249461839144902e+31
2 3.8344807393260552475e+33
3 3.5504451290056067106e+33
4 2.348714844481388544e+26
5 3.5504451290056067108e+34
6 1.7042136619226912211e+33
7 4.0910478852739501193e+32
8 1.1414754144179548324e+31
9 2.5683196824403983729e+29
10 3.235343123806359115e+34



BUGS:

1. In the above examples we see that min and max are different from
   the results in the file (in the last digits)?


TODOS:

-1. Output of the probing-results as returned by laMols:
   - Documentation needs update (showing the filename-pattern).
   - Perhaps the filename should be more telling?
      - Including a simplified form of the specs (similar to BBScan).
      - DONE And for "all", also outputting the filename.
   - We also need "-all" (or another name? "mall"?), which shows all 5 numbers,
     but just space-separated (so no ":" and no ";"), and not
     creating the file with all cases.
   - DONE
     Easiest is to add to "case STTS::all", which currently
     only extracts the "four stats", also to (always) output
     the M single results, stored in variable results, to file.
   - DONE Filename "TS_M_threads_timestamp.R"
   - As comments:
    - DONE command-line inputs
    - DONE version of TAUscan and id
    - DONE version of laMols? how to obtain?
MOLS> ./laMols -v | grep "^ version"
 version:           0.101.2

?
      Better to make this available as a string-constant.
   - DONE header " estlvs"
   - DONE Then, numbered with 1, ..., the results, one per line.

0. Hashing should take all algorithmic parameters into account:
    - Similar to BBScan (in Numerics).
    - So hash N, the specs-object, the algorithmic options, and
      the weights-vector.
    - Not, as now, the strings, but the objects.
    - Perhaps then the hash-instruction needs to be transferred to
      laMols (since laMols only has all the data).
      So no hashing in TAUscan.
    - See Todo "Provide hashing for seeds" in laMols.cpp.

1. The handling of "tprob" versus "rand" is somewhat fragile.

2. More statistics on the distribution
    - Kurtosis https://en.wikipedia.org/wiki/Kurtosis
      seems relevant.

*/

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <fstream>

#include <cassert>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <SystemSpecifics/ParSysCalls.hpp>
#include <Numerics/Statistics.hpp>

#include "CommandLine.hpp"
#include "LookaheadBranching.hpp"
#include "Options.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.9.2",
        "25.11.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TAUscan.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace CommandLine;
  using namespace Options;
  namespace PSC = ParSysCalls;
  namespace LB = LookaheadBranching;

  constexpr int commandline_args_transfer = 8;
  constexpr int commandline_args_own = 3;
  constexpr int commandline_args =
    commandline_args_transfer  + commandline_args_own;
  const std::string solver_call = "./laMols";

  const std::string
    default_bro = "tprob", special_bro = "rand",
    default_sel = "estlvs", special_sel = "uestlvs";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " has " << commandline_args << " command-line arguments:\n\n"
      " N file_cond file_ps branch-type la-red distance init-seeds weights"
      "  M threads selection\n\n"
      " - the first " << commandline_args_transfer << " arguments are"
      " transferred to \"" << solver_call << "\":\n"
      "   - branch-type may contain a comma,"
      " after which the branching-order is given:\n"
      "     - by default \"" << default_bro << "\" is used, leading to \""
            << default_sel << "\"\n"
      "     - while \"" << special_bro << "\" leads to \"" << special_sel << "\"\n"
      "   - init-seeds is the initial seed-sequence for random branching\n"
      "     - may be empty\n"
      "     - may contain \"h\", hashing the main parameters\n"
      "   - weights may be \"cin\"\n"
      " - M         : number of runs or \"probes\" (unsigned integer)\n"
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

  std::array<std::string,2> read_branchtype(std::string branchtypearg) {
    const auto find = branchtypearg.find(',');
    if (find == std::string::npos) return {branchtypearg, default_bro};
    else return {branchtypearg.substr(0,find),
             branchtypearg.substr(find+1)};
  }

  // The seed-arg is to be completed by the unning seed for the probes:
  std::string seed_arg(std::string initseedarg) {
    Environment::mremove_spaces(initseedarg);
    std::string res = ";";
    res += initseedarg;
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

  std::string fileoutput_name(const size_t M, const size_t threads) {
    return "TS_" + std::to_string(M) + "_" + std::to_string(threads) +
      "_" + Environment::CurrentTime::timestamp_str() + ".R";
  }
  void fileoutput(const LB::vec_t& results,
                  const int argc, const char* const argv[],
                  const std::string& filename,
                  const std::string& valuesel) {
    std::ofstream output(filename);
    if (not output) {
      std::cerr << error << "Outputfile \"" << filename << "\" can not "
        "be created.\n";
      std::exit(1);
    }
    output << "# " << proginfo.prg << " " << proginfo.vrs << " "
           << proginfo.git << " laMols=" << version_laMols << "\n# ";
    Environment::args_output(output, argc, argv);
    output << "\n " << valuesel << "\n";
    FloatingPoint::fullprec_float80(output);
    for (size_t i = 0; i < results.size(); ++i)
      output << i+1 << " " << results[i] << "\n";
    if (not output) {
      std::cerr << error << "Write-error to outputfile \"" << filename
                << ".\n";
      output.close();
      std::exit(1);
    }
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
  // the components of argument_list (below):
  const std::string
    Narg_1 = argv[1],
    filecondarg_2 = qu(argv[2]),
    filepsarg_3 = qu(argv[3]),
    runtypearg_4 = "count",
    proplevelarg_5 = "dom",
    branchtypearg = que(argv[4]), // branchtypearg_6 see below
    laredarg_7 = que(argv[5]),
    distancearg_8 = que(argv[6]),
    initseedarg = argv[7],
    // branchorderarg_9 see below
    latypearg_10 = "relpr",
    weightsarg = argv[8];
  // gcdarg_11,threadsarg_12,weightsarg_13,stoparg_14,formattingarg_15 below
  const std::string Marg = argv[9], threadsarg = argv[10];

  const auto [branchtypearg_6, prefix_branchorder] =
    read_branchtype(branchtypearg);
  const std::string valuesel = prefix_branchorder != special_bro ?
    default_sel : special_sel;
  
  const size_t
    M = read_M(Marg),
    threads = read_threads(threadsarg);
  const auto select0 = Environment::read<STTS>(argv[11]);
  if (not select0) {
    std::cerr << error << "Unknown selection-option \"" << argv[11] <<
      "\".\n";
    return 1;
  }
  const STTS select = select0.value();

  const std::string
    gcdarg_11 = "1",
    threadsarg_12 = "1",
    weightsarg_13 = weights_arg(weightsarg), // cin has been read
    branchorderarg = // completed below; no spaces allowed here by laMols
      prefix_branchorder + seed_arg(initseedarg),
    stoparg_14 = "lvs,0",
    formattingarg_15 = valuesel + ",-info,-w,-stop";

  PSC::vargs_t calls; calls.reserve(M);
  for (size_t seed = 0; seed < M; ++seed) {
    const std::string branchorderarg_9 =
      Environment::qu(branchorderarg + std::to_string(seed));
    static_assert(commandline_args_laMols == 15);
    const std::string argument_list =
      Narg_1 + " " + filecondarg_2 + " " + filepsarg_3 + " " +
      runtypearg_4 + " " + proplevelarg_5 + " " +
      branchtypearg_6 + " " + laredarg_7 + " " + distancearg_8 + " " +
      branchorderarg_9 + " " + latypearg_10 + " " + gcdarg_11 + " " +
      threadsarg_12 + " " + weightsarg_13 + " " + stoparg_14 + " " +
      formattingarg_15;
    calls.push_back({solver_call + " " + argument_list});
  }
  const auto res = PSC::run_parallel(calls, threads);
  assert(res.size() == M);
  LB::vec_t results; results.reserve(M);
  for (const auto& r : res) {
    assert(r);
    const auto& value = r.value();
    if (value.rv.s != SystemCalls::ExitStatus::normal or
        not value.err.empty()) {
      std::cerr << error << "A " << solver_call << "-run resulted in an"
        " error: " << value.rv.s << ", with error-message:\n" << value.err
                << "\n";
      return 1;
    }
    assert(not value.out.empty());
    results.push_back(FloatingPoint::to_float80(
      Environment::remove_leadingtrailing_spaces(value.out)));
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

  case STTS::all : {
    const std::string outputname = fileoutput_name(M, threads);
    std::cout << stats.extract() << "\n" << outputname;
    std::cout.flush();
    fileoutput(results, argc, argv, outputname, valuesel);
  }}

  std::cout << "\n";
}
