// Oliver Kullmann, 17.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "PQOptions.hpp"
#include "PQEncoding.hpp"
#include "Commandline.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
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
