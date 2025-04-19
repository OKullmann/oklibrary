// Oliver Kullmann, 29.3.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading a graph (undirected, in the general library-format),
  and outputting the maximum-independent-set problem in MaxSAT-form.

TODOS:

0. Enable also "MinVertexCover2MaxSAT":
  - The same clauses, but all element-wise complemented.
  - As an awk-filter appended to the output of this program:
awk '/^c/;!/^c/{printf "%s", $1; for (i=2; i<=NF; ++i) printf " %d", -$i; printf "\n"}'
  - But better having an optional argument for the output-type.
  - See TODOS in Graphs.hpp.

1. Once independent2MaxSAT(std::ostream, AdjVecUInt) is available,
   use this here (so that we don't need to store the graph (as clauses)).

*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "29.3.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/MaxIndependent2MaxSAT.cpp",
        "GPL v3"};

  using namespace Graphs;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  const std::string default_sep = "-";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << "\n\n"
    " reads a graph from standard input, and prints the MaxSAT-translation (post-2022) to standard output.\n"

;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 1) {
    std::cerr << error <<
      "No argument allowed, but " << argc-1 << " provided.\n";
    return 1;
  }

  std::cout << independent2MaxSAT(make_AdjVecUInt(std::cin, GT::und));
}
