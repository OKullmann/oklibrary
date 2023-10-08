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

First (just) reproducing the inputs:

Numerics> ./BBSample_debug data/blackbox/g_2d data/blackbox/s_cat "" 1
BBS_g5F2d_data2Fblackbox2Fs5Fcat_1696746228118836801.R
Numerics> cat BBS_g5F2d_data2Fblackbox2Fs5Fcat_1696746228118836801.R
# "./BBSample_debug" "data/blackbox/g_2d" "data/blackbox/s_cat" "" "1"
# 2 2 3 9
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
Input-dimension = 2, output-dimension = 2, index sorting = 3 (Y2),
3*3 = 9 points, 0 seeds.

With randomisation:
> ./BBSample_debug data/blackbox/g_2d data/blackbox/s_cat "0" 1
BBS_g5F2d_data2Fblackbox2Fs5Fcat_1696746344955769638.R
BBS_g5F2d_data2Fblackbox2Fs5Fcat_1696746344955769638.R
Numerics> cat BBS_g5F2d_data2Fblackbox2Fs5Fcat_1696746344955769638.R
# "./BBSample_debug" "data/blackbox/g_2d" "data/blackbox/s_cat" "0" "1"
# 2 2 3 9
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

Now adding the inputs:

> ./BBSample_debug data/blackbox/g_2d data/blackbox/s_add "" 1
BBS_g5F2d_data2Fblackbox2Fs5Fadd_1696746421166888260.R
Numerics> cat BBS_g5F2d_data2Fblackbox2Fs5Fadd_1696746421166888260.R
# "./BBSample_debug" "data/blackbox/g_2d" "data/blackbox/s_add" "" "1"
# 2 1 2 9
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

> ./BBSample_debug data/blackbox/g_2d data/blackbox/s_add "0" 1
BBS_g5F2d_data2Fblackbox2Fs5Fadd_1696746472466880872.R
Numerics> cat BBS_g5F2d_data2Fblackbox2Fs5Fadd_1696746472466880872.R
# "./BBSample_debug" "data/blackbox/g_2d" "data/blackbox/s_add" "0" "1"
# 2 1 2 9
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

> ./BBSample_debug data/blackbox/g_5d data/blackbox/s_add "0" 1
BBS_g5F5d_data2Fblackbox2Fs5Fadd_1696746526007526083.R
Numerics> cat BBS_g5F5d_data2Fblackbox2Fs5Fadd_1696746526007526083.R
# "./BBSample_debug" "data/blackbox/g_5d" "data/blackbox/s_add" "0" "1"
# 5 1 5 10
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


A larger computation:

> cat data/blackbox/g_5d_100000
-inf 0 9e0 100 inf
-inf 100 99999e0 200 inf
-inf 200 99999e0 300 inf
-inf 300 99999e0 400 inf
-inf 400 99999e0 500 inf

> time ./BBSample data/blackbox/g_5d_100000 data/blackbox/s_mul "0" 0
# "./BBSample" "data/blackbox/g_5d_100000" "data/blackbox/s_mul" "0" "0"
# BBS_g5F5d5F100000_data2Fblackbox2Fs5Fmul_1696746573163284359.R
# N=5 index=-1 #points=100000
real	0m0.013s
user	0m0.013s
sys	0m0.000s

> time ./BBSample data/blackbox/g_5d_100000 data/blackbox/s_mul "0" 1
BBS_g5F5d5F100000_data2Fblackbox2Fs5Fmul_1696746609777230294.R
real	3m34.226s
user	4m5.435s
sys	0m38.781s
Numerics> head BBS_g5F5d5F100000_data2Fblackbox2Fs5Fmul_1696746609777230294.R
# "./BBSample" "data/blackbox/g_5d_100000" "data/blackbox/s_mul" "0" "1"
# 5 1 5 100000
# 4 : 0 1265311246719625050 7640216111640571731 16804024383169274529
                   X1                     X2                     X3                     X4                     X5                     Y1
12.121584215023058397  103.50248958601902424  203.14847669243306597   314.8818078144024164  403.30428231034261682  32367127079.832867019
12.121584215023058397  105.23727307899718077  206.12089431343614124  305.16294938926252481  412.28795185328165995  33081366039.278147824
12.121584215023058397  100.19201282466014255  202.17118590859793603  303.01237270581599209  445.94118914386234637  33177961050.796819285
12.121584215023058397  102.50658071979773519  206.07090424579942908  315.16483047089940628  417.46215386653917395   33688576836.65327842
12.121584215023058397   103.2575550445745163  205.66566693480223411  313.83059131181741122  419.08603749236650618  33856455252.827702433
12.121584215023058397  100.23070415111455041  218.36478740989419003  308.40569707355392939  413.96825525111727179  33871326301.666914733

Numerics> tail BBS_g5F5d5F100000_data2Fblackbox2Fs5Fmul_1696746609777230294.R
98.429974396756213982  199.57933053620069196  276.91112205372610289  391.67467207507510216  496.99203053500799457  1058908041092.3896691
98.429974396756213982  198.29773422842254245  281.06419749500010796  392.94067172660637463  491.93694184362252986  1060442371208.3908358
98.429974396756213982  194.61054481104895875  299.82883740535849648  386.06419189701822089  480.64418148311157863  1065737757787.4814643
98.429974396756213982  193.74468489991081234  287.36285396291546085  392.43358799795494385  498.10135176494213738  1071202774715.2347933
 92.56623920495920646  199.06373670667775934  298.58732151012535472  393.62434259767803538  498.97839863814528583  1080636986366.1684597
98.429974396756213982  191.92816271916592742  292.95877901694730466  394.85944828024405942   494.6148276463024506  1080891896505.2872863
98.429974396756213982  191.98847493493933924  297.29387638811581621   388.4457768906768512  497.60952817801671347  1085944391120.1821805
98.429974396756213982  199.63080652043846309  285.74246139774979256  397.11632831270225436   489.1228697916078629  1090599837960.3238195
98.429974396756213982  199.72792094995813607  289.31765793042891383   394.1943926988430332  497.59620546313177805  1115651609561.5780722
98.429974396756213982  199.06373670667775934  298.58732151012535472  393.62434259767803538  498.97839863814528583  1149091416198.6519789
> ls -l BBS_g5F5d5F100000_data2Fblackbox2Fs5Fmul_1696746609777230294.R
13700294

With 6 threads:
> time ./BBSample data/blackbox/g_5d_100000 data/blackbox/s_mul "0" 6
BBS_g5F5d5F100000_data2Fblackbox2Fs5Fmul_1696746959984118140.R
real	1m0.308s
user	4m57.755s
sys	1m6.879s
With 12 threads:
> time ./BBSample data/blackbox/g_5d_100000 data/blackbox/s_mul "0" 12
BBS_g5F5d5F100000_data2Fblackbox2Fs5Fmul_1696747033222199283.R
real	0m44.225s
user	5m15.627s
sys	1m29.561s


Remarks: the number of threads does not influence the results.


TODOS:

1. DONE As an optional argument, take the index of the Y-column
   to be sorted.
    - Default value is -1, which means the last argument.
    - -2 is the penultimate, and so on.
    - While 1 is the first one, and so on.
    - DONE (no)
      So the natural type perhaps is int?
    - 0 means no sorting.

*/

#include <iostream>
#include <utility>
#include <filesystem>

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
        "0.2.1",
        "8.10.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/BBSample.cpp",
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
         << " grid script seeds threads [sort-index=-1]\n\n"
    " grid           : " << "filename\n"
    " script         : " << "string\n"
    " seeds          : " << "sequence, can contain \"t\" or \"r\"\n"
    " threads        : " << "natural number >= 0\n"
    " sort-index     : " << "integer\n\n"
    " applies the script to the points of the grid :\n\n"
    "  - if threads is \"+1\", then scanning is logged (stdout)\n"
    "  - the output is stored in file \"" << SP::scanning_prefix <<
      "_grid_script_timestamp.R\"\n"
    "    - here \"grid\" and \"script\" are simplifications of the"
             " corresponding strings\n"
    "  - the final sorting-indices are N+1,...,N+M:\n"
    "    - N is the number of input-values, M is the number of output-values\n"
    "    - \"-1\" means the last index (N+M)\n"
    "    - \"1\" means the first index (N+1)\n"
    "    - \"0\" means no sorting.\n\n"
;
    return true;
  }

  std::filesystem::path read_script(const std::string& s) {
    if (s.empty()) {
      std::cerr << error <<
        "Empty script-string.\n";
      std::exit(int(OS::Error::faulty_parameters));
    }
    namespace fs = std::filesystem;
    const fs::path p(s);
    if (not fs::is_regular_file(p)) {
      std::cerr << error <<
        "Script " << p << " is not a regular file.\n";
      std::exit(int(OS::Error::faulty_parameters));
    }
    if (fs::perms::none ==
        (fs::status(p).permissions() & fs::perms::owner_exec)) {
      std::cerr << error <<
        "Script " << p << " is not executable.\n";
      std::exit(int(OS::Error::faulty_parameters));
    }
    return p;
  }

  // Second component true iff logging:
  std::pair<index_t, bool> read_threads(const std::string& s) {
    if (s == "+1") return {1, true};
    index_t res;
    try { res = FP::touint(s); }
    catch (const std::exception& e) {
      std::cerr
        << error << "Reading-error for number of threads \"" << s
        << "\", with error-message\n  " << e.what() <<"\n";
      std::exit(int(OS::Error::faulty_parameters));
    }
    return {res, false};
  }

  std::vector<std::string> header(const index_t N, const index_t M) {
    std::vector<std::string> res; res.reserve(N+M);
    for (index_t i = 1; i <= N; ++i)
      res.push_back(std::string("X") + std::to_string(i));
    for (index_t i = 1; i <= M; ++i)
      res.push_back(std::string("Y") + std::to_string(i));
    return res;
  }

  constexpr FP::Int_t default1_sorting_index = -1;
  constexpr FP::Int_t default2_sorting_index = 1;
  FP::Int_t read_sorting_index(const std::string& s) {
    if (s.empty()) return default2_sorting_index;
    FP::Int_t res;
    try { res = FP::to_Int(s); }
    catch (const std::exception& e) {
      std::cerr
        << error << "Reading-error for sorting-index \"" << s
        << "\", with error-message\n  " << e.what() <<"\n";
      std::exit(int(OS::Error::faulty_parameters));
    }
    return res;
  }
  constexpr index_t determine_sorting_index(FP::Int_t i,
                      const index_t N, const index_t M) noexcept {
    if (i == 0) return 0;
    assert(N >= 1); assert(M >= 1);
    const size_t num_vars = N+M;
    assert(num_vars > N); assert(num_vars > M);
    if (std::cmp_greater_equal(i,M)) return num_vars - 1;
    else if (std::cmp_greater_equal(-i,M)) return N;
    else return i > 0 ? N+(i-1) : num_vars + i;
  }
  static_assert(determine_sorting_index(-2,2,2) == 2);

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 5 and argc != 6) {
    std::cerr << error <<
      "Exactly four or five arguments (grid, script, seeds, threads,"
      " [sorting-index]) needed, but " << argc-1 << " provided.\n";
    return int(OS::Error::missing_parameters);
  }

  const std::string grid = argv[1];
  std::ifstream grids(grid);
  if (not grids) {
    std::cerr << error <<
      "Reading-error for grid-file \"" << grid << "\".\n";
    return int(OS::Error::faulty_parameters);
  }
  const std::filesystem::path script = read_script(argv[2]);
  const RandGen::vec_eseed_t seeds0 = RandGen::extract_seeds(argv[3]);
  const bool randomised = not seeds0.empty();
  const auto [threads, logging] = read_threads(argv[4]);
  const FP::Int_t sorting_index = argc == 5 ? default1_sorting_index
    : read_sorting_index(argv[5]);
  if (sorting_index == FP::mP263) {
    std::cerr << error <<
      "Sorting-index is " << FP::mP263 << ".\n";
    return int(OS::Error::faulty_parameters);
  }

  const std::string output = SP::scanning_output(grid, script);

  if (threads == 0) { // dry run
    const auto [I,x] = OS::read_scanning_info(grids);
    const index_t N = I.size();
    std::cout << "# ";
    Environment::args_output(std::cout, argc, argv);
    std::cout << "\n# " << output << "\n# N=" << N <<
      " index=" << sorting_index << " #points=";
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
    const auto [X,FX] =
      SP::perform_scanning_script(grids, seeds, randomised,
                                  script, threads,
                                  logging ? &std::cout : nullptr);
    if (X.empty()) return 0;
    const auto size = X.size();
    assert(size == FX.size());
    const index_t N = X[0].size();
    const index_t M = FX[0].size();
    const index_t final_index = determine_sorting_index(sorting_index,N,M);
    outputs << "# " << N << " " << M << " " << final_index << " "
            << size << "\n# " << seeds.size() << " : ";
    Environment::out_line(outputs, seeds);
    outputs << std::endl;
    auto matrix = Algorithms::append2d_ranges(X, FX);
    if (final_index != 0)
      std::ranges::stable_sort(matrix, [final_index]
        (const auto& x, const auto& y){return x[final_index] < y[final_index];});
    FP::fullprec_float80(outputs);
    Environment::print2dformat(outputs, matrix, 2, header(N, M));
  }

}
