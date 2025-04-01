// Oliver Kullmann, 31.3.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading a graph (undirected, in the general library-format),
  and outputting it in one of the AdjVecUInt-formats
*/

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.7",
        "31.3.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/GraphConversion.cpp",
        "GPL v3"};

  using namespace Graphs;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " graph-type format-option with-names\n\n"
    " graph-type     : " << Environment::WRP<GT>{} << "\n"
    " format-option  : " << Environment::WRP<GrFo>{} << "\n"
    " with-names     : \"false\" or anything else (default \"true\")\n\n"
    " reads a graph from standard input, and outputs it to standard output:\n\n"
    " - Arguments \"\" (the empty string) yield the default-values.\n"
    "  - Default-values for the options are the first possibilities given.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 4) {
    std::cerr << error <<
      "Exactly three arguments (graph-type, format-opt, with-names)"
      " needed, but " << argc-1 << " provided.\n";
    return 1;
  }

  const auto opt_type = Environment::read<GT>(argv[1]);
  if (not opt_type) {
    std::cerr << error <<
      "Faulty type-argument \"" << argv[1] << "\".\n";
    return 1;
  }
  const GT type = opt_type.value();

  const auto opt_format = Environment::read<GrFo>(argv[2]);
  if (not opt_format) {
    std::cerr << error <<
      "Faulty format-argument \"" << argv[2] << "\".\n";
    return 1;
  }
  const GrFo format = opt_format.value();

  const bool with_names = std::string(argv[3]) != "false";

  std::cout << make_AdjVecUInt(std::cin, type, format, with_names);
}
