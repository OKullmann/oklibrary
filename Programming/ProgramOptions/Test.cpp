// Oliver Kullmann, 19.3.2019 (Swansea)
/* Copyright 2019, 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <cassert>
#include <iostream>

#include "Strings.hpp"
#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo pi{
        "0.2.2",
        "21.2.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/ProgramOptions/Test.cpp",
        "GPL v3"};

  enum class Pol1 { val0=0, val1, val2 };
  template <class T>
  bool eqp(const T& x, const T& y) {
    return x == y;
  }
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
  }
  {assert(basename("abc.def") == "abc");
  }
  {assert(auto_prg("abc.def") == "abc_debug");
  }

  {assert(split("", ',') == tokens_t{}),
   assert((split("a,b,c", ',') == tokens_t{"a","b","c"}));
   assert((split(",a,cf ,x\n,", ',') == tokens_t{"","a","cf ","x\n"}));
   assert((split("   x a ", ' ') == tokens_t{"","","","x","a"}));
  }
  {std::stringstream s("a,b,c");
   assert((split(s, ',') == tokens_t{"a","b","c"}));
   s.clear(); s.str(",a,cf ,x\n,");
   assert((split(s, ',') == tokens_t{"","a","cf ","x\n"}));
   s.clear(); s.str("\n\na b\ncc\nd");
   assert((split(s, '\n') == tokens_t{"", "", "a b", "cc", "d"}));
   s.clear(); s.str("");
   assert(split(s, ',') == tokens_t{});
  }
  {std::stringstream s("a,b,c,");
   char c = 0;
   assert((split(s, ',', c) == tokens_t{"a","b","c"}));
   assert(c == ',');
   s.clear(); s.str("a,b,c"); c = 0;
   assert((split(s, ',', c) == tokens_t{"a","b","c"}));
   assert(c == 'c');
   s.clear(); s.str(""); c = 0;
   assert((split(s, '\n', c) == tokens_t{}));
   assert(c == 0);
   s.clear(); s.str("\n"); c = 0;
   assert((split(s, '\n', c) == tokens_t{{}}));
   assert(c == '\n');
   s.clear(); s.str("\n\na b\ncc\nd"); c = 0;
   assert((split(s, '\n', c) == tokens_t{"", "", "a b", "cc", "d"}));
   assert(c == 'd');
   s.clear(); s.str("\n\na b\ncc\n"); c = 0;
   assert((split(s, '\n', c) == tokens_t{"", "", "a b", "cc"}));
   assert(c == '\n');
   s.clear(); s.str("\n\na b\ncc"); c = 0;
   assert((split(s, '\n', c) == tokens_t{"", "", "a b", "cc"}));
   assert(c == 'c');
  }

  {assert(eqp(split2("", ',',','), {}));
   assert(eqp(split2("a,b,c\nd,e\nf", '\n',','), {{"a","b","c"},{"d","e"},{"f"}}));
  }

  {assert(onlyspaces(""));
   assert(onlyspaces("\n\t \n\t  "));
   assert(not onlyspaces("x"));
  }

  {std::string s = " \n a\n\n  \t b\t\t \n";
   remove_spaces(s);
   assert(s == "ab");
   s.clear();
   remove_spaces(s);
   assert(s.empty());
  }
  {assert(transform_spaces("") == "");
   assert(transform_spaces("\n \n \t ab \t cd\n e  \n \t f\tg  \n \t") == "ab cd e f g");
  }
  {assert(remove_trailing_spaces("abc  \n  ") == "abc");
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

  {const auto t1 = CurrentTime::timestamp_uint();
   const auto t2 = CurrentTime::timestamp_uint();
   assert(t2 > t1);
  }

}
