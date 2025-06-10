// Oliver Kullmann, 9.6.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Generating the NB-CNFs as given in NBgenerators.hpp, in
  NOBOCONF format.


EXAMPLES:

> NBGen complete n D m
 - n : number of variables
 - D : uniform domain-size
 - m : clause-multiplicity
> NBGen complete 2 2 2
n 2
c 8
0:0 1:0 0
0:0 1:0 0
0:1 1:0 0
0:1 1:0 0
0:0 1:1 0
0:0 1:1 0
0:1 1:1 0
0:1 1:1 0


*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "NBgenerators.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.9",
        "9.6.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/NBGen.cpp",
        "GPL v3"};

  using namespace NBgenerators;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " NBCNF-type ... parameters ...\n"
    " NBCNF-type  : " << Environment::WRP<Types>{} << "\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  try { std::cout << create(argc, argv); }
  catch (std::exception& e) {
    std::cerr << error << e.what() << "\n";
    return 1;
  }

}
