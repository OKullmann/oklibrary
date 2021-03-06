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
#include <Transformers/Generators/Random/Numbers.hpp>

#include "VarLit.hpp"
#include "ClauseSets.hpp"
#include "Parsing.hpp"
#include "PartialAssignments.hpp"
#include "Backtracking.hpp"
#include "Counting.hpp"
#include "RPL_trees.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.1",
        "2.5.2021",
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

  template <class COUNT>
  void test_count() {
    assert(COUNT::min_prec >= 64);
    COUNT count;
    assert(eq(std::string(count), "0"));
    count.add(10);
    assert(eq(std::string(count), "1024"));
    count.add(0);
    assert(eq(std::string(count), "1025"));
    COUNT count2;
    count += count2;
    assert(eq(std::string(count), "1025"));
    count2.add(2);
    count += count2;
    assert(eq(std::string(count), "1029"));
    count.add(63);
    assert(eq(std::string(count), "9223372036854776837"));
    if (COUNT::min_prec >= 1001) {
      count.add(1000);
      assert(eq(std::string(count), "10715086071862673209484250490600018105614048117055336074437503883703510511249361224931983788156958581275946729175531468251871452856923140435984577574698574803934567774824230985421074605062371141877954182153046474983581941267398767559165543946077062914571196477686542167660429831652633610209242522846213"));
    }
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

  {assert(eq(full_cls(0), {{}}));
   assert(eq(full_cls(1), {{1},{-1}}));
   assert(eq(full_cls(2), {{1,2},{-1,2},{1,-2},{-1,-2}}));
   const VAR max_n = 5;
   ClauseSet F({max_n,RandGen::iexp2(max_n)});
   for (VAR v = 0; v <= max_n; ++v) {
     CLS F0 = full_cls(v);
     assert(valid(F0));
     const auto e2 = RandGen::iexp2(v);
     assert(F0.size() == e2);
     F.F = std::move(F0);
     assert(F.F.size() == e2);
     F.occ.clear();
     F.update();
     assert(valid(F));
     assert(eq(F.s, {v,v,0,e2,v*e2}));
     {typedef FirstOpen<Count64> FO;
      FO fo(F);
      fo.solve();
      assert(eq(fo.result.t, {2*e2-1, e2}));
      assert(eq(fo.result.ca, RandGen::iexp2(max_n)));
      assert(fo.result.cn == e2);
     }
     {typedef FirstOpen<Count80> FO;
      FO fo(F);
      fo.solve();
      assert(eq(fo.result.t, {2*e2-1, e2}));
      assert(eq(fo.result.ca, RandGen::iexp2(max_n)));
      assert(fo.result.cn == e2);
     }
     {typedef FirstOpen<Count_mpz> FO;
      FO fo(F);
      fo.solve();
      assert(eq(fo.result.t, {2*e2-1, e2}));
      assert(eq(fo.result.ca, RandGen::iexp2(max_n)));
      assert(fo.result.cn == e2);
     }
   }
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

  {Pass phi(10);
   assert(phi == Pass(10, CL{}));
   phi.set({{1,PA::t}, {7, PA::f}});
   assert(phi == Pass(10, CL{1,-7}));
  }

  {Pass pa(0);
   assert(pa.size() == 0);
   assert(pa[0] == PA::o);
   pa[0] = PA::t;
   assert(pa[0] == PA::t);
   assert(pa.size() == 1);
  }
  {Pass pa(1);
   assert(pa.size() == 0);
   assert(pa[1] == PA::o);
   pa[1] = PA::t;
   assert(pa.size() == 1);
   assert(pa(lit(1,-1)) == PA::f);
   pa.set(lit(1,-1), PA::t);
   assert(pa.at(1) == PA::f);
   pa.at(1) = PA::t;
   assert(pa.at(1) == PA::t);
   assert(pa.size() == 1);
  }
  {Pass pa(5);
   pa.set({{1,PA::t},{-2,PA::t},{1,PA::f}});
   assert(pa.size() == 2);
  }

  {Pass pa(5);
   assert(not sat(pa, CL{}));
   pa.set({{1,PA::t},{-2,PA::t},{5,PA::f}});
   assert(not sat(pa, CL{}));
   assert(sat(pa, CL{3,-2}));
   assert(not sat(pa, CL{-1,2,3,-4,5}));
  }

  {ClauseSet F({10,20});
   assert(valid(F));
   F.F = {{1,2,-4},{},{3,-7,9}};
   assert(valid(F.F));
   assert(not valid(F));
   F.update();
   assert(F.s.l == 6);
   assert(valid(F));
   Pass pa(5);
   assert(occurs(1, pa, F));
   assert(degree(1, pa, F) == 1);
   assert(not occurs(-1, pa, F));
   assert(degree(-1, pa, F) == 0);
   assert(occurs(2, pa, F));
   assert(degree(2, pa, F) == 1);
   assert(not occurs(10, pa, F));
   assert(degree(10, pa, F) == 0);
   pa.set({{1,PA::t},{-5,PA::t}});
   assert(pa.size() == 2);
   assert(not occurs(1, pa, F));
   assert(not occurs(-1, pa, F));
   assert(not occurs(2, pa, F));
   assert(occurs(3, pa, F));
  }

  {ClauseSet F({10,10});
   assert(valid(F));
   {Pass pa(10);
    assert(pa.size() == 0);
    assert(add_pure(pa, F) == 10);
    assert(pa.size() == 10);
    for (VAR v = 1; v <= 10; ++v) {
      assert(pa[v] == PA::b);
      assert(pa.at(v) == PA::b);
    }
    assert(add_pure(pa, F) == 0);
    pa.clear();
    assert(pa.size() == 0);
   }
   F.F = {{1,2,-4},{},{3,-7,9},{-1,-2,9},{3,-9}};
   F.update();
   assert(valid(F));
   {Pass pa(10);
    assert(degree(3,pa,F) == 2);
    assert(add_pure(pa, F) == 7); // 3, -4, 5, 6, -7, 8, 10 -> f
    assert(pa.size() == 7);
    assert(pa(3) == PA::fp);
    assert(pa(-4) == PA::fp);
    assert(pa[1] == PA::o);
    pa[1] = PA::t; // 1 -> t
    assert(pa.size() == 8);
    assert(degree(1,pa,F) == 0);
    assert(degree(-1,pa,F) == 0);
    assert(degree(2,pa,F) == 0);
    assert(degree(-2,pa,F) == 1);
    assert(add_pure(pa, F) == 1); // -2 -> f
    for (VAR v = 1; v <= 10; ++v) {
      if (v == 9) {
        assert(degree(v,pa,F) == 2);
        assert(degree(lit(v,-1),pa,F) == 1);
      }
      else {
        assert(degree(v,pa,F) == 0);
        assert(degree(lit(v,-1),pa,F) == 0);
      }
    }
    assert(add_pure(pa, F) == 0);
   }
  }

  test_count<Count64>();
  test_count<Count80>();
  test_count<Count_mpz>();

  {const CLS F = {{1,2},{2,-3},{3,-4},{-1,4}};
   assert(maxn(F) == 4);
   const auto m = profile(F);
   assert(m.at({}) == 4);
   assert(m.at({0}) == 1);
   assert(m.at({0,1}) == 2);
   assert(m.at({0,2}) == 1);
   assert(m.at({1}) == 1);
   assert(m.at({1,3}) == 1);
   assert(m.at({2}) == 3);
   assert(m.at({3}) == 3);
  }

}
