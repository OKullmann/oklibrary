// Oliver Kullmann, 31.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading a NB-CNF in NOBOCONF format, and performing DP-reduction for
  a sequence of variables

TODOS:

1. In the multi-clause-set-case, performing doting (and final un-doting) when
   running subsumption.
    - The doting-variables perhaps are not registered with dom.
    - Perhaps doting-variables have variable indices say >= 2^48, and use
      the singular variable (so produce singular literals).


EXAMPLES:

A larger example, testing commutativity-modulo-subsumption in the
boolean case:

Bicliques> time BRG "3000*200,5" | Dimacs2NOBOCONF.awk | ./NBDPreduction 3,5,13,20,101 1 1 > OUT1.cnf
real	1m23.291s user	1m23.267s sys	0m0.029s
Bicliques> time BRG "3000*200,5" | Dimacs2NOBOCONF.awk | ./NBDPreduction 101,20,13,5,3 1 1 > OUT2.cnf
real	1m16.390s user	1m16.380s sys	0m0.016s

Differences only in the reduction-details:
Bicliques> diff OUT1.cnf OUT2.cnf
3c3
< Cred-aeds      236584 2019 61088 90517
---
> Cred-aeds      179525 1613 29734 65218

Bicliques> head OUT1.cnf -n 6
Cnc            201 3000
Cred-ccl       0 0 0
Cred-aeds      236584 2019 61088 90517
n 201
c 85960
2:0 25:1 42:1 43:0 45:0 0
Bicliques> head OUT2.cnf -n 6
Cnc            201 3000
Cred-ccl       0 0 0
Cred-aeds      179525 1613 29734 65218
n 201
c 85960
2:0 25:1 42:1 43:0 45:0 0

Remark: Using 4000 clauses here (instead of 3000) in the input yields a large
increase in clauses created by DP-reduction.

The above instance does not have duplications or subsumptions, but switching to
multi-clause-sets (still with subsumption-elimination) yields additional
clauses, which are resolvents subsumed (only) by original clauses:

Bicliques> time BRG "3000*200,5" | Dimacs2NOBOCONF.awk | ./NBDPreduction 3,5,13,20,101 0 1 > OUT3.cnf
real	1m38.970s
user	1m38.884s
sys	0m0.070s
Bicliques> head OUT3.cnf -n 6
Cnc            201 3000
Cred-cl        0 0
Cred-aeds      236584 2019 61088 90363
Cred-us        361683
n 201
c 86114

Bicliques> time BRG "3000*200,5" | Dimacs2NOBOCONF.awk | ./NBDPreduction 101,20,13,5,3 0 1 > OUT4.cnf
real	1m18.090s user	1m18.047s sys	0m0.048s
Bicliques> head OUT4.cnf -n 6
Cred-cl        0 0
Cred-aeds      179525 1613 29734 65064
Cred-us        361683
n 201
c 86114

Bicliques> diff OUT3.cnf OUT4.cnf
3c3
< Cred-aeds      236584 2019 61088 90363
---
> Cred-aeds      179525 1613 29734 65064

Bicliques> time cat OUT3.cnf | NBDPreduction "" 1 1 > OUT3s.cnf
real	0m31.996s user	0m31.973s sys	0m0.033s
Bicliques> diff OUT1.cnf OUT3s.cnf
< Cnc            201 3000
---
> Cnc            201 86114
3c3
< Cred-aeds      236584 2019 61088 90517
---
> Cred-aeds      0 0 0 154

Bicliques> time cat OUT4.cnf | NBDPreduction "" 1 1 > OUT4s.cnf
real	0m32.355s user	0m32.343s sys	0m0.024s
Bicliques> diff OUT2.cnf OUT4s.cnf
1c1
< Cnc            201 3000
---
> Cnc            201 86114
3c3
< Cred-aeds      179525 1613 29734 65218
---
> Cred-aeds      0 0 0 154


*/

#include <iostream>
#include <exception>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>

#include <Transformers/Generators/Random/GenClauseSets.hpp>

#include "GenResolution.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
    "0.2.0",
    "3.6.2025",
    __FILE__,
    "Oliver Kullmann",
    "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/NBDPreduction.cpp",
    "GPL v3"};

  using namespace GenResolution;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " V [sets] [subsumption]\n\n"
    " V              : list of variables, comma-separated\n"
    " [sets]         : default 0 (false)\n"
    " [subsumption]  : default 0 (false)\n\n"
    " reads a NB-CNF from standard-input, and prints it to"
    " standard output.\n\n"
;
    return true;
  }

  const unsigned initial_width = 15;
  bool to_bool(const std::string& s) noexcept { return s != "0"; }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc == 1) {
    std::cerr << error <<
      "At least one argument (V [sets] [subsumption])"
      " needed, but none provided.\n";
    return 1;
  }

  const varlist_t V = FloatingPoint::to_vec_unsigned<GenLit::var_t>(argv[1], ',');
  const bool clauseset = argc >= 3 ? to_bool(argv[2]) : false;
  const bool subsumption = argc >= 4 ? to_bool(argv[3]) : false;

  GenClauseSets::GClauseList F;
  try { std::cin >> F; }
  catch (std::exception& e) {
    std::cerr << error << "SYNTAX ERROR with clause-list:\n"
              << e.what() << "\n";
    return 1;
  }
  assert(F.valid());
  GenClauseSets::out_datacomment(std::cout, "nc", initial_width,
                                 varlist_t{F.n(), F.c()});
  std::cout << std::endl;

  if (clauseset) {
    const auto stats = F.make_clauseset();
    GenClauseSets::out_datacomment(std::cout, "red-ccl", initial_width, stats);
    std::cout << std::endl;
  }
  else {
    const auto stats = F.fully_standardise();
    GenClauseSets::out_datacomment(std::cout, "red-cl", initial_width, stats);
    std::cout << std::endl;
  }

  if (subsumption and not clauseset)
    F.spiking();

  const auto stats = DP_reduction(F, V, clauseset or subsumption, subsumption);
  GenClauseSets::out_datacomment(std::cout, "red-aeds", initial_width, stats);
  std::cout << std::endl;

  if (subsumption and not clauseset) {
    const auto count = F.unspiking();
    GenClauseSets::out_datacomment(std::cout, "red-us", initial_width, varlist_t{count});
    std::cout << std::endl;
  }

  std::cout << F;
}
