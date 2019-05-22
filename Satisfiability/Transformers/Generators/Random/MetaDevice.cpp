// Oliver Kullmann, 15.5.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Statistics for class bernoulli_random_device

Examples (annotations on following lines):

Version information:

Profiling version:

TODOS:

1. Add parallelisation.
    - What are we doing then with the output?
    - Just no output? Or to different files, for each thread one?
    - Seems easiest to have no output, and hoping that the run will finish
      soon enough.

2. Add the option to use the precise Bernoulli-distribution instead
   of the approximation (for the analysis of frequencies).
    - One gets always very low p-values ksfreq and ksruns for low N-values
      -- does this come from the approximated calculation of the primary
      p-values for frequencies and runs?

For example (on cs-wsok):
ExpMD_1558155799820561145> time ./MetaDevice 1e5 1e3 1e4 > data
real    1127m27.438s
user    58m20.733s
sys     1068m9.666s
ExpMD_1558155799820561145> cat data
# Timestamp: 18.05.2019 06:04:21_+0100 1558155861050217151
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaDevice.cpp
# program name:       MetaDevice
#  version:           0.1.0
#  last change:       15.5.2019
#  git-id:            e37f77bbef6be3bd2fa98a910f3f0097bc3f7462
# machine name:       cs-wsok
#  bogomips:          5986.74
# compiler version:   g++ 8.3.0
#  compilation date:  May_18_2019 06:00:51
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only
# N = 100000, M = 1000, T = 10000
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
-4.7921736023859295983e-17 3 14 0.1354261473739373761 -4.7921736023859295983e-17 3 13 0.20834867055478272597 4.9618905220487352376e-05 9.9999500501463112201e-06

These very low ks-p-values always seem to arise from higher T-values.
The values themselves can't be correct, since negative, and their
computation should be improved. But let's assume that they are indeed
very low, so really unlikely.


-- Interlude on MetaBernoulli

For comparison with MetaBernoulli (now on csverify):
$ time ./MetaBernoulli 1e5 1e3 1e4 1/2
# Timestamp: 19.05.2019 02:37:27_+0100 1558229847813492413
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp
# program name:       MetaBernoulli
#  version:           0.1.5
#  last change:       14.5.2019
#  git-id:            e37f77bbef6be3bd2fa98a910f3f0097bc3f7462
# machine name:       csverify
#  bogomips:          7183.75
# compiler version:   g++ 8.3.0
#  compilation date:  May_19_2019 02:35:28
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only
# N = 100000, M = 1000, T = 10000, p = 1/2
# Main seed: 1558229847813484551
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
-4.8355416892853497757e-17 4 2 0.26424111735051843217 -4.8355416892853497757e-17 4 1 0.63213895356707004367 9.9999500501610021595e-06 8.1970020328228144525e-05
real    62m11.525s
user    62m11.464s
sys     0m0.052s

Same negative ks-p-values.

Now MetaBernoulli with parallelisation, and N*=10:
$ time ./MetaBernoulli 1e6 1e3 1e4 1/2
# Timestamp: 19.05.2019 09:56:49_+0100 1558256209354454887
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp
# program name:       MetaBernoulli
#  version:           0.2.0
#  last change:       19.5.2019
#  git-id:            949085a4fd893a05c05fd7fffdc8a89268d0122c
# machine name:       csverify
#  bogomips:          7183.75
# compiler version:   g++ 8.3.0
#  compilation date:  May_19_2019 09:51:38
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only -pthread
# N = 1000000, M = 1000, T = 10000, p = 1/2
# Main seed: 1558256209354430958
# Number threads: 6
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
0.00085927906220670469606 3 13 0.2083486705547827572 0.015597047471298435667 3 7 0.8699847268232271054 9.9995005166165313801e-05 9.9999500501610021595e-06
real    99m40.524s
user    595m13.461s
sys     0m0.033s

ks-p-values much better.
Again N*=10:
ExpMB_1558271024320473493$ time ./MetaBernoulli 1e7 1e3 1e4 1/2 > data
real    991m57.943s
user    5929m11.531s
sys     0m0.012s
ExpMB_1558271024320473493$ cat data
# Timestamp: 19.05.2019 14:05:05_+0100 1558271105086496095
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp
# program name:       MetaBernoulli
#  version:           0.2.0
#  last change:       19.5.2019
#  git-id:            949085a4fd893a05c05fd7fffdc8a89268d0122c
# machine name:       csverify
#  bogomips:          7183.75
# compiler version:   g++ 8.3.0
#  compilation date:  May_19_2019 09:51:38
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only -pthread
# N = 10000000, M = 1000, T = 10000, p = 1/2
# Main seed: 1558271105086474635
# Number threads: 6
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
0.66770260455254797441 4 1 0.63213895356707004367 0.76084733446469696996 4 1 0.63213895356707004367 0.00015422348474724591454 9.9995005166165313801e-05


Smaller T-value:
ExpMB_1558297076297296536> time ./MetaBernoulli 1e6 1e3 1e3 1/2 > data
real    48m14.983s
user    96m22.099s
sys     0m0.131s
ExpMB_1558297076297296536> cat data
# Timestamp: 19.05.2019 21:21:57_+0100 1558297317911779944
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp
# program name:       MetaBernoulli
#  version:           0.2.0
#  last change:       19.5.2019
#  git-id:            3cbb9ec50199f8c1d1cca68a095588351dd2b7b3
# machine name:       cs-wsok
#  bogomips:          5986.74
# compiler version:   g++ 8.3.0
#  compilation date:  May_19_2019 18:32:57
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only -pthread
# N = 1000000, M = 1000, T = 1000, p = 1/2
# Main seed: 1558297317911758977
# Number threads: 2
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
0.61653607964489636639 2 6 0.933860488392748025 0.37407725298944459491 2 6 0.933860488392748025 8.1970020328224132977e-05 9.9995005166173716368e-05

Now increasing the M-value:
ExpMB_1558323558769739576> time ./MetaBernoulli 1e6 1e4 1e3 1/2 > data
real    485m24.782s
user    969m20.666s
sys     0m3.587s
ExpMB_1558323558769739576> cat data
# Timestamp: 20.05.2019 04:40:46_+0100 1558323646849297858
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp
# program name:       MetaBernoulli
#  version:           0.2.0
#  last change:       19.5.2019
#  git-id:            3cbb9ec50199f8c1d1cca68a095588351dd2b7b3
# machine name:       cs-wsok
#  bogomips:          5986.74
# compiler version:   g++ 8.3.0
#  compilation date:  May_19_2019 18:32:57
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only -pthread
# N = 1000000, M = 10000, T = 1000, p = 1/2
# Main seed: 1558323646849276963
# Number threads: 2
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
1.7437129417729058806e-08 3 2 0.2642410869698126999 4.9173360763962118553e-08 3 2 0.2642410869698126999 0.00099950021657514503352 0.00099950021657514503352

N*=10:
ExpMB_1558347856626817237$ time ./MetaBernoulli 1e7 1e4 1e3 1/2 > data
real    993m47.701s
user    5933m43.865s
sys     0m0.172s
ExpMB_1558347856626817237$ cat data
# Timestamp: 20.05.2019 11:25:41_+0100 1558347941774802275
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp
# program name:       MetaBernoulli
#  version:           0.2.0
#  last change:       19.5.2019
#  git-id:            3cbb9ec50199f8c1d1cca68a095588351dd2b7b3
# machine name:       csverify
#  bogomips:          7183.75
# compiler version:   g++ 8.3.0
#  compilation date:  May_19_2019 18:33:53
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only -pthread
# N = 10000000, M = 10000, T = 1000, p = 1/2
# Main seed: 1558347941774780197
# Number threads: 6
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
0.14593114526034559351 2 5 0.97131360000099527138 0.038291213351263658456 3 2 0.26424108696981270282 0.00059357185123889766093 0.00099950021657506095364

Repeating:
$ time ./MetaBernoulli 1e7 1e4 1e3 1/2 > data
real    993m46.044s
user    5932m48.576s
sys     0m0.116s
$ cat data
# Timestamp: 21.05.2019 06:49:45_+0100 1558417785496416305
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaBernoulli.cpp
# program name:       MetaBernoulli
#  version:           0.2.0
#  last change:       19.5.2019
#  git-id:            3cbb9ec50199f8c1d1cca68a095588351dd2b7b3
# machine name:       csverify
#  bogomips:          7183.75
# compiler version:   g++ 8.3.0
#  compilation date:  May_19_2019 18:33:53
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only -pthread
# N = 10000000, M = 10000, T = 1000, p = 1/2
# Main seed: 1558417785496394147
# Number threads: 6
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
0.77298255311655757649 2 11 0.41695919669890178918 0.090198652744780582352 2 9 0.66831273680083601925 0.00099950021657506095364 0.0015789454960306285197


-- End of interlude

Smaller T-values, higher N-value (so the normal approximation is more
precise):
ExpMD_1558229218716428942> time ./MetaDevice 1e6 1e3 1e3 > data
real    1117m26.610s
user    49m50.578s
sys     1066m45.500s
ExpMD_1558229218716428942> cat data
# Timestamp: 19.05.2019 02:28:03_+0100 1558229283205293388
# Producing program: https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaDevice.cpp
# program name:       MetaDevice
#  version:           0.1.0
#  last change:       15.5.2019
#  git-id:            e37f77bbef6be3bd2fa98a910f3f0097bc3f7462
# machine name:       cs-wsok
#  bogomips:          5986.74
# compiler version:   g++ 8.3.0
#  compilation date:  May_18_2019 06:00:51
#  used options:      --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-signed-zeros -fno-math-errno -fno-trapping-math -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math -fno-finite-math-only
# N = 1000000, M = 1000, T = 1000
 ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns
0.62786502387606789831 3 1 0.63230457522903596158 0.51339118104683734563 4 1 0.095167106441453733386 0.00058807010176299516491 0.00015422348474724184879


Summary for p=1/2:

N|M|T          pf-device     pf-bernoulli    pr-d        pr-b

1e5|1e3|1e4    -4e-17        -4e-17          -4e-17      -4e-17
1e6|1e3|1e4                  8.6e-4                      1.6e-2
1e7|1e3|1e4                  0.67                        0.76

1e6|1e3|1e3    0.63          0.62            0.51        0.37
1e6|1e4|1e3                  1.7e-8                      4.9e-8
1e7|1e4|1e3                  0.15                        0.038
                             0.77                        0.09


Apparently N >= M*T is needed for reasonable p-values:
 - It seems higher N means more precision.
 - Perhaps especially regarding outliers?
 - Then with the precise p-value computation at the bottom all these
   very low numbers should vanish (at least for the frequencies, since
   there we know the precise probability distribution; for the runs it
   needs to be determined).

Perhaps M is more "sensitive" than T ? At last regarding runs?


3. Also make the precise analysis of runs available (without the
   normal approximation).

*/

#include <iostream>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "15.5.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/MetaDevice.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 1e4L;
  constexpr gen_uint_t M_default = 1e3L;
  constexpr gen_uint_t T_default = 1e2L;


  // Outputting the parameters:

  using FloatingPoint::Wrap;
  using FloatingPoint::float80;

  void out_header(std::ostream& out) {
    out << " ksfreq lksfreq cksfreq pksfreq ksruns lksruns cksruns pksruns minpfreq minpruns\n";
  }

  void analyse(const gen_uint_t T, const gen_uint_t M, const gen_uint_t N, AnalysePVal_vt& Afreq, AnalysePVal_vt& Aruns) {
    for (gen_uint_t i = 0; i < T; ++i) {
      fvec_t Pfreq, Pruns;
      Pfreq.reserve(M); Pruns.reserve(M);

      for (gen_uint_t j = 0; j < M; ++j) {
        bernoulli_random_device b;
        if (b.entropy() == 0) {
          std::cerr << "[MetaDevice]: entropy = 0.\n";
          std::exit(1);
        }
        CountRuns count(b());
        for (gen_uint_t k = 1; k < N; ++k) count(b());
        const auto [cr, ct] = *count;
        Pfreq.push_back(monobit(ct, N));
        Pruns.push_back(runstest_gen(N, cr, {1,2}));
      }
      assert(Pfreq.size() == M and Pruns.size() == M);

      Afreq[i] = analyse_pvalues(Pfreq);
      Aruns[i] = analyse_pvalues(Pruns);
    }
  }

}

int main(const int argc0, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc0, argv)) return 0;
  const bool profiling = Environment::profiling(argc0, argv);
  const int argc = profiling ? 1 : argc0;

  Environment::Index index;
  const gen_uint_t N = (argc <= index) ? N_default : FloatingPoint::toUInt(argv[index++]);
  assert(N >= 1);
  const gen_uint_t M = (argc <= index) ? M_default : FloatingPoint::toUInt(argv[index++]);
  assert(M >= 1);
  const gen_uint_t T = (argc <= index) ? T_default : FloatingPoint::toUInt(argv[index++]);
  assert(T >= 1);
  index.deactivate();

  // Header info:
  std::cout << Environment::Wrap(proginfo, Environment::OP::rf);
  std::cout << "# N = " << N << ", M = " << M << ", T = " << T << "\n";
  out_header(std::cout);

  AnalysePVal_vt Afreq, Aruns;
  Afreq.resize(T); Aruns.resize(T);
  analyse(T, M, N, Afreq, Aruns);
  const auto anal_freq = analyse_pvalues(Afreq);
  const auto anal_runs = analyse_pvalues(Aruns);
  std::cout << anal_freq << " " << anal_runs;
  const float80 minpfreq = min_pvalue(Afreq);
  const float80 minpruns = min_pvalue(Aruns);
  std::cout << " " << Wrap(minpfreq) << " " << Wrap(minpruns) << "\n";
}
