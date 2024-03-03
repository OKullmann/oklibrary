// Oliver Kullmann, 17.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Translating the pandiagonal-square condition (order N) into SAT
   - allowing additionally the (generalised) Sudoku-conditions (arbitrary N)
   - allowing AMO-constraints via "prime" or "sequential-counter" encoding
     (the later also in with the unique-extension-property, allowing
     counting)
   - the first row is standardised.

"Exactly-one" is used wherever possible, that is for the rows, columns,
(broken) diagonals and antidiagonals, while for the sudoku-constraints
for non-square-N only "at-most-one" is possible.

EXAMPLES:

1. There are 11 pandiagonal squares (with standardised first row), using
sharpSAT for this (extracting only the solutions-count):
LatinSquares> Pandiagonal 11 "" | sharpSAT /dev/stdin | awk '/^# solutions/{getline; print $0}'
8

2. Four of them are also Sudokus:
LatinSquares> Pandiagonal "+11" "" | sharpSAT /dev/stdin | awk '/^# solutions/{getline; print $0}'
4

The above uses "prime" (same results with secouep, and in case of "11", that
is, plain Pandiagonal, also with seco (since we have uep if no all-zero
is possible for the literals of amo-seco), but here not for "+11").


The output-dimacs-cnf contains a leading comment-section with information
on the translation.
In the Sudoku-case we have some special information:
LatinSquares> Pandiagonal +11 "" | awk '/^c/; /^p/{exit 0}'
c Output_time 03.03.2024 12:08:13_+0000 1709467693914029521
c ** Program information **
c program_name                          "Pandiagonal"
c version                               "0.1.4"
c date                                  "3.3.2024"
c gid_id                                "44957f98c6b19e8e7ab97bacea50d4a6f3bd0e1f"
c author                                "Oliver Kullmann"
c url                                   "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/Pandiagonal.cpp"
c ** Machine information **
c machine_name                          "csltok2"
c bogomips                              5799.77
c ** Compilation information **
c compiler_version                      "g++ 10.3.0"
c compilation_date                      "Mar_3_2024 12:07:58"
c compilation_options                   "--std=c++20 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static -fno-math-errno -fno-trapping-math -fno-rounding-math -fno-signaling-nans -fno-unsafe-math-optimizations -fno-finite-math-only -fno-associative-math -fno-reciprocal-math -fsigned-zeros"
c NDEBUG                                1
c OPTIMIZE                              1
c ** Parameters **
c command-line                          "Pandiagonal" "+11" ""
c N                                     11
c Constraint_type                       prime-implicates(prime)
c box-constraint                        1
c   b,q,r                               3 3 2
c   main,sides,corner                   9*9:at-most-one(amo) 6*6:at-most-one(amo) 0*4:at-most-one(amo)

 - b is the box-dimension
 - q is N / b (integer division), that is, the number of boxes of dimension b
   is q*q)
 - r is the remainder, that is, the number of "overhanging" columns and rows
   (not covered by the square boxes)

The square boxes are the "main boxes", and we have 3*3=9 of them, each with 9
cells inside, and encoding used at-most-one (exactly-one not possible, since
we have only 9 cells, while N=11).
Then we have 6 "side boxes", 3 on the right and 3 on the bottom, each
with 2*3=6 cells, and we would have 1 "corner box" (bottom-right) with
2*2=4 cells, but since there the amo-condition is already achieved by
diagonal- and antidiagonal-conditions, we have "0" such conditions.

Remark:
The CNFs are not stored, but created on the fly; used together with piping as
above means that no further output is created, e.g. (now showing the p-line):
LatinSquares> Pandiagonal +10000 "" | awk '/^c/; /^p/{print $0; exit 0}'
...
c N                                     10000
c Constraint_type                       prime-implicates(prime)
c box-constraint                        1
c   b,q,r                               100 100 0
c   main,sides,corner                   10000*10000:exactly-one(eo) 0*0:at-most-one(amo) 0*0:at-most-one(amo)
p cnf 1000000000000 29997000600010000


The list of sizes for 1 <= N <= 25:

LatinSquares> for (( i=1; i <= 25; ++i )); do Pandiagonal $i prime | awk '/^p cnf/'; done
p cnf 1 6
p cnf 8 42
p cnf 27 183
p cnf 64 564
p cnf 125 1380
p cnf 216 2886
p cnf 343 5397
p cnf 512 9288
p cnf 729 14994
p cnf 1000 23010
p cnf 1331 33891
p cnf 1728 48252
p cnf 2197 66768
p cnf 2744 90174
p cnf 3375 119265
p cnf 4096 154896
p cnf 4913 197982
p cnf 5832 249498
p cnf 6859 310479
p cnf 8000 382020
p cnf 9261 465276
p cnf 10648 561462
p cnf 12167 671853
p cnf 13824 797784
p cnf 15625 940650

LatinSquares> for (( i=1; i <= 25; ++i )); do Pandiagonal $i seco | awk '/^p cnf/'; done
p cnf 1 6
p cnf 8 42
p cnf 27 183
p cnf 64 564
p cnf 250 1255
p cnf 396 2346
p cnf 833 3927
p cnf 1152 6088
p cnf 1944 8919
p cnf 2500 12510
p cnf 3751 16951
p cnf 4608 22332
p cnf 6422 28743
p cnf 7644 36274
p cnf 10125 45015
p cnf 11776 55056
p cnf 15028 66487
p cnf 17172 79398
p cnf 21299 93879
p cnf 24000 110020
p cnf 29106 127911
p cnf 32428 147642
p cnf 38617 169303
p cnf 42624 192984
p cnf 50000 218775

LatinSquares> for (( i=1; i <= 25; ++i )); do Pandiagonal $i secouep | awk '/^p cnf/'; done
p cnf 1 6
p cnf 8 42
p cnf 27 183
p cnf 64 564
p cnf 250 1380
p cnf 396 2526
p cnf 833 4417
p cnf 1152 6728
p cnf 1944 10134
p cnf 2500 14010
p cnf 3751 19371
p cnf 4608 25212
p cnf 6422 32968
p cnf 7644 41174
p cnf 10125 51765
p cnf 11776 62736
p cnf 15028 76602
p cnf 17172 90738
p cnf 21299 108319
p cnf 24000 126020
p cnf 29106 147756
p cnf 32428 169422
p cnf 38617 195753
p cnf 42624 221784
p cnf 50000 253150

LatinSquares> for (( i=1; i <= 25; ++i )); do Pandiagonal +$i prime | awk '/^p cnf/'; done
p cnf 1 6
p cnf 8 42
p cnf 27 183
p cnf 64 564
p cnf 125 1380
p cnf 216 2886
p cnf 343 5397
p cnf 512 9288
p cnf 729 17991
p cnf 1000 26250
p cnf 1331 38445
p cnf 1728 55164
p cnf 2197 74256
p cnf 2744 99918
p cnf 3375 132765
p cnf 4096 185872
p cnf 4913 230622
p cnf 5832 288090
p cnf 6859 357675
p cnf 8000 442020
p cnf 9261 528276
p cnf 10648 633622
p cnf 12167 756861
p cnf 13824 901464
p cnf 15625 1128775

LatinSquares> for (( i=1; i <= 25; ++i )); do Pandiagonal +$i seco | awk '/^p cnf/'; done
p cnf 1 6
p cnf 8 42
p cnf 27 183
p cnf 64 564
p cnf 250 1255
p cnf 396 2346
p cnf 833 3927
p cnf 1152 6088
p cnf 2187 10701
p cnf 2770 14400
p cnf 4114 19822
p cnf 5184 26364
p cnf 7046 33111
p cnf 8428 42322
p cnf 11250 52890
p cnf 13312 66064
p cnf 16660 77911
p cnf 19188 94086
p cnf 23788 111606
p cnf 27000 131020
p cnf 32256 149961
p cnf 36168 174702
p cnf 43056 200836
p cnf 47808 229272
p cnf 56875 262525

LatinSquares> for (( i=1; i <= 25; ++i )); do Pandiagonal +$i secouep | awk '/^p cnf/'; done
p cnf 1 6
p cnf 8 42
p cnf 27 183
p cnf 64 564
p cnf 250 1380
p cnf 396 2526
p cnf 833 4417
p cnf 1152 6728
p cnf 2187 12159
p cnf 2770 16170
p cnf 4114 22605
p cnf 5184 29820
p cnf 7046 37960
p cnf 8428 48006
p cnf 11250 60765
p cnf 13312 75280
p cnf 16660 89658
p cnf 19188 107442
p cnf 23788 128535
p cnf 27000 150020
p cnf 32256 172956
p cnf 36168 200222
p cnf 43056 231725
p cnf 47808 263256
p cnf 56875 303775

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "PQOptions.hpp"
#include "PQEncoding.hpp"
#include "Commandline.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.0",
        "3.3.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/Pandiagonal.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr int commandline_args = 2;

  using namespace Commandline;
  using namespace PQOptions;
  using namespace PQEncoding;

  const dim_t maxN = 49804;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " [+]N constraint-type\n\n"
      " - N               : unsigned integer\n"
      " - constraint-type : " << Environment::WRPO<CT>{} << "\n\n" <<
      "Here\n"
      "  - \"+\" adds the (generalised) Sudoku-conditions\n"
      "  - for options the first possibility is the default, "
        "triggered by the empty string.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed, but the real number is " << argc-1 << ".\n";
    return 1;
  }

  const auto [N, sudoku] = read_dim(argv[1], error);
  if (N > maxN) {
    std::cerr << error << "N=" << N << " > maxN=" << maxN << ".\n";
    return 1;
  }
  const auto ct0 = Environment::read<CT>(argv[2]);
  if (not ct0) {
    std::cerr << error << "The constraint-type could not be read from"
        " string \"" << argv[2] << "\".\n";
    return 1;
  }
  const CT ct = ct0.value();

  const PEncoding enc(N, ct, sudoku);

  std::cout << Environment::Wrap(proginfo, Environment::OP::dimacs);
  using Environment::DHW;
  using Environment::DWW;
  std::cout << DHW{"Parameters"}
            << DWW{"command-line"};
  Environment::args_output(std::cout, argc, argv);
  std::cout << "\n"
            << DWW{"N"} << N << "\n"
            << DWW{"Constraint_type"} << ct << "\n"
            << DWW{"box-constraint"} << sudoku << "\n";
  if (sudoku) {
    std::cout << DWW{"  b,q,r"} << enc.b << " " << enc.q << " "
              << enc.r << "\n"
              << DWW{"  main,sides,corner"};
    PEncoding::output(std::cout, enc.boxes);
    std::cout << std::endl;
  }

  pandiagonal(std::cout, enc, sudoku);
}
