// Oliver Kullmann, 1.7.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For input-QCNF F, analyse F


EXAMPLES:

*/

#include <iostream>
#include <fstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "QDimacsSyntax.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.9",
        "12.7.2023",
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
    syntax_error();
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
    syntax_error();
  }
  if (first_nonc == num_lines) {
    if (level >= 1)
      std::cout << "\nall lines are comments\n";
    syntax_error();
  }
  assert(first_nonc < num_lines);
  if (level >= 2)
    std::cout << "num-comments " << first_nonc << "\n";

  const auto [dp, dp_error] = analyse_parline(F[first_nonc]);
  if (dp_error) {
    if (level >= 1)
      std::cout << "\ndp-line " << first_nonc << ":\n  \""
                << F[first_nonc] << "\"\n";
    syntax_error();
  }
  if (level >= 2)
    std::cout << "pars " << dp.n << " " << dp.c << "\n";
  if (dp.n == 0) {
    if (level >= 1)
      std::cout << "\nn=0\n";
    syntax_error();
  }
  if (dp.c == 0) {
    if (level >= 1)
      std::cout << "\nc=0\n";
    syntax_error();
  }

  const count_t first_ae = first_nonc + 1;
  if (first_ae == num_lines or not begins_ae(F[first_ae])) {
    if (level >= 1)
      std::cout << "\nno a-/e-line at line " << first_ae << "\n";
    syntax_error();
  }
  const bool first_a = F[first_ae][0] == 'a';
  if (level >= 2)
    std::cout << "first_a " << std::boolalpha << first_a << "\n";
  const count_t end_ae = first_nonae(F, first_ae+1);
  assert(end_ae > first_ae);
  if (level >= 2)
    std::cout << "num_ae " << end_ae - first_ae << "\n";
  assert(end_ae <= num_lines);
  if (num_lines - end_ae != dp.c) {
    if (level >= 1)
      std::cout << "\nremaining-lines " << num_lines - end_ae << " != c = "
                << dp.c << "\n";
    syntax_error();
  }

  {char firstc = 0;
   for (count_t i = first_ae; i < end_ae; ++i) {
     const std::string& line = F[i];
     if (firstc == line[0]) {
       if (level >= 1)
         std::cout << "\na-e-line " << i - first_ae <<
           " same quantifier as previous line:\n"
           "  \"" << line << "\"\n";
       syntax_error();
     }
     firstc = line[0];
     if (not line.ends_with(" 0")) {
       if (level >= 1)
         std::cout << "\na-e-line " << i - first_ae <<
           " not finishing with \" 0\":\n"
           "  \"" << line << "\"\n";
       syntax_error();
     }
   }
  }
  const auto [vars, wrongaeline] = readae(F, dp.n, first_ae, end_ae, level);
  if (wrongaeline != end_ae) {
    if (level >= 1)
      std::cout << "problem with a-e-line " << wrongaeline - first_ae << "\n";
    syntax_error();
  }

}
