// Oliver Kullmann, 3.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <type_traits>
#include <string_view>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "FloatingPoint.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "23.8.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/Test.cpp",
        "GPL v3"};

  using namespace FloatingPoint;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(isnan(stold("NaN")));
  }

  {bool thrown = false;
   try { to_float80("x"); }
   catch(const std::invalid_argument& e) {
     assert(e.what() == std::string_view("FloatingPoint::to_float80(string), failed"
        " for \"x\""));
     thrown = true;
   }
   assert(thrown);
  }
  {bool thrown = false;
   try { to_float80("1e5000"); }
   catch(const std::out_of_range& e) {
     assert(e.what() == std::string_view("FloatingPoint::to_float80(string),"
       " \"1e5000\""));
     thrown = true;
   }
   assert(thrown);
  }
  {bool thrown = false;
   try { to_float80("0x"); }
   catch(const std::domain_error& e) {
     assert(e.what() == std::string_view("FloatingPoint::to_float80(string), trailing:"
        " \"x\" in \"0x\""));
     thrown = true;
   }
   assert(thrown);
  }

  {assert(toUInt("1e18") == pow(10.0L,18.0L));
   assert(toUInt("-1e100") == 0);
   assert(toUInt("1.8446744073709551614e19") == P264m1 - 1);
   assert(toUInt("NaN") == 0);
   assert(toUInt("inf") == P264m1);
   assert(toUInt("-inf") == 0);
  }
  {assert(touint("1e9") == pow(10.0L,9.0L));
   assert(touint("-1e100") == 0);
   assert(touint("4.294967294e9") == P232m1 - 1);
   assert(touint("NaN") == 0);
   assert(touint("inf") == P232m1);
   assert(touint("-inf") == 0);
   static_assert(std::is_same_v<decltype(touint("")), uint_t>);
  }

}
