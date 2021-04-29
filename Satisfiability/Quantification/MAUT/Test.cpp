// Oliver Kullmann, 29.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <utility>
#include <vector>
#include <array>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "VarLit.hpp"
#include "ClauseSets.hpp"
#include "Parsing.hpp"
#include "RPL_trees.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.0",
        "29.4.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Quantification/MAUT/Test.cpp",
        "GPL v3"};

  using namespace MAUT;

  template <class X>
  constexpr bool eq(const X& x, const X& y) noexcept {
    return x == y;
  }
  template <class X, class Y>
  constexpr bool eq(const X& x, const Y& y) noexcept {
    return x == X(y);
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(valid(CL{}));
   assert(valid(CL{1}));
   assert(valid(CL{-1,2,-3,5}));
   assert(not valid(CL{0}));
   assert(not valid(CL{max_var+1}));
   assert(valid(CL{max_var}));
   assert(not valid(CL{2,1}));
   assert(valid(CL{1,2}));
   assert(not valid(CL{2,3,2}));
   assert(not valid(CL{-2,2}));
  }

  {const CLS F;
   assert(valid(F));
   assert(eq(count(F), {}));
   Occurrences O(0);
   assert(O.n == 0);
   assert(O[0].empty());
   O.enter(F);
   assert(O[0].empty());
   bool caught = false;
   try { O.at(1); }
   catch(const std::out_of_range& e) {
     assert(eq(std::string(e.what()), "Occurrences::trc, x=1, n=0"));
     caught = true;
   }
   assert(caught);
   caught = false;
   try { O.at(-1); }
   catch(const std::out_of_range& e) {
     assert(eq(std::string(e.what()), "Occurrences::trc, x=-1, n=0"));
     caught = true;
   }
   assert(caught);
  }

  {const CLS F{{},{},{1,2},{-2,3},{-5},{},{1,-2,-3}};
   assert((valid(F)));
   assert(eq(count(F), {5,4,2,7,8}));
   Occurrences O(6);
   assert(O.n == 6);
   assert(O.at(6).empty());
   assert(O.at(-6).empty());
   O.enter(F);
   assert(O.at(0).empty());
   assert(O.at(4).empty());
   assert(O.at(-4).empty());
   assert(O.at(6).empty());
   assert(O.at(-6).empty());
   const LitOcc Fp = &F[0];
   assert(eq(O.at(1), {Fp+2, Fp+6}));
   assert(eq(O.at(-1), {}));
   assert(eq(O.at(2), {Fp+2}));
   assert(eq(O.at(-2), {Fp+3,Fp+6}));
   assert(eq(O[2], {Fp+2}));
   assert(eq(O[-2], {Fp+3,Fp+6}));
   assert(eq(O.at(3), {Fp+3}));
   assert(eq(O.at(-3), {Fp+6}));
   assert(eq(O.at(5), {}));
   assert(eq(O.at(-5), {Fp+4}));
  }

  {ClauseSet F({6,8});
   assert(eq(F.dp, {6,8}));
   F.F.assign({{},{},{1,2},{-2,3},{-5},{},{1,-2,-3}});
   const CLS F0{{},{},{1,2},{-2,3},{-5},{},{1,-2,-3}};
   assert(F.F == F0);
   F.update();
   assert(eq(F.s, {5,4,2,7,8}));
   const LitOcc Fp = &F.F[0];
   assert(eq(F.occ.at(2), {Fp+2}));
   assert(eq(F.occ.at(-2), {Fp+3,Fp+6}));
   const ClauseSet M(std::move(F));
   assert(eq(M.dp, {6,8}));
   assert(M.F == F0);
   assert(eq(M.occ.at(2), {Fp+2}));
   assert(eq(M.occ.at(-2), {Fp+3,Fp+6}));
  }

  {bool caught = false;
   try { throw Syntax("xyz"); }
   catch (const Syntax& e) {
     assert(eq(std::string(e.what()), syntax_prefix + "xyz"));
     caught = true;
   }
   assert(caught);
   caught = false;
   try { throw Number("abc"); }
   catch (const Number& e) {
     assert(eq(std::string(e.what()), number_prefix + "abc"));
     caught = true;
   }
   assert(caught);
  }

  {assert(comment_rx.mark_count() == 1);
   assert(comment("c ").empty());
   bool caught = false;
   try { comment(""); }
   catch (const Syntax& e) {
     assert(eq(std::string(e.what()), syntax_prefix + "comment="));
     caught = true;
   }
   assert(caught);
   caught = false;
   try { comment("c"); }
   catch (const Syntax& e) {
     assert(eq(std::string(e.what()), syntax_prefix + "comment=c"));
     caught = true;
   }
   assert(caught);
   assert(eq(comment("c pqr"), "pqr"));
   caught = false;
   try { comment("c pqr\n"); }
    catch (const Syntax& e) {
     assert(eq(std::string(e.what()), syntax_prefix + "comment=c pqr\n"));
     caught = true;
   }
   assert(caught);
  }

  {const std::basic_regex size(size_rx0);
   assert(not std::regex_match("-1", size));
   assert(std::regex_match("1", size));
   assert(not std::regex_match("-0", size));
   assert(std::regex_match("0", size));
   assert(not std::regex_match("00", size));
   assert(not std::regex_match("01", size));
   assert(not std::regex_match("1 ", size));
   assert(not std::regex_match("-10", size));
   assert(std::regex_match("112", size));
  }

  {assert(pline_rx.mark_count() == 2);
   assert(eq(pline("p cnf 0 0"), {0,0}));
   assert(eq(pline("p cnf 0 5"), {0,5}));
   assert(eq(pline("p cnf 2 0"), {2,0}));
   assert(eq(pline("p cnf 123 456"), {123,456}));
   assert(eq(pline("p cnf 9223372036854775806 18446744073709551615"),
             {9223372036854775806UL, 18446744073709551615UL}));
   {const std::vector<const char*> syntax{"", "p cnf -0 0", "p cnf  0 0",
        "p  cnf 0 0", "p cnf 01 0", "p cnf 0 0 ", "p cnf x 0",
        "p cnf 0 0x"};
    for (const std::string s : syntax) {
      bool caught = false;
      try { pline(s); }
      catch (const Syntax& e) {
        assert(eq(syntax_prefix + "pline=" + s, e.what()));
        caught = true;
      }
      assert(caught);
    }
   }
   {const std::vector<std::pair<std::array<const char*,2>, unsigned>> number
     {{{"9223372036854775807","0"}, 0},
      {{"0", "18446744073709551616"}, 1},
      {{"99999999999999999999999999999","999999999999999999999999999999"}, 0}};
    for (const auto s : number) {
      const std::string es = std::string("p cnf ") +
        s.first[0] + " " + s.first[1];
      bool caught = false;
      try { pline(es); }
      catch (const Number& e) {
        if (s.second == 0)
          assert(eq(number_prefix + "n=" + s.first[0], e.what()));
        else
          assert(eq(number_prefix + "c=" + s.first[1], e.what()));
        caught = true;
      }
      assert(caught);
    }
   }
  }

  {const std::basic_regex lit(literal_rx0);
   assert(std::regex_match("-1", lit));
   assert(std::regex_match("1", lit));
   assert(not std::regex_match("-0", lit));
   assert(not std::regex_match("0", lit));
   assert(not std::regex_match("-1 ", lit));
   assert(not std::regex_match("1 ", lit));
   assert(std::regex_match("-10", lit));
   assert(std::regex_match("112", lit));
  }

  {assert((clause("0") == CL{}));
   assert((clause("1 0") == CL{1}));
   assert((clause("-2 3 -1 10 -4 0") == CL{-1,-2,3,-4,10}));
   assert((clause("-2 3 1 -4 0") == CL{1,-2,3,-4}));
   {const std::vector<const char*> syntax{"", "1", "1  0",
        "0 0", "-0 0", "0 ", "1  2 0", "1x 0"};
    for (const std::string s : syntax) {
      bool caught = false;
      try { clause(s); }
      catch (const Syntax& e) {
        assert(eq(syntax_prefix + "clause=" + s, e.what()));
        caught = true;
      }
      assert(caught);
    }
   }
   {const std::vector<const char*> number{"9223372036854775807",
        "-9223372036854775807", "99999999999999999999999999999"};
    for (const std::string s : number) {
      const std::string es = s + " 0";
      bool caught = false;
      try { clause(es); }
      catch (const Number& e) {
        assert(eq(number_prefix + "x=" + s, e.what()));
        caught = true;
      }
      assert(caught);
    }
   }
   {const std::vector<const char*> logic{"1 1 0", "1 -2 3 -2 0",
        "1 -1 0", "-1 2 3 1 0"};
    for (const std::string s : logic) {
      bool caught = false;
      try { clause(s); }
      catch (const Logic& e) {
        assert(eq(logic_prefix + "clause=" + s, e.what()));
        caught = true;
      }
      assert(caught);
    }
   }
  }

}
