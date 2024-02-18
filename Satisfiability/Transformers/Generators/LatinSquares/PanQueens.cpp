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

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.2",
        "17.2.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/PanQueens.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace PQOptions;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " N [line-type,constraint-form[,constraint-type]]*\n\n"
      " - N            : \";\"-separated list of \"a[,b][,c]\"-sequences\n"
      " - line-type    : " << Environment::WRPO<LT>{} << "\n" <<
      " - constraint-form : " << Environment::WRPO<CF>{} << "\n" <<
      " - constraint-type : " << Environment::WRPO<CT>{} << "\n\n" <<
      "Here\n"
      "  - \n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

}
