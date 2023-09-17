// Oliver Kullmann, 15.9.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Applying a function, given as script, to a grid of points:
   - output to file
   - sorted by last component of output-vector.

EXAMPLES:

The 2d-grid:
> cat data/blackbox/g_2d
-10 -1 2 2 10
0 1 2 3 4

Reproducing the input:
> cat data/blackbox/s_cat
cat -
Adding the inputs:
> cat data/blackbox/s_add
input="$(cat -)"; echo "$input" | tr ' ' '+' | bc

First reproducing the inputs:

Numerics> ./BBScan_debug data/blackbox/g_2d data/blackbox/s_cat "" 1
BBs_g5F2d_data2Fblackbox2Fs5Fcat_1694960414816250236.R
Numerics> cat BBs_g5F2d_data2Fblackbox2Fs5Fcat_1694960414816250236.R
# "./BBScan_debug" "data/blackbox/g_2d" "data/blackbox/s_cat" "" "1"
# 2 2 9
# 0 :
 X1  X2   Y1  Y2
 -1   1   -1   1
0.5   1  0.5   1
  2   1    2   1
 -1   2   -1   2
0.5   2  0.5   2
  2   2    2   2
 -1   3   -1   3
0.5   3  0.5   3
  2   3    2   3

Explanation:
Input-dimension = 2, output-dimension = 2, 3*3 = 9 points, 0 seeds.

With randomisation:
> ./BBScan_debug data/blackbox/g_2d data/blackbox/s_cat "0" 1
BBs_g5F2d_data2Fblackbox2Fs5Fcat_1694960645767118375.R
> cat BBs_g5F2d_data2Fblackbox2Fs5Fcat_1694960645767118375.R
# "./BBScan_debug" "data/blackbox/g_2d" "data/blackbox/s_cat" "0" "1"
# 2 2 9
# 4 : 0 14842017794727834215 10005490731784888750 8630965937302476832
                      X1                     X2                        Y1                     Y2
 -0.26984558498791723951   2.018737656379828717   -0.26984558498791723951   2.018737656379828717
-0.070129503540719459913   2.018737656379828717  -0.070129503540719459913   2.018737656379828717
   1.7212280468720926698   2.018737656379828717     1.7212280468720926698   2.018737656379828717
 -0.26984558498791723951  2.6897230209697268345   -0.26984558498791723951  2.6897230209697268345
-0.070129503540719459913  2.6897230209697268345  -0.070129503540719459913  2.6897230209697268345
   1.7212280468720926698  2.6897230209697268345     1.7212280468720926698  2.6897230209697268345
 -0.26984558498791723951  2.9306611451987749635   -0.26984558498791723951  2.9306611451987749635
-0.070129503540719459913  2.9306611451987749635  -0.070129503540719459913  2.9306611451987749635
   1.7212280468720926698  2.9306611451987749635     1.7212280468720926698  2.9306611451987749635

Here now we have 4 seeds: the original seed(-sequence), plus 3 further hashs.

Adding the inputs:

> ./BBScan_debug data/blackbox/g_2d data/blackbox/s_add "" 1
BBs_g5F2d_data2Fblackbox2Fs5Fadd_1694960915359801025.R
> cat BBs_g5F2d_data2Fblackbox2Fs5Fadd_1694960915359801025.R
# "./BBScan_debug" "data/blackbox/g_2d" "data/blackbox/s_add" "" "1"
# 2 1 9
# 0 :
 X1  X2   Y1
 -1   1    0
 -1   2    1
0.5   1  1.5
 -1   3    2
0.5   2  2.5
  2   1    3
0.5   3  3.5
  2   2    4
  2   3    5

> ./BBScan_debug data/blackbox/g_2d data/blackbox/s_add "0" 1
BBs_g5F2d_data2Fblackbox2Fs5Fadd_1694960969853642415.R
Numerics> cat BBs_g5F2d_data2Fblackbox2Fs5Fadd_1694960969853642415.R
# "./BBScan_debug" "data/blackbox/g_2d" "data/blackbox/s_add" "0" "1"
# 2 1 9
# 4 : 0 14842017794727834215 10005490731784888750 14974945910891974697
                    X1                     X2                     Y1
0.24294871090993161366  1.4349311563374949226  1.6778798672474265363
0.24294871090993161366  2.1646930746624880356  2.4076417855724196492
0.24294871090993161366  2.4592800581410239585  2.7022287690509555721
 1.8438342065904413163  1.4349311563374949226   3.278765362927936239
 1.9681824407554592822  1.4349311563374949226  3.4031135970929542048
 1.8438342065904413163  2.1646930746624880356  4.0085272812529293518
 1.9681824407554592822  2.1646930746624880356  4.1328755154179473178
 1.8438342065904413163  2.4592800581410239585  4.3031142647314652746
 1.9681824407554592822  2.4592800581410239585  4.4274624988964832407

We see that the hash for the script(-name) is different.


Above in the randomised forms, the uniform distribution is used.
Now using additional equidistance ("-") and "boxed" ("+") distribution,
together with "latin hypercube sampling" (due to using "e0"):

s> cat data/blackbox/g_5d
-10 -1 +9e0 9 10
0 1 -9e0 10 10
-inf 100 9e0 200 +inf
0 0 +9e0 10 10
-10 -10 -9e0 -1 inf

The "9" means that 9+1=10 points are created.

> ./BBScan_debug data/blackbox/g_5d data/blackbox/s_add "0" 1
BBs_g5F5d_data2Fblackbox2Fs5Fadd_1694961933763192830.R
Numerics> cat BBs_g5F5d_data2Fblackbox2Fs5Fadd_1694961933763192830.R
# "./BBScan_debug" "data/blackbox/g_5d" "data/blackbox/s_add" "0" "1"
# 5 1 10
# 4 : 0 721945341853075457 7457344390317643264 14974945910891974697
                      X1  X2                     X3                      X4   X5                     Y1
   6.0853952803682812176   6  100.66589147358692353   1.8271925799483893617   -2  112.57847933390359411
   4.6376062966163948548   3  118.45591875073528145   2.7098389154424130823   -8  120.80336396279408939
  0.76692696386467083976   8  117.55094013020433216   9.7150897326513421864   -5  131.03295682672034518
   7.8189375980268962575   1  138.16771233333139528  0.12683161671838181741   -9  138.11348154807667335
-0.020840240050511162637  10  127.18486113178383581   5.4036719753600970864   -3  139.56769286709342173
   5.9165713434689220268   4  147.37569791644085163   8.0941780343001156891  -10  155.38644729420988935
   1.0564359034538061718   9  149.83961872974521592   3.5188061905603327465   -1  162.41486082375935483
   2.9690461445941889222   2  172.97228870378465133   7.0952776146281270869   -4  181.03661246300696734
   3.7147340886692604078   7  193.72941276340531798   6.8601271478212146222   -6  205.30427399989579301
   8.6830987445522149551   5  195.52544781982753072   4.9233970992828493328   -7  207.13194366366259501

We see for X1, X4 the boxed distribution is used:
 - -0.x, +0.x, 1.x, 2.x, ..., 8.x
 - 0.x, 1.x, ..., 9.x.
While for X3 we have uniform randomness (in [100,200]), and for X2, X5 we have
equidistant sampling (always including the bounds).

*/

#include <iostream>

#include <cassert>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Numerics/NumInOut.hpp>
#include <Transformers/Generators/Bicliques/Algorithms.hpp>

#include "OptTypes.hpp"
#include "Sampling.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "17.9.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/BBScan.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  namespace FP = FloatingPoint;
  namespace OS = Optimisation;
  namespace SP = Sampling;

  using index_t = OS::index_t;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " grid script seeds threads\n\n"
    " grid           : " << "filename\n"
    " script         : " << "string\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"\n"
    " threads        : " << "natural number >= 0\n\n"
    " applies the script to the points of the script :\n\n"
    "  - the output is stored in file \"BBs_grid_script_timestamp\"\n"
    "  - here \"grid\" and \"script\" are simplifications of the"
    " corresponding strings.\n\n"
;
    return true;
  }

  index_t read_threads(const std::string& s) {
    index_t res;
    try { res = FP::touint(s); }
    catch (const std::exception& e) {
      std::cerr << error <<
        "Reading-error for number of threads, with error-message\n  " <<
        e.what();
      std::exit(int(OS::Error::faulty_parameters));
    }
    return res;
  }

  std::vector<std::string> header(const index_t N, const index_t M) {
    std::vector<std::string> res; res.reserve(N+M);
    for (index_t i = 1; i <= N; ++i)
      res.push_back(std::string("X") + std::to_string(i));
    for (index_t i = 1; i <= M; ++i)
      res.push_back(std::string("Y") + std::to_string(i));
    return res;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 5) {
    std::cerr << error <<
      "Exactly four arguments (grid, script, seeds, threads)"
      " needed, but " << argc-1 << " provided.\n";
    return int(OS::Error::missing_parameters);
  }

  const std::string grid = argv[1];
  std::ifstream grids(grid);
  if (not grids) {
    std::cerr << error <<
      "Reading-error for grid-file \"" << grid << "\".\n";
    return int(OS::Error::faulty_parameters);
  }
  const std::string script = argv[2];
  if (script.empty()) {
    std::cerr << error <<
      "Empty script-string.\n";
    return int(OS::Error::faulty_parameters);
  }
  const RandGen::vec_eseed_t seeds0 = RandGen::extract_seeds(argv[3]);
  const bool randomised = not seeds0.empty();
  const index_t threads = read_threads(argv[4]);

  const std::string output = SP::scanning_output(grid, script);

  if (threads == 0) { // dry run
    const auto [I,x] = OS::read_scanning_info(grids);
    const index_t N = I.size();
    std::cout << "# ";
    Environment::args_output(std::cout, argc, argv);
    std::cout << "\n# " << output << "\n# N=" << N << " #points=";
    OS::vec_t dummy(N);
    std::cout << size_scanning(std::get<0>(
                   SP::prepare_scanning(x,I,seeds0,randomised,dummy))) << "\n";
    return 0;
  }
  else {
    std::cout << output << std::endl;
    std::ofstream outputs(output);
    if (not outputs) {
      std::cerr << error <<
        "Can not open output-file \"" << output << "\".\n";
      return int(OS::Error::write_error);
    }
    outputs << "# ";
    Environment::args_output(outputs, argc, argv);
    outputs << std::endl;
    RandGen::vec_eseed_t seeds(seeds0);
    const auto [X,FX] = SP::perform_scanning_script(grids, seeds, randomised,
                                                    script, threads);
    if (X.empty()) return 0;
    const auto size = X.size();
    assert(size == FX.size());
    const index_t N = X[0].size();
    const index_t M = FX[0].size();
    outputs << "# " << N << " " << M << " " << size << "\n# "
            << seeds.size() << " : ";
    Environment::out_line(outputs, seeds);
    outputs << std::endl;
    auto matrix = Algorithms::append2d_ranges(X, FX);
    std::ranges::stable_sort(matrix, [](const auto& x, const auto& y){
                               return x.back() < y.back();});
    FP::fullprec_float80(outputs);
    Environment::print2dformat(outputs, matrix, 2, header(N, M));
  }

}
