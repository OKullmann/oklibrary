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

MOLS> time ./TAUscan 10 data/SpecsCollection/3MOLS/symmb "" enu wdL hash 0.1 10 10 all
10 : 8.67774578323596440922e+24 1.22450048735265398083e+36 1.14699293230448961192e+37; 3.60495586501903848797e+36
TS_10_10_1695122450839812219.R
real	0m44.558s
user	5m58.006s
sys	0m22.757s
MOLS> cat TS_10_10_1695122450839812219.R
# TAUscan 0.9.0 341e8762241f4a1eac0a1f6e2cf4eb6da240e5ae laMols=0.102.0
# "./TAUscan" "10" "data/SpecsCollection/3MOLS/symmb" "" "enu" "wdL" "hash" "0.1" "10" "10" "all"
 estlvs
1 2.7147946178118402006e+34
2 1.1469929323044896119e+37
3 4.3558281368091233982e+32
4 8.6777457832359644092e+24
5 1.5442854410081259727e+27
6 1.1445825129039211791e+35
7 7.9915856021724196495e+31
8 6.6325978061787310117e+31
9 6.1809392887493355786e+35
10 1.4793597937472000791e+34


Above "tauprob" (tprob) was used, now "uniform random" (rand):

MOLS> time ./TAUscan 10 data/SpecsCollection/3MOLS/symmb "" enu,rand wdL hash 0.1 10 10 all
10 : 6.57489838704741983453e+30 9.25281428686421350058e+33 4.79310092415756905961e+34; 1.82953695554334109691e+34
TS_10_10_1695122539524332866.R
real	0m41.155s
user	5m51.190s
sys	0m20.980s
MOLS> cat TS_10_10_1695122539524332866.R
# TAUscan 0.9.0 341e8762241f4a1eac0a1f6e2cf4eb6da240e5ae laMols=0.102.0
# "./TAUscan" "10" "data/SpecsCollection/3MOLS/symmb" "" "enu,rand" "wdL" "hash" "0.1" "10" "10" "all"
 uestlvs
1 1.5219672192239397765e+31
2 4.7931009241575690596e+34
3 7.8012710354127100578e+30
4 1.2944331199499607799e+31
5 3.9449390322284519006e+34
6 3.5948256931181767944e+33
7 2.7737852570356302427e+31
8 1.4382590221666230889e+33
9 4.4380564112570083883e+31
10 6.5748983870474198345e+30



BUGS:

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
        "0.9.0",
        "19.9.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TAUscan.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace CommandLine;
  using namespace Options;
  namespace PSC = ParSysCalls;
  namespace LB = LookaheadBranching;

  constexpr int commandline_args_transfer = 7;
  constexpr int commandline_args_own = 3;
  constexpr int commandline_args =
    commandline_args_transfer  + commandline_args_own;
  static_assert(commandline_args_laMols == 14);
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
      " N file_cond file_ps branch-type distance init-seeds weights"
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
      "     - may be \"hash\", hashing the weights\n"
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

  // Argument weightsarg as computed by weights_arg (below):
  std::string seed_arg(std::string initseedarg,
                       const std::string& weightsarg) {
    Environment::mremove_spaces(initseedarg);
    std::string res = ";";
    if (initseedarg == "hash")
      return res + std::to_string(Environment::hash(weightsarg)) + ",";
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
  const std::string
    Narg_1 = argv[1],
    filecondarg_2 = qu(argv[2]),
    filepsarg_3 = qu(argv[3]),
    runtypearg_4 = "count",
    proplevelarg_5 = "dom",
    branchtypearg = que(argv[4]),
    distancearg_7 = que(argv[5]),
    initseedarg = argv[6],
    latypearg_9 = "relpr",
    weightsarg = argv[7];
  const std::string Marg = argv[8], threadsarg = argv[9];

  const auto [branchtypearg_6, prefix_branchorder] =
    read_branchtype(branchtypearg);
  const std::string valuesel = prefix_branchorder != special_bro ?
    default_sel : special_sel;
  
  const size_t
    M = read_M(Marg),
    threads = read_threads(threadsarg);
  const auto select0 = Environment::read<STTS>(argv[10]);
  if (not select0) {
    std::cerr << error << "Unknown selection-option \"" << argv[10] <<
      "\".\n";
    return 1;
  }
  const STTS select = select0.value();

  const std::string
    gcdarg_10 = "1",
    threadsarg_11 = "1",
    weightsarg_12 = weights_arg(weightsarg), // cin has been read
    branchorderarg = // completed below; no spaces allowed here by laMols
      prefix_branchorder + seed_arg(initseedarg, weightsarg_12),
    stoparg_13 = "lvs,0",
    formattingarg_14 = valuesel + ",-info,-w,-stop";

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
