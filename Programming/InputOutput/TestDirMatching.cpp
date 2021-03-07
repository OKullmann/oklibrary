// Oliver Kullmann, 3.7.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>
#include <cmath>

#include <ProgramOptions/Environment.hpp>

#include "DirMatching.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "7.3.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/InputOutput/TestDirMatching.cpp",
        "GPL v3"};

  using namespace DirMatching;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(make_absolute("", "") == "");
   assert(make_absolute("XYZ", "") == "XYZ");
   assert(make_absolute("DirMatching.hpp", "") == "DirMatching.hpp");
   assert(make_absolute("./DirMatching.hpp", "").ends_with("InputOutput/DirMatching.hpp"));
  }

  {assert(convert_dir("tests", "").string().ends_with("InputOutput/tests"));
  }

}
