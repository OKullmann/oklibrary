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


EXAMPLES:

A larger example, testing commutativity-modulo-subsumption in the
boolean case:

Bicliques> time BRG "3000*200,5" | Dimacs2NOBOCONF.awk | ./NBDPreduction 3,5,13,20,101 1 1 > OUT1.cnf
real	1m35.524s user	1m35.491s sys	0m0.032s
Bicliques> time BRG "3000*200,5" | Dimacs2NOBOCONF.awk | ./NBDPreduction 101,20,13,5,3 1 1 > OUT2.cnf
real	2m50.112s user	2m49.958s sys	0m0.092s
Bicliques> diff OUT1.cnf OUT2.cnf
1c1
< Cred-aeds 264904 2210 78613 101121
---
> Cred-aeds 325323 2030 105381 134952
Bicliques> head OUT1.cnf -n 4
Cred-aeds 264904 2210 78613 101121
n 201
c 85960
2:0 25:1 42:1 43:0 45:0 0
Bicliques> head OUT2.cnf -n 4
Cred-aeds 325323 2030 105381 134952
n 201
c 85960
2:0 25:1 42:1 43:0 45:0 0

Remark: Using 4000 clauses here (instead of 3000) in the input yields a large
increase in clauses created by DP-reduction.

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
    "0.0.9",
    "1.6.2025",
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
  if (clauseset) F.make_clauseset();
  else F.fully_standardise();

  const auto stats = DP_reduction(F, V, clauseset, subsumption);
  std::cout << GenClauseSets::comchar << "red-aeds ";
  Environment::out_line(std::cout, stats);
  std::cout << std::endl;

  std::cout << F;
}
