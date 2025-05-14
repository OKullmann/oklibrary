// Oliver Kullmann, 14.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Pseudo-random generation of graphs

*/



#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "RandomGraphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.4",
        "14.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/GRGG.cpp",
        "GPL v3"};

  using namespace RandomGraphs;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " graph-type par1 par2 seeds format-options\n\n"
    " prints a pseudo-random graph to standard output:\n\n"
    " - Arguments \"\" (the empty string) yield the default-values.\n"
    " - Default-values for the options are the first possibilities given.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

}
