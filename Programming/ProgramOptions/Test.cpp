// Oliver Kullmann, 19.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <cassert>
#include <iostream>

#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo pi{
        "0.1.4",
        "20.6.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/ProgramOptions/Test.cpp",
        "GPL v3"};

  enum class Pol1 { val0=0, val1, val2 };
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Pol1> {
    static constexpr int size = int(Pol1::val2) + 1;
    static constexpr std::array<const char*, size> string
      {"0", "1", "two"};
  };
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, pi, argc, argv))
  return 0;

  using namespace Environment;

  {assert(replace("xxyxzxxyx", 'x', 'y') == "yyyyzyyyy");
   assert(basename("abc.def") == "abc");
   assert(auto_prg("abc.def") == "abc_debug");
   assert((split(",a,cf ,x\n,", ',') == tokens_t{"","a","cf ","x\n"}));
   assert(transform_spaces("\n \n ab  cd\n e  \n") == "ab cd e");
  }

  {ProgramInfo pi("AAA", "BBB", "CCC");
   assert(pi.vrs == "AAA");
   assert(pi.date == "BBB");
   assert(pi.prg == "CCC_debug");
   assert(pi.aut == "");
   assert(pi.url == "");
   assert(pi.lic == "");
  }
  {ProgramInfo pi("AAA", "BBB", "CCC.D", "x", "y", "z");
   assert(pi.vrs == "AAA");
   assert(pi.date == "BBB");
   assert(pi.prg == "CCC_debug");
   assert(pi.aut == "x");
   assert(pi.url == "y");
   assert(pi.lic == "z");
  }
  {ProgramInfo pi("AAA", "BBB", "CCC.D", "EEE", "FFF", "GGG", NP::given);
   assert(pi.vrs == "AAA");
   assert(pi.date == "BBB");
   assert(pi.prg == "CCC.D");
   assert(pi.aut == "EEE");
   assert(pi.url == "FFF");
   assert(pi.lic == "GGG");
  }

  {Index i;
   assert(i == 1);
   i++;
   assert(i == 2);
   i.deactivate();
  }

  {using std::tuple;
   using tp = tuple<OP,Pol1>;
   const auto tr = [](const std::string& s) noexcept {
     return translate<OP,Pol1>()(s,','); };
   assert((tr("") == tp{}));
   assert((tr("1,x") == tp{{},Pol1(1)}));
   assert((tr("x,0,1,0,two") == tp{{},Pol1(2)}));
   assert((tr("x,0,1,0,d,two,z,rh,1") == tp{OP::rh,Pol1(1)}));
  }

  {const auto t1 = CurrentTime::timestamp();
   const auto t2 = CurrentTime::timestamp();
   assert(t2 > t1);
  }

}
