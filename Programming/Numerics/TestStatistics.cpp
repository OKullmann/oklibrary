// Oliver Kullmann, 8.11.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <vector>
#include <sstream>

#include <cassert>
#include <cmath>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Statistics.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.3",
        "21.5.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestStatistics.cpp",
        "GPL v3"};

  using namespace GenStats;
  namespace FP = FloatingPoint;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {typedef BStatsR<int> bsr;
   assert(bsr{} == bsr(0,0,0,0,0));
   bsr b;
   assert(b == bsr{});
   b.N = 1; b.min = 2; b.amean = 3; b.max = 4; b.sdc = 5;
   assert((b == bsr{1,2,3,4,5}));
   assert(bsr{} < b);
   std::ostringstream ss;
   ss << b;
   assert(ss.str() == "1 : 2 3 4; 5");
   typedef StatsR<int> ebsr;
   assert(ebsr{} == ebsr({}, 0));
   ebsr eb(b, 77);
   assert(bsr(eb) == b);
   assert(eb.median == 77);
   assert(ebsr{} < eb);
   ss.str("");
   ss << eb;
   assert(ss.str() == "1 : 2 3 4; 5 77");
  }

  {typedef BasicStats<FP::UInt_t, FP::float80> bst;
   bst S;
   assert((S == bst{0,0,0,FP::P264m1,-FP::P264m1-1}));
   assert(S.min() == FP::P264m1);
   assert(S.max() == -FP::P264m1-1);
   assert(S.amean() == 0);
   assert(S.var_population() == 0);
   assert(S.var_unbiased() == 0);
   assert(S.sd_population() == 0);
   assert(S.sd_corrected() == 0);
   S+=1;
   assert((S == bst{1,1,1,1,1}));
   assert(S.amean() == 1);
   assert(S.var_population() == 0);
   assert(S.var_unbiased() == 0);
   assert(S.sd_population() == 0);
   assert(S.sd_corrected() == 0);
   S+=0; (S+=2)+=3;
   assert(S.min() == 0);
   assert(S.max() == 3);
   assert(S.N() == 4);
   assert(S.sum() == 6);
   assert(S.sum_sq() == 14);
   assert((S.base() == bst::base_t{6,14,0,3}));
   assert(S.amean() == 1.5L);
   assert(S.var_population() == 1.25L);
   assert(S.var_unbiased() == 5.0L / 3.0L);
   assert(S.sd_population() == std::sqrt(1.25L));
   assert(S.sd_corrected() == std::sqrt(5.0L/3.0L));
   assert(eqp(S.extract(), {4, 0, 1.5, 3, std::sqrt(5.0L/3.0L)}));
   assert(eqp(BStatsR<FP::float80>(S), {4, 0, 1.5, 3, std::sqrt(5.0L/3.0L)}));
   bst S2(11, 12, 14, 1, 2);
   S += S2;
   assert((S == bst{15, 18, 28, 0, 3}));
   S2 += S;
   assert((S2 == bst{26, 30, 42, 0, 3}));
   assert((S + S2 + S == bst{56, 66, 98, 0, 3}));
  }

  {StatsStore<FP::Int_t, FP::float80> ss;
   assert(ss.N() == 0);
   assert(ss.sum() == 0);
   assert(ss.amean() == 0);
   assert(ss.sum_sqd() == 0);
   assert(ss.var_population() == 0);
   assert(ss.sd_population() == 0);
   assert(ss.median() == 0);
   ss += 1;
   assert(ss.N() == 1);
   assert(ss.sum() == 1);
   assert(ss.amean() == 1);
   assert(ss.sum_sqd() == 0);
   assert(ss.var_population() == 0);
   assert(ss.sd_population() == 0);
   assert(ss.median() == 1);
   ss += 2;
   assert(ss.N() == 2);
   assert(ss.sum() == 3);
   assert(ss.amean() == 1.5);
   assert(ss.sum_sqd() == 0.5);
   assert(ss.var_population() == 0.25);
   assert(ss.sd_population() == 0.5);
   assert(ss.median() == 1.5);
   assert(ss.var_population() == 0.25);
   assert(ss.sd_population() == 0.5);
   ss += -1;
   assert(ss.N() == 3);
   assert(ss.min() == -1);
   assert(ss.max() == 2);
   assert(ss.sum() == 2);
   assert(ss.amean() == 2.0L/3);
   assert(ss.sum_sqd() == 42.0L/9);
   assert(ss.var_population() == 42.0L/27);
   assert(ss.sd_population() == FP::sqrt(42.0L/27));
   assert(ss.sd_corrected() == FP::sqrt(21.0L/9));
   assert(ss.median() == 1);
   assert(ss.var_population() == 42.0L/27);
   assert(ss.sd_population() == FP::sqrt(42.0L/27));
   assert(eqp(ss.extract(), {{3, -1, 2.0L/3, 2, FP::sqrt(21.0L/9)}, 1}));
  }

  {typedef RandVal<FP::float80> RV;
   RV rv(2, {});
   assert(not rv.sorted);
   assert(rv.a(0) == 0);
   assert(rv.b(0) == 1);
   assert(rv.a(1) == 0);
   assert(rv.b(1) == 1);
   rv.a(0,-1);
   assert(rv.a(0) == -1);
   rv.b(1,2);
   assert(rv.b(1) == 2);
   assert(rv.N == RV::default_N);
   rv.N = 20000;
   assert(rv.N == 20000);
   typedef RV::vec_t v_t;
   // Intervals [-1,1] and [0,2]:
   const auto res = rv.run([](const v_t& v){return v[0]+v[1];});
   assert(res.min() == -0.96904338503248971986L);
   assert(res.max() == 2.9817165562207108166L);
   const auto res2 = rv.run([](const v_t& v){return v[0]*v[1];});
   assert(res2.min() == -1.9541231351623961616L);
   assert(res2.max() == 1.9898062578958443037L);
   typedef RV::function_t f_t;
   f_t diff = [](const v_t& v){return v[0]-v[1];};
   auto res3 = rv.run(diff);
   assert(res3.min() == -2.9820686473523859189L);
   assert(res3.max() == 0.97118761954145433376L);
   rv.sorted = true;
   res3 = rv.run(diff);
   assert(res3.min() == -2.9859418709374049908L);
   assert(res3.max() == -4.9237024167701304127e-05L);
  }

  {typedef StatsPoints<FP::float80> SP;
   typedef SP::point_t point_t;
   typedef std::vector<point_t> v_t;
   {const SP s(v_t{{0,0}});
    assert(s.N == 1);
    assert((s.xmin == point_t{0,0}));
    assert((s.xmax == point_t{0,0}));
    assert((s.ymin == point_t{0,0}));
    assert((s.ymax == point_t{0,0}));
    assert(s.xmid == 0);
    assert(s.ymid == 0);
    assert(s.xspan == 0);
    assert(s.yspan == 0);
    assert(FP::isnan(s.spanq));
    assert(s.ymean == 0);
    assert(s.ysd == 0);
    assert(s.ymed == 0);
   }
   {const SP s(v_t{{1,2},{2,4}});
    assert(s.N == 2);
    assert((s.xmin == point_t{1,2}));
    assert((s.xmax == point_t{2,4}));
    assert((s.ymin == point_t{1,2}));
    assert((s.ymax == point_t{2,4}));
    assert(s.xmid == 1.5);
    assert(s.ymid == 3);
    assert(s.xspan == 1);
    assert(s.yspan == 2);
    assert(s.spanq == 2);
    assert(s.ymean == 3);
    assert(s.ysd == 1);
    assert(s.ymed == 3);
   }
   {const SP s(v_t{{-1,1},{1,3},{3,0}});
    assert(s.N == 3);
    assert((s.xmin == point_t{-1,1}));
    assert((s.xmax == point_t{3,0}));
    assert((s.ymin == point_t{3,0}));
    assert((s.ymax == point_t{1,3}));
    assert(s.xmid == 1);
    assert(s.ymid == 1.5);
    assert(s.xspan == 4);
    assert(s.yspan == 3);
    assert(s.spanq == 0.75);
    assert(s.ymean == 4.0L/3);
    assert(s.ysd == FP::sqrt(14) / 3);
    assert(s.ymed == 1);
   }
  }

  {typedef FreqStats<FP::UInt_t, FP::float80> fst;
   typedef fst::stats_t stats_t;
   
   fst FS;
   assert(FS == fst{});
   assert(FS.num_inputs() == 0);
   assert(FS.num_values() == 0);
   assert(FS.cmap().empty());
   assert(FS.min() == FP::UInt_t(-1));
   assert(FS.max() == 0);
   assert(FS.sum() == 0);
   assert(FS.sum_sq() == 0);
   assert(FS.sum_sqd() == 0);
   assert(FS.amean() == 0);
   assert((FS.extract1() == stats_t{{0,FP::UInt_t(-1),0,0,0},0}));
   assert((FS.extract2() == stats_t{{0,FP::UInt_t(-1),0,0,0},0}));
   std::ostringstream ss;
   ss << FS;
   assert(ss.str() == "L1 0 : 1.84467e+19 0 0; 0 0\n");
   assert(eqp(FS += 0, {1,1}));
   assert(eqp(FS.cmap(), {{0,1}}));
   assert((FS.extract1() == stats_t{{1,0,0,0,0},0}));
   assert((FS.extract2() == stats_t{{1,1,1,1,0},1}));
   ss.str(""); ss << FS;
   assert(ss.str() == "L1 1 : 0 0 0; 0 0\n0:1\nL2 1 : 1 1 1; 0 1\n");
   assert(eqp(FS += 0, {1,0}));
   assert(eqp(FS.cmap(), {{0,2}}));
   assert((FS.extract1() == stats_t{{2,0,0,0,0},0}));
   assert((FS.extract2() == stats_t{{1,2,2,2,0},2}));
   assert(eqp(FS += 1, {1,1}));
   assert(eqp(FS.cmap(), {{0,2},{1,1}}));
   assert((FS.extract1() == stats_t{{3,0,1.0L/3,1,FP::sqrt(1.0L/3)},0}));
   assert((FS.extract2() == stats_t{{2,1,1.5,2,FP::sqrt(0.5)},1.5}));
   assert(eqp(FS += 1, {1,0}));
   assert(eqp(FS.cmap(), {{0,2},{1,2}}));
   assert((FS.extract1() == stats_t{{4,0,0.5,1,FP::sqrt(1.0L/3)},0.5}));
   assert((FS.extract2() == stats_t{{2,2,2,2,0},2}));
   assert(eqp(FS += 2, {1,1}));
   assert(eqp(FS.cmap(), {{0,2},{1,2},{2,1}}));
   {const auto ex = FS.extract1();
    assert(ex.N == 5); assert(ex.min == 0); assert(ex.amean == 0.8L); assert(ex.max == 2);
    assert(FP::accuracy(FP::sqrt(7.0L/10), ex.sdc) <= 1);
    assert(ex.median == 1);
    assert((FS.extract2() == stats_t{{3,1,5.0L/3,2,FP::sqrt(1.0L/3)},2}));
   }
   assert(eqp(FS += 10, {1,1}));
   assert(eqp(FS.cmap(), {{0,2},{1,2},{2,1},{10,1}}));
   {const auto ex = FS.extract1();
    assert(ex.median == 1);
   }
   assert((FS.extract2() == stats_t{{4,1,1.5,2,FP::sqrt(1.0L/3)},1.5}));
  }
}
