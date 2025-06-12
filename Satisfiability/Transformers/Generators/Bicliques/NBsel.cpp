// Oliver Kullmann, 10.6.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Selection of a ramdom sub-CNF of input in NOBOCONF format,
  according to the uniform or binomial model

  Similar to GRRG.cpp, two models are provided:
   - probability p (either of inclusion or of exclusion)
   - number k (either keeping or removing).
  And also the general framework for random generation is used (see below
  for some examples)

  Similar to TotalPermutation.cpp, a hash-value of the input CNF is appended
  to the seed-sequence.

EXAMPLES:

Bicliques> NBGen complete 2 2 2 | ./NBsel
C p=1/2
C seeds: 1609092523835210350 0 6 1749600226408680268 0 2 1 2 1 2
C additional-seeds: 2 8 1530578581647396732 13273198538908859975
n 2
c 5
0:0 1:0 0
0:1 1:0 0
0:0 1:1 0
0:0 1:1 0
0:1 1:1 0

Without arguments, the binomial model with p=1/2 is used (doesn't matter
here whether it's inclusion or exclusion).

Explanation of the basic ten seeds (first seed-line):
1. OKlibrary-timestamp                  1609092523835210350
2. code for "logic"                     0
3. code for this program                6
4. timestamp of this program            1749600226408680268
# these four initial seeds are always the same
5. 0 for binomial, 1 for uniform        0
6. number of mode-parameters            2
# seed 6 is always the same, and actually means
# 2-1 = 1 mode-parameters:
7. 0 for exclusion, 1 for inclusion     1
8. number of parameters                 2
# seed 8 is also always the same
9. numerator of p                       1
10. denominator of p                    2

The additional four seeds are derived from the input F:
1. Number of variables of F             2
2. Number of clauses of F               8
3. hash-value derived only from the     1530578581647396732
   sequence of clause-sizes
4. hash-value, taking as initial seed   13273198538908859975
   a hash value of the sequence of the
   first three values, and then combines
   this one after the other with the
   hashes of the literals of the clauses of F.

The two arguments explicitly given (with empty seed sequence):
Bicliques> NBGen complete 2 2 2 | ./NBsel_debug 1/2 ""

Now exlusion-probability 999/1000, and two user-defined seeds
(which are appended to the initial 10 seeds as above):
Bicliques> NBGen complete 10 2 1 | ./NBsel -999/1000 44,57
C p=999/1000
C seeds: 1609092523835210350 0 6 1749600226408680268 0 2 0 2 999 1000 44 57
C additional-seeds: 10 1024 4558594999487443195 2864920488709920045
n 10
c 1
0:0 1:1 2:0 3:1 4:1 5:0 6:1 7:0 8:0 9:1 0

Showing the "uniform" model, just taking 5 clauses, and using a timestamp
as key:
Bicliques> NBGen complete 20 2 2 | ./NBsel 5 t
C k=5
C seeds: 1609092523835210350 0 6 1749600226408680268 1 2 1 2 5 0 1749764479886584872
C additional-seeds: 20 2097152 4715844191747265585 12591386078888374306
n 20
c 5
0:1 1:0 2:0 3:1 4:1 5:0 6:0 7:0 8:1 9:1 10:0 11:0 12:0 13:0 14:0 15:0 16:0 17:0 18:0 19:0 0
0:0 1:0 2:1 3:0 4:1 5:1 6:1 7:1 8:0 9:0 10:0 11:0 12:1 13:0 14:0 15:0 16:0 17:0 18:0 19:0 0
0:1 1:0 2:0 3:1 4:1 5:1 6:1 7:1 8:1 9:0 10:0 11:1 12:1 13:1 14:0 15:1 16:0 17:0 18:0 19:0 0
0:1 1:0 2:0 3:1 4:0 5:0 6:1 7:1 8:1 9:1 10:0 11:1 12:0 13:1 14:1 15:1 16:0 17:1 18:0 19:0 0
0:0 1:0 2:1 3:1 4:1 5:1 6:0 7:0 8:0 9:0 10:0 11:0 12:1 13:1 14:1 15:0 16:1 17:1 18:1 19:0 0

We see now that k=5 takes the place of the numerator, while the denominator
for this model is always 0.
The timestamp is in nanoseconds.
In order to reproduce:
Bicliques> NBGen complete 20 2 2 | ./NBsel 5 1749764479886584872

Now removing clauses, and showing the use of a full seed-sequence, with
constants, timestamps, and true random values:
Bicliques> time NBGen complete 14 3 1 | ./NBsel -4782965 0,1,t,2,r,3,t
C k=4782965
C seeds: 1609092523835210350 0 6 1749600226408680268 1 2 0 2 4782965 0 0 1 1749764825654350082 2 18263586717512168848 3 1749764825654353599
C additional-seeds: 14 4782969 16969110916415458821 9529851727277684240
n 14
c 4
0:0 1:2 2:0 3:0 4:1 5:0 6:2 7:0 8:1 9:1 10:0 11:0 12:1 13:0 0
0:0 1:0 2:2 3:1 4:1 5:2 6:1 7:1 8:1 9:0 10:0 11:0 12:2 13:0 0
0:1 1:1 2:0 3:2 4:2 5:2 6:0 7:0 8:0 9:0 10:2 11:2 12:0 13:2 0
0:2 1:0 2:1 3:0 4:0 5:2 6:2 7:1 8:1 9:2 10:0 11:1 12:1 13:2 0
real	0m9.289s
user	0m13.934s
sys	0m0.922s

Remark: The order of the clauses (which here come from NBgen, and is
anti-lexicographic) is always maintained.

Some other uses:

Creating a number of random numbers resp. timestamps (the awk-program
selects the seed-line and prints the user-seeds):
Bicliques> echo | ./NBsel "" r,r,r,t,r | awk '/^C seeds:/{for (i=2+11; i<=NF; ++i) printf " %s", $i; printf "\n"}'
 6663750733307010797 14450907744850819822 9460199439678480495 1749766007234214523 15568442165790638732

The hash-value of the input (available as "hash(GenClauseSets:GClauseList)"
in Generators/Random/GenClauseSets.hpp):
Bicliques> time NBGen complete 17 3 1 | ./NBsel 0 "" | awk '/^C add/{print $6}'
4962778143753605888
real	0m53.232s
user	0m7.608s
sys	0m27.970s

Remark: This is a quick hash-value (not a cryptographic one).

*/

#include <iostream>
#include <vector>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>

#include <Transformers/Generators/Random/GenClauseSets.hpp>

#include "NBsel.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.0",
        "12.6.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/NBsel.cpp",
        "GPL v3"};

  using namespace NBsel;

  namespace RG = RandGen;
  namespace GCS = GenClauseSets;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " [[+-]k|p] [seeds]\n\n"
    " k|p        : number k of clauses or probability p = \"num/denom\"\n"
    " seeds      : "; RG::explanation_seeds(std::cout, 12);
    std::cout <<
    "\n\n"
    " selects k clauses resp. with probability p from standard input:\n\n"
    " - Arguments \"\" (the empty string) yield also the default-values.\n"
    " - The default for the first argument is p=1/2.\n"
    " - Numbers are unsigned 64-bit integers, however the input allows to\n"
    "   give them as 80-floating-point values (which will be rounded).\n"
    " - The optional \"-\" means \"exclusion\" (inclusion otherwise).\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const auto [k, p, uniform, inclusion] = read_sel_arg(
   argc>1 ? std::string(argv[1]) : std::string(""), error);
  const auto [seeds, basic_size] =
    all_seeds(k, p, uniform, inclusion,
              argc>2 ? std::string(argv[2]) : std::string(""));
  std::cout << GCS::comchar << " ";
  if (uniform) std::cout << "k=" << k;
  else std::cout << "p=" << p;
  std::cout << "\n" << GCS::comchar << " seeds: ";
  Environment::out_line(std::cout, seeds);
  std::cout << std::endl;

  GenClauseSets::GClauseList F;
  try { std::cin >> F; }
  catch (std::exception& e) {
    std::cerr << error << "SYNTAX ERROR with clause-list:\n"
              << e.what() << "\n";
    return 1;
  }
  assert(F.valid());

  const auto add_seeds = hash_seq(F);
  std::cout << GCS::comchar << " additional-seeds: ";
  Environment::out_line(std::cout, add_seeds);
  std::cout << std::endl;
  const auto full_seeds = [&seeds, &add_seeds]{auto res = seeds;
    res.insert(res.end(), add_seeds.begin(), add_seeds.end());
                                               return res;}();
  RandGen::RandGen_t g(full_seeds);

  if (uniform)
    if (inclusion) Algorithms::keep_krandom(F.F, k, g);
    else Algorithms::erase_krandom(F.F, k, g);
  else
    if (inclusion) Algorithms::keep_prandom(F.F, p, g);
    else Algorithms::erase_prandom(F.F, p, g);

  std::cout << F;
}
