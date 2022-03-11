// Oliver Kullmann, 19.3.2019 (Swansea)
/* Copyright 2019, 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <cassert>
#include <iostream>
#include <algorithm>
#include <set>
#include <sstream>
#include <vector>

#include "Strings.hpp"
#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo pi{
        "0.2.8",
        "11.3.2022",
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

  {assert(not CDstr::valid(""));
   assert(CDstr::valid("0"));
   assert(not CDstr::valid("00"));
   assert(not CDstr::valid("01"));
   assert(not CDstr::valid("0a"));
   assert(not CDstr::valid("00a"));
   assert(not CDstr::valid("01a"));
   assert(CDstr::valid("123"));
   assert(CDstr::valid("10230"));
   assert(not CDstr::valid("123a"));
   assert(CDstr::valid("abc"));
   assert(not CDstr::valid("abc0"));
   assert(not CDstr::valid("abc1"));
  }

  {CDstr::size_t pos;
   pos=0;
   assert(CDstr::extract("0", pos) == "0"); assert(pos == 1);
   assert(CDstr::extract("000", pos) == "0"); assert(pos == 3);
   pos=0; assert(CDstr::extract("00012", pos) == "12"); assert(pos == 5);
   pos=0; assert(CDstr::extract("000ab", pos) == "0"); assert(pos == 3);
   pos=0; assert(CDstr::extract("00012ab", pos) == "12"); assert(pos == 5);
   pos=2; assert(CDstr::extract("xy0", pos) == "0"); assert(pos == 3);
   pos=2; assert(CDstr::extract("xy000", pos) == "0"); assert(pos == 5);
   pos=2; assert(CDstr::extract("xy00012", pos) == "12"); assert(pos == 7);
   pos=2; assert(CDstr::extract("xy000ab", pos) == "0"); assert(pos == 5);
   pos=2; assert(CDstr::extract("xy00012ab", pos) == "12"); assert(pos == 7);

   pos=0; assert(CDstr::extract("1", pos) == "1"); assert(pos == 1);
   pos=0; assert(CDstr::extract("101", pos) == "101"); assert(pos == 3);
   pos=0; assert(CDstr::extract("1g", pos) == "1"); assert(pos == 1);
   pos=0; assert(CDstr::extract("101\n", pos) == "101"); assert(pos == 3);
   pos=2; assert(CDstr::extract("xy1", pos) == "1"); assert(pos == 3);
   pos=2; assert(CDstr::extract("xy101", pos) == "101"); assert(pos == 5);
   pos=2; assert(CDstr::extract("xy1g", pos) == "1"); assert(pos == 3);
   pos=2; assert(CDstr::extract("xy101\n", pos) == "101"); assert(pos == 5);

   pos=0; assert(CDstr::extract("a", pos) == "a"); assert(pos == 1);
   pos=0; assert(CDstr::extract("abc", pos) == "abc"); assert(pos == 3);
   pos=0; assert(CDstr::extract("a0", pos) == "a"); assert(pos == 1);
   pos=0; assert(CDstr::extract("a1", pos) == "a"); assert(pos == 1);
   pos=0; assert(CDstr::extract("abc0", pos) == "abc"); assert(pos == 3);
   pos=0; assert(CDstr::extract("abc123", pos) == "abc"); assert(pos == 3);
   pos=2; assert(CDstr::extract("12a", pos) == "a"); assert(pos == 3);
   pos=2; assert(CDstr::extract("12abc", pos) == "abc"); assert(pos == 5);
   pos=2; assert(CDstr::extract("12a0", pos) == "a"); assert(pos == 3);
   pos=2; assert(CDstr::extract("12a1", pos) == "a"); assert(pos == 3);
   pos=2; assert(CDstr::extract("12abc0", pos) == "abc"); assert(pos == 5);
   pos=2; assert(CDstr::extract("12abc123", pos) == "abc"); assert(pos == 5);
  }

  {CDstr::size_t pos = 0;
   CDstr x("0", pos); assert(pos == 1); assert(x() == "0");
   CDstr y("0xy", pos); assert(pos == 3); assert(y() == "xy");
   CDstr z("0xy00123", pos); assert(pos == 8); assert(z() == "123");
   CDstr a("0xy00123ab55", pos); assert(pos == 10); assert(a() == "ab");
   CDstr b("0xy00123ab55", pos); assert(pos == 12); assert(b() == "55");
   --pos; CDstr c("0xy00123ab0055", pos); assert(pos == 14); assert(b() == "55");
   assert(b == c);
   assert(x==x and x!=y and x<y and x<z and x<a and x<b);
   assert(y==y and y!=z and y>x and y>z and y>a and y>b);
   assert(z>x and z<y and z<a and z>b);
   assert(a>x and a<y and a>z and a>b);
   assert(b>x and b<y and b<z and b<a);
   std::vector<CDstr> v{x,y,z,a,b,x,y,z,a,b};
   std::ranges::sort(v);
   assert(eqp(v, {x,x,b,b,z,z,a,a,y,y}));
  }

  {const DecompStr E;
   assert(E().empty());
   const DecompStr E2("");
   assert(E2().empty());
   assert(E == E2);
   const DecompStr D("0");
   assert(D().size() == 1);
   assert(D()[0]() == "0");
   const DecompStr D2("56gj^&0012b?z120");
   assert(D2().size() == 5);
   assert(D2()[0]() == "56");
   assert(D2()[1]() == "gj^&");
   assert(D2()[2]() == "12");
   assert(D2()[3]() == "b?z");
   assert(D2()[4]() == "120");
   const DecompStr D3("560gj^&0012b?z120");
   const DecompStr D4("57");
   std::vector<DecompStr> v{D3,D4,E,E2,D,D2,E,E2,D,D2};
   std::ranges::sort(v);
   assert(eqp(v, {E,E,E2,E2,D,D,D2,D2,D4,D3}));
  }

  {typedef std::set<std::string, AlphaNum> aset;
   aset S{"0", "1", "00", "10", "010", "99"};
   assert(S.size() == 4);
   std::vector<std::string> v(S.begin(), S.end());
   assert(eqp(v, {"0", "1", "10", "99"}));
   S.insert({"name0", "namw20", "name15", "name3", "name10"});
   v.assign(S.begin(), S.end());
   assert(eqp(v, {"0", "1", "10", "99", "name0", "name3", "name10", "name15", "namw20"}));
   S.insert({"a5", "x11", "name15_x", ""});
   v.assign(S.begin(), S.end());
   assert(eqp(v, {"", "0", "1", "10", "99", "a5", "name0", "name3", "name10", "name15", "name15_x", "namw20", "x11"}));
  }

  {const std::string t = "ab\n \t\n x y";
   std::istringstream ss(t);
   assert(get_content(ss) == t);
   assert(ss.str() == t);
   ss.str(t);
   assert(eqp(get_lines(ss), {"ab", " \t", " x y"}));
   ss.str("");
   assert(get_content(ss) == "");
   ss.str("");
   assert(get_lines(ss).empty());
  }

  {std::string s;
   cutoff(s, '#');
   assert(s.empty());
   s = "a";
   cutoff(s, 'a');
   assert(s.empty());
   s = "abc \n def \n";
   cutoff(s, '\n');
   assert(s == "abc ");
  }

  {std::istringstream ss;
   assert(eqp(split_cutoff(ss, ' ', '#'), {}));
   const std::string t = "# 555 \nab\n \t\n x y # nm\n  # hg\n  \t\n\n 789";
   ss.str(t); ss.clear();
   assert(eqp(split_cutoff(ss, '\n', '#'), {"ab", " x y ", " 789"}));
  }

  {std::istringstream ss;
   assert(eqp(split2_cutoff(ss, '\n', '#'), {}));
   ss.clear();
   ss.str(" a bc 01&* c # hj\n \t\t # \n\n  \t\t x y89\n 89#");
   assert(eqp(split2_cutoff(ss, '\n', '#'),
              {{"a", "bc", "01&*", "c"},{"x", "y89"},{"89"}}));
  }

  {assert(valid(indstr_t{}));
   assert(valid(indstr_t{{""},{{"",0}}}));
   assert(not valid(indstr_t{{""},{{"",1}}}));
   assert(not valid(indstr_t{{""},{{"",0},{"x",0}}}));
   assert(valid(indstr_t{{"x",""},{{"x",0},{"",1}}}));
  }

  {index_vec_t v;
   assert(eqp(indexing_strings(v, false), {}));
   v.push_back("");
   assert(eqp(indexing_strings(v, false), {{""},{{"",0}}}));
   v.push_back("x");
   assert(eqp(indexing_strings(v, false), {{"","x"},{{"",0},{"x",1}}}));
   v.push_back("x");
   {bool found = false;
    try{ indexing_strings(v, false); }
    catch (std::runtime_error& e) { found = true; }
    assert(found);
   }
   assert(eqp(indexing_strings(v), {{"","x"},{{"",0},{"x",1}}}));
   v.push_back("a");
   assert(eqp(indexing_strings(v), {{"","x","a"},{{"",0},{"x",1},{"a",2}}}));
  }

}
