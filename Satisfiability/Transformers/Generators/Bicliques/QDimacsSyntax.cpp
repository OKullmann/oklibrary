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
  - The "global variable" are the outermost a-variables, if the first
    block is universal, otherwise there are no global variables.
  - Every global variable must occur positively and negatively in the
    clauses.

  The above is tolerance-level 0.
  Level 1 allows:
       - comment-check: just "c"
       - arbitrary spaces after n in the dp-line
       - further spaces after 0 in clause-lines
       - max_ae <= n
       - final empty lines (if there are such line then c can count them or
         not)
  Level 2 additionally allows:
       - repeated literals
       - clashing literals, that is tautological clauses (these are
         removed).


BUGS:

EXAMPLES:

For the QCNF

p cnf 5 3
a 1 4 0
e 2 5 0
a 3 0
5 5 4 1 -4 0
2 3 4 5 1 3 0
-1 -3  -4 -5 0

we get

Bicliques> echo -en "c kk\nc jk\nc \np cnf 5 3\na 1 4 0\ne 2 5 0\na 3  0\n5 5 4 1 -4 0\n2 3 4 5 1 3 0\n-1 -3  -4 -5 0\n\n" | ./QDimacsSyntax_debug /dev/stdin 2 2
filename "/dev/stdin"
verbosity-tolerance 2 2
num-lines 10
num-chars 78
trailing-empty-lines 1
num-comments 3
pars 5 3
first_a true
num-ae-blocks 3
remaining-lines 3
add-spaces-ae 1
max_ae 5
num_ae 5
ae_blocks 2 2 1
num-glob 2
add-spaces-clauses 1
repeated-literals 1
tautologies 1
clause-lengths 2 : 4 4.5 5; 0.707107
formal-global-vars 0
pure-global-vars 0



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
        "0.4.1",
        "31.7.2023",
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
         << " filename verbosity tolerance [" << grepstr << "]\n\n"
    " filename       : " << "the input-QCNF\n"
    " verbosity      : " << "natural number >= 0:\n"
    "                  - 0 : only \"" << is_incorrect << "\" or nothing\n"
    "                  - 1 : error descriptions\n"
    "                  - 2 : statistics\n"
    " tolerance      : " << "natural number >= 0:\n"
    "                  - 0 : zero tolerance\n"
    "                  - 1 : space and max-ae allowances\n"
    "                  - 2 : repeated and clashing literals and pure variables\n"
    " " << grepstr << "           : "
         << "replace original file with purified version:\n"
    "                  - currently only label \"" << grepstr << "\" allowed\n"
    "                  - tautological clauses are eliminated\n"
    "                  - pure/formal global variables are eliminated\n"
    "                  - if no global variables left, then the empty file is created\n\n"
    " reads a qcnf from filename, and analyses its syntax.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 4 and argc != 5) {
    std::cerr << error <<
      "Exactly three or four arguments (filename, verbosity, tolerance, ["
              << grepstr << "]) needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }
  const bool replacement = argc == 5;
  if (replacement and grepstr != argv[4]) {
    std::cerr << error <<
      "Currently the fourth argument can only be \"" << grepstr << "\".\n";
    return int(Error::replacement_file_error);
  }

  const std::string filename = argv[1];
  std::ifstream input(filename);
  if (not input) {
    std::cerr << error << "Can not open input-file \"" << filename
              << "\" for reading.\n";
    return int(Error::input_file_error);
  }
  const level_t verbosity = FloatingPoint::touint(argv[2]);
  const level_t tolerance = FloatingPoint::touint(argv[3]);
  if (verbosity >= 2) {
    std::cout << "filename \"" << filename << "\"\n";
    std::cout << "verbosity-tolerance-replacement " << verbosity << " "
              << tolerance << " " << std::boolalpha << replacement << "\n";
  }

  const auto [Feol, empty_trailing_lines] = get_lines(input);
  input.close();
  const auto& [F, final_eol] = Feol;
  const count_t num_lines = F.size();
  if (verbosity >= 2) {
    std::cout << "num-lines " << num_lines << "\n"
              << "num-chars " << Algorithms::sum_sizes(F) << "\n"
              << "trailing-empty-lines " << empty_trailing_lines << "\n";
  }
  if (not final_eol) {
    if (verbosity >= 1)
      std::cout << "\nno final eol\n";
    syntax_error(1);
  }
  if (tolerance == 0 and empty_trailing_lines != 0) {
    if (verbosity >= 1)
      std::cout << "\ntrailing empty lines\n";
    syntax_error(15);
  }

  const auto [first_nonc, c_error] = analyse_comments(F, tolerance);
  if (c_error) {
    if (verbosity >= 1)
      std::cout << "\nc-line " << first_nonc << ":\n  \""
                << F[first_nonc] << "\"\n";
    syntax_error(2);
  }
  if (first_nonc == num_lines) {
    if (verbosity >= 1)
      std::cout << "\nall lines are comments\n";
    syntax_error(3);
  }
  assert(first_nonc < num_lines);
  if (verbosity >= 2)
    std::cout << "num-comments " << first_nonc << "\n";

  const auto [dp, dp_error] = analyse_parline(F[first_nonc], tolerance);
  if (dp_error) {
    if (verbosity >= 1)
      std::cout << "\ndp-line " << first_nonc << ":\n  \""
                << F[first_nonc] << "\"\n";
    syntax_error(4);
  }
  if (verbosity >= 2)
    std::cout << "pars " << dp.n << " " << dp.c << "\n";
  if (dp.n == 0) {
    if (verbosity >= 1)
      std::cout << "\nn=0\n";
    syntax_error(5);
  }
  if (dp.c == 0) {
    if (verbosity >= 1)
      std::cout << "\nc=0\n";
    syntax_error(6);
  }

  const count_t first_ae0 = first_nonc + 1;
  if (first_ae0 == num_lines or not begins_ae(F[first_ae0])) {
    if (verbosity >= 1)
      std::cout << "\nno a-/e-line at line " << first_ae0 << "\n";
    syntax_error(7);
  }
  const count_t end_ae = first_nonae(F, first_ae0+1);
  assert(end_ae > first_ae0);
  if (verbosity >= 2)
    std::cout << "num-ae-blocks " << end_ae - first_ae0 << "\n";
  assert(end_ae <= num_lines);
  const count_t remaining_lines = num_lines - end_ae;
  assert(end_ae > first_ae0);
  if (verbosity >= 2)
    std::cout << "remaining-lines " << remaining_lines << "\n";
  if (remaining_lines != dp.c) {
    if (tolerance == 0 ) {
      if (verbosity >= 1)
        std::cout << "\nremaining-lines " << remaining_lines << " != c = "
                  << dp.c << "\n";
      syntax_error(8);
    }
    else if (remaining_lines + empty_trailing_lines != dp.c) {
      if (verbosity >= 1)
        std::cout << "\nremaining-lines " << remaining_lines << " != c = "
                  << dp.c << " and != c - empty-lines = "
                  << dp.c - empty_trailing_lines << "\n";
      syntax_error(8);
    }
  }

  {char firstc = 0;
   for (count_t i = first_ae0; i < end_ae; ++i) {
     const std::string& line = F[i];
     if (firstc == line[0]) {
       if (verbosity >= 1)
         std::cout << "\na-e-line " << i - first_ae0 <<
           " same quantifier as previous line:\n"
           "  \"" << line << "\"\n";
       syntax_error(9);
     }
     firstc = line[0];
     if (not line.ends_with(" 0")) {
       if (verbosity >= 1)
         std::cout << "\na-e-line " << i - first_ae0 <<
           " not finishing with \" 0\":\n"
           "  \"" << line << "\"\n";
       syntax_error(10);
     }
   }
  }
  const auto [vars, wrongaeline, inc] =
    readae(F, dp.n, first_ae0, end_ae, verbosity, tolerance);
  if (verbosity >= 2)
    std::cout << "initial-ae-lines " << inc << "\n";
  const count_t first_ae = first_ae0 + inc;
  assert(not vars.empty());
  const bool first_a = F[first_ae][0] == 'a';
  if (verbosity >= 2)
    std::cout << "first_a " << std::boolalpha << first_a << "\n";
  if (wrongaeline != end_ae) {
    if (verbosity >= 1)
      std::cout << "problem with a-e-line " << wrongaeline - first_ae
                << " (line " << wrongaeline << ")\n";
    syntax_error(11);
  }
  const count_t max_ae = max_ae_index(vars);
  assert(max_ae <= dp.n);
  if (verbosity >= 2)
    std::cout << "max_ae " << max_ae << "\n";
  if (tolerance == 0 and max_ae != dp.n) {
    if (verbosity >= 1)
      std::cout << "\nmax-ae=" << max_ae << " # n=" << dp.n << "\n";
    syntax_error(12);
  }
  const count_t num_ae = Algorithms::sum_sizes(vars);
  assert(num_ae <= max_ae);
  if (verbosity >= 2)
    std::cout << "num_ae " << num_ae << "\n";
  if (verbosity >= 2) {
    std::cout << "ae_blocks " << vars[0].size();
    for (count_t i = 1; i < vars.size(); ++i)
      std::cout << " " << vars[i].size();
    std::cout << "\n";
  }
  const count_t num_glob = first_a ? vars[0].size() : 0;
  if (verbosity >= 2)
    std::cout << "num-glob " << num_glob << "\n";
  std::cout.flush();
  const std::vector<bool>
    univ_block = is_universal_block(vars, first_a),
    ae_var = is_ae_var(vars, dp.n),
    univ_var = is_universal_var(vars, univ_block, dp.n);

  using stats_t = GenStats::BasicStats<count_t, FloatingPoint::float80>;
  degvec_t posd(dp.n+1), negd(dp.n+1);
  count_t tautologies = 0;
  DimacsTools::ClauseList rF; // empty if not replacement
  {stats_t Scl;
   count_t additional_spaces = 0, repetitions = 0;
   for (count_t i = end_ae; i < num_lines; ++i) {
     const auto [L, correct] = analyse_clause(F[i], posd, negd, dp.n,
                                              verbosity,
                                              ae_var, univ_var,
                                              additional_spaces, tolerance,
                                              repetitions,
                                              replacement, rF);
     if (not correct) {
       if (verbosity >= 1)
         std::cout << "problem with clause " << i - end_ae
                   << " (line " << i << ")\n";
       syntax_error(13);
     }
     if (L == 0) ++tautologies; else Scl += L;
   }
   assert(replacement or rF.empty());
   if (verbosity >= 2)
     std::cout << "add-spaces-clauses " << additional_spaces << "\n"
               << "repeated-literals " << repetitions << "\n"
               << "tautologies " << tautologies << "\n"
               << "clause-lengths " << Scl << "\n";
  }

  const auto [num_form_g, num_pure_g, rem_g] =
    num_pure_global_vars(first_a, vars[0], posd, negd);
  assert(rem_g.size() == num_form_g + num_pure_g);
  const count_t fin_num_glob = num_glob - num_form_g - num_pure_g;
  if (verbosity >= 2)
    std::cout << "formal-global-vars " << num_form_g << "\n"
              << "pure-global-vars " << num_pure_g << "\n"
              << "final-global-vars " << fin_num_glob << "\n";
  if (tolerance <= 2 and num_form_g + num_pure_g != 0) {
    if (verbosity >= 1)
      std::cout << "\nformal-or-pure-global-vars " << num_form_g + num_pure_g
                << "\n";
    syntax_error(14);
  }

  if (verbosity >= 2) {
    // statistics on degrees XXX
  }

  if (replacement) {
    if (filename == "/dev/stdin") {
      std::cerr << error << "Can not replace /dev/stdin.\n";
      return int(Error::replacement_output_error);
    }
    std::ofstream output(filename, std::ios::out | std::ios::trunc);
    if (not output) {
      std::cerr << error << "Can not open output-file \"" << filename
              << "\" for truncated writing.\n";
      return int(Error::replacement_output_error);
    }
    if (fin_num_glob == 0) {
      if (verbosity >= 2)
        std::cout << "emptied input-file\n";
      return 0;
    }
    const count_t c = rF.size();
    assert(c != 0);
    const count_t n = update_max_ae(max_ae, rem_g, ae_var);
    assert(n != 0);
    if (verbosity >= 2)
        std::cout << "new-pars " << n << " " << c << std::endl;

    output << "p cnf " << n << " " << c << "\n";
    assert(univ_block.size() >= 2);
    assert(univ_block.size() == vars.size());
    for (count_t i = 0; i < univ_block.size(); ++i) {
      if (univ_block[i]) output << "a "; else output << "e ";
      assert(not vars[i].empty());
      Environment::out_line(output, vars[i]);
      output << " 0\n";
    }
    for (const auto& C : rF) {
      for (const auto& x : C)
        if (not rem_g.contains(var(x).v)) output << x << " ";
      output << "0\n";
    }
  }

}
