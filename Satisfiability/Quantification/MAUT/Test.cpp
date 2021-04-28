// Oliver Kullmann, 29.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "VarLit.hpp"
#include "ClauseSets.hpp"
#include "Parsing.hpp"
#include "RPL_trees.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "29.4.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Quantification/MAUT/Test.cpp",
        "GPL v3"};

    using namespace MAUT;
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

}
