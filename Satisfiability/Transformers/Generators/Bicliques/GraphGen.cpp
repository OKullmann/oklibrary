// Oliver Kullmann, 25.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Generating the graphs as given in Generators.hpp


EXAMPLES:

Bicliques> ./GraphGen clique 3
# 3 3 1
# clique(3)
# bcc=2
1 2 3
2 1 3
3 1 2
Bicliques> ./GraphGen biclique 2 3
# 5 6 1
# biclique(2,3)
# bcc=1
l1 r1 r2 r3
l2 r1 r2 r3
r1 l1 l2
r2 l1 l2
r3 l1 l2
Bicliques> ./GraphGen crown 3
# 6 6 1
# crown(3)
# bcc=3
l1 r2 r3
l2 r1 r3
l3 r1 r2
r1 l2 l3
r2 l1 l3
r3 l1 l2
Bicliques> ./GraphGen grid 2 3
# 6 7 1
# grid(2,3)
# bcc=2
1,1 1,2 2,1
1,2 1,1 1,3 2,2
1,3 1,2 2,3
2,1 1,1 2,2
2,2 1,2 2,1 2,3
2,3 1,3 2,2

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Generators.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.0",
        "8.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/GraphGen.cpp",
        "GPL v3"};

  using namespace Generators;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " graph-type ... parameters ...\n"
    " graph-type  : " << Environment::WRP<Types>{} << "\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  std::cout << create(argc, argv);

}
