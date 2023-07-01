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
        "0.0.3",
        "1.7.2023",
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

  const tokens_t F = Environment::get_lines(input);
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

}
