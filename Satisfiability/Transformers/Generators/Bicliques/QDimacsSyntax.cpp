// Oliver Kullmann, 1.7.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For input-QCNF F, analyse F, and return output "ERROR" when not in
  "strict q-dimacs syntax:
   - First >= 0 lines starting with "c ".
   - A line is always completed by "\n" (EOL).
   - A "strict natural numbers" is defined as follows:
     - only digits, at least one
     - no leading zero
     - >= 1
     - < 2^64.
   - Then one line "p cnf n c" (no additional spaces), where n, c are
     strict natural numbers, and n < 2^64-1.
   - Then at least one a-e-line:
    - starting with "a " or "e "
    - finishing with " 0"
    - inbetween strict natural numbers (at least one) <= n, separated
      by at least one space (which always means " ", no other space-symbols;
      leading spaces possible)
    - no repetitions inside a line
    - no common elements between different lines
    - for max_ae the maximum number listed we have max_ae = n.
  - Then exactly c lines containing the clauses:
    - ending with " 0"
    - otherwise only literals (at least one), separated by at least one space,
      (possibly leading spaces), which are strict natural numbers, possibly
      prefixed with "-"
    - the underlying variables must occur in the a-e-lines
    - no repeated or clashing literals
    - at least one existential literal.
  - Every universal variable must occur positively and negatively in the
    clauses.
  - The "global variable" are the outermost a-variables, if the first
    block is universal, otherwise there are no global variables.


BUGS:

EXAMPLES:



*/

#include <iostream>
#include <fstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/Statistics.hpp>

#include "QDimacsSyntax.hpp"
#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "15.7.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/QDimacsSyntax.cpp",
        "GPL v3"};

  using namespace QDimacsSyntax;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " filename verbosity\n\n"
    " filename       : " << "the input-QCNF\n"
    " verbosity      : " << "natural number >= 0:\n"
    "                  - 0 : only \"" << is_incorrect << "\" or nothing\n"
    "                  - 1 : error descriptions\n"
    "                  - 2 : statistics\n\n"
    " reads a qcnf from filename, and analyses its syntax.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3) {
    std::cerr << error <<
      "Exactly two arguments (filename, verbosity)"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const std::string filename = argv[1];
  std::ifstream input(filename);
  if (not input) {
    std::cerr << error << "Can not open input-file \"" << filename
              << "\" for reading.\n";
    return int(Error::input_file_error);
  }
  const level_t level = FloatingPoint::touint(argv[2]);

  const auto [F, final_eol] = Environment::get_lines_check(input);
  if (not final_eol) {
    if (level >= 1)
      std::cout << "\nno final eol\n";
    syntax_error(1);
  }
  const count_t num_lines = F.size();
  if (level >= 2) {
    std::cout << "num-lines " << num_lines << "\n";
    count_t sum = 0;
    for (const auto& L : F) sum += L.size();
    std::cout << "num-chars " << sum << "\n";
  }

  const auto [first_nonc, c_error] = analyse_comments(F);
  if (c_error) {
    if (level >= 1)
      std::cout << "\nc-line " << first_nonc << ":\n  \""
                << F[first_nonc] << "\"\n";
    syntax_error(2);
  }
  if (first_nonc == num_lines) {
    if (level >= 1)
      std::cout << "\nall lines are comments\n";
    syntax_error(3);
  }
  assert(first_nonc < num_lines);
  if (level >= 2)
    std::cout << "num-comments " << first_nonc << "\n";

  const auto [dp, dp_error] = analyse_parline(F[first_nonc]);
  if (dp_error) {
    if (level >= 1)
      std::cout << "\ndp-line " << first_nonc << ":\n  \""
                << F[first_nonc] << "\"\n";
    syntax_error(4);
  }
  if (level >= 2)
    std::cout << "pars " << dp.n << " " << dp.c << "\n";
  if (dp.n == 0) {
    if (level >= 1)
      std::cout << "\nn=0\n";
    syntax_error(5);
  }
  if (dp.c == 0) {
    if (level >= 1)
      std::cout << "\nc=0\n";
    syntax_error(6);
  }

  const count_t first_ae = first_nonc + 1;
  if (first_ae == num_lines or not begins_ae(F[first_ae])) {
    if (level >= 1)
      std::cout << "\nno a-/e-line at line " << first_ae << "\n";
    syntax_error(7);
  }
  const bool first_a = F[first_ae][0] == 'a';
  if (level >= 2)
    std::cout << "first_a " << std::boolalpha << first_a << "\n";
  const count_t end_ae = first_nonae(F, first_ae+1);
  assert(end_ae > first_ae);
  if (level >= 2)
    std::cout << "num-ae-blocks " << end_ae - first_ae << "\n";
  assert(end_ae <= num_lines);
  if (num_lines - end_ae != dp.c) {
    if (level >= 1)
      std::cout << "\nremaining-lines " << num_lines - end_ae << " != c = "
                << dp.c << "\n";
    syntax_error(8);
  }

  {char firstc = 0;
   for (count_t i = first_ae; i < end_ae; ++i) {
     const std::string& line = F[i];
     if (firstc == line[0]) {
       if (level >= 1)
         std::cout << "\na-e-line " << i - first_ae <<
           " same quantifier as previous line:\n"
           "  \"" << line << "\"\n";
       syntax_error(9);
     }
     firstc = line[0];
     if (not line.ends_with(" 0")) {
       if (level >= 1)
         std::cout << "\na-e-line " << i - first_ae <<
           " not finishing with \" 0\":\n"
           "  \"" << line << "\"\n";
       syntax_error(10);
     }
   }
  }
  const auto [vars, wrongaeline] = readae(F, dp.n, first_ae, end_ae, level);
  assert(not vars.empty());
  if (wrongaeline != end_ae) {
    if (level >= 1)
      std::cout << "problem with a-e-line " << wrongaeline - first_ae
                << " (line " << wrongaeline << ")\n";
    syntax_error(11);
  }
  const count_t max_ae = max_ae_index(vars);
  assert(max_ae <= dp.n);
  if (level >= 2)
    std::cout << "max_ae " << max_ae << "\n";
  if (max_ae != dp.n) {
    if (level >= 1)
      std::cout << "\nmax-ae=" << max_ae << " < n=" << dp.n << "\n";
    syntax_error(12);
  }
  const count_t num_ae = Algorithms::sum_sizes(vars);
  assert(num_ae <= max_ae);
  if (level >= 2)
    std::cout << "num_ae " << num_ae << "\n";
  if (level >= 2) {
    std::cout << "ae_blocks " << vars[0].size();
    for (count_t i = 1; i < vars.size(); ++i)
      std::cout << " " << vars[i].size();
    std::cout << "\n";
  }
  const count_t num_glob = first_a ? vars[0].size() : 0;
  if (level >= 2)
    std::cout << "num-glob " << num_glob << "\n";
  std::cout.flush();
  const std::vector<bool>
    univ_block = is_universal_block(vars, first_a),
    ae_var = is_ae_var(vars, dp.n),
    univ_var = is_universal_var(vars, univ_block, dp.n);

  using stats_t = GenStats::BasicStats<count_t, FloatingPoint::float80>;
  degvec_t posd(dp.n+1), negd(dp.n+1);
  {stats_t Scl;
   count_t additional_spaces = 0;
   for (count_t i = end_ae; i < num_lines; ++i) {
     const count_t L = analyse_clause(F[i], posd, negd, dp.n, level,
                                      ae_var, univ_var,
                                      additional_spaces);
     if (L == 0) {
       if (level >= 1)
         std::cout << "problem with clause " << i - end_ae
                   << " (line " << i << ")\n";
       syntax_error(13);
     }
     Scl += L;
   }
   if (level >= 2)
     std::cout << "add-spaces-clauses " << additional_spaces << "\n"
               << "clause-lengths " << Scl << "\n";
  }

  const auto [num_form_g, num_pure_g] =
    num_pure_global_vars(first_a, vars[0], posd, negd);
  if (level >= 2)
    std::cout << "formal-global-vars " << num_form_g << "\n"
              << "pure-global-vars " << num_pure_g << "\n";
  if (num_form_g + num_pure_g != 0) {
    if (level >= 1)
      std::cout << "\nformal-or-pure-global-vars " << num_form_g + num_pure_g
                << "\n";
    syntax_error(14);
  }

  if (level >= 2) {
    // statistics on degrees XXX
  }

}
