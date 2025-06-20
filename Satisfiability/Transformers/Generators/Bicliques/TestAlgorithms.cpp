// Oliver Kullmann, 26.3.2023 (Swansea)
/* Copyright 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumBasicFunctions.hpp>

#include <Transformers/Generators/Random/Algorithms.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>
#include <Transformers/Generators/Random/Sequences.hpp>

#include "TestTools.hpp"
#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.0",
        "8.6.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestAlgorithms.cpp",
        "GPL v3"};

  using namespace TestTools;
  using namespace Algorithms;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {typedef std::vector<int> v_t;
   assert(pointed_view(v_t{}, 0).empty());
   assert(std::ranges::equal(pointed_view(v_t{}, 0), v_t{}));
   assert(pointed_view(v_t{0}, 0).empty());
   assert(std::ranges::equal(pointed_view(v_t{0}, 0), v_t{}));
   assert(std::ranges::equal(pointed_view(v_t{1,0}, 0), v_t{0}));
   assert(std::ranges::equal(pointed_view(v_t{1,0}, 1), v_t{1}));
   assert(std::ranges::equal(pointed_view(v_t{2,1,0}, 1), v_t{2,0}));
  }
  {typedef std::array<unsigned, 3> a3_t;
   typedef std::array<unsigned, 2> a2_t;
   static_assert(std::ranges::equal(pointed_view(a3_t{0,3,2}, 0), a2_t{3,2}));
   static_assert(std::ranges::equal(pointed_view(a3_t{0,3,2}, 1), a2_t{0,2}));
   static_assert(std::ranges::equal(pointed_view(a3_t{0,3,2}, 2), a2_t{0,3}));
   static_assert(std::ranges::equal(pointed_view(a3_t{0,3,2}, 3), a3_t{0,3,2}));
  }

  {using r_t = std::vector<std::vector<int>>;
   assert(rmerge(r_t{}).empty());
   assert(rmerge(r_t{{},{},{}}).empty());
   assert(eqp(rmerge(r_t{{2,3,1}}), {2,3,1}));
   assert(eqp(rmerge(r_t{{2,3,1},{}}), {2,3,1}));
   assert(eqp(rmerge(r_t{{},{2,3,1}}), {2,3,1}));
   assert(eqp(rmerge(r_t{{1,5,7},{-1},{},{2,3,7,8},{-1,10}}), {-1,-1,1,2,3,5,7,7,8,10}));
  }

  {typedef std::vector<int> v_t;
   assert(empty_intersection(v_t{}, v_t{}));
   assert(empty_intersection(v_t{1}, v_t{}));
   assert(empty_intersection(v_t{}, v_t{1}));
   assert(not empty_intersection(v_t{1}, v_t{1}));
   assert(not empty_intersection(v_t{1}, v_t{0,1,2}));
   assert(not empty_intersection(v_t{0,1,2}, v_t{1}));
   const auto empty_int = [](const v_t& v1, const v_t& v2){
     v_t res;
     std::ranges::set_intersection(v1, v2, std::back_inserter(res));
     return res.empty();
   };
   RandGen::RandGen_t g({77});
   const auto size_dist = RandGen::UniformRange(g, 10);
   const auto val1_dist = RandGen::UniformRange(g, 10);
   const auto val2_dist = RandGen::UniformRange(g, 10, 5);
   for (unsigned i = 0; i < 10000; ++i) {
     const auto N1 = size_dist(); const auto N2 = size_dist();
     v_t v1; v1.reserve(N1); v_t v2; v2.reserve(N2);
     for (unsigned j = 0; j < N1; ++j) v1.push_back(val1_dist());
     for (unsigned j = 0; j < N2; ++j) v2.push_back(val2_dist());
     assert(empty_int(v1, v2) == empty_intersection(v1, v2));
   }
  }

  {typedef std::vector<int> v_t;
   assert(intersection(v_t{}, v_t{}) == v_t{});
   assert(intersection(v_t{1,2,3}, v_t{2,4,5}) == v_t{2});
   typedef std::vector<double> v2_t;
   assert((intersection(v2_t{-5.5,-3,1,2,2.1,3}, v_t{-5,-3,2,4,5})
           == v2_t{-3,2}));
  }

  {typedef std::vector<int> v_t;
   assert(eqp(split(v_t{},v_t{}), {}));
   assert(eqp(split(v_t{0},v_t{}), {{{0},{},{}}}));
   assert(eqp(split(v_t{},v_t{0}), {{{},{0},{}}}));
   assert(eqp(split(v_t{0},v_t{0}), {{{},{},{0}}}));
   assert(eqp(split(v_t{0,0},v_t{0}), {{{0},{},{0}}}));
   assert(eqp(split(v_t{0,0},v_t{0,0}), {{{},{},{0,0}}}));
   assert(eqp(split(v_t{0,0,0},v_t{0,0}), {{{0},{},{0,0}}}));
   assert(eqp(split(v_t{0,0},v_t{0,0,0}), {{{},{0},{0,0}}}));
   assert(eqp(split(v_t{0,0,1,1,2,3,4,4,4},v_t{0,0,0,2,2,4}),
              {{{1,1,3,4,4},{0,2},{0,0,2,4}}}));
   assert(eqp(split(v_t{1,2,3},v_t{2,4,5}),
              {{{1,3},{4,5},{2}}}));
   typedef std::vector<double> v2_t;
   typedef std::array<v2_t, 3> a2_t;
   assert((split(v2_t{-5.5,-3,1,2,2.1,3},v_t{-5,-3,2,4,5}) ==
           a2_t{{{-5.5,1,2.1,3},{-5,4,5},{-3,2}}}));
  }
  {using uint_t = RandGen::gen_uint_t;
   typedef std::vector<uint_t> v_t;
   for (const uint_t R : v_t{2,5,10,30,50,100}) {
     auto U = RandGen::UniformRangeS(R,RandGen::transform({R}));
     const uint_t s0 = 13, s1 = 17;
     for (unsigned T = 0; T < 1000; ++T) {
       v_t v0(s0), v1(s1);
       for (auto& x : v0) x = U();
       std::ranges::sort(v0);
       for (auto& x : v1) x = U();
       std::ranges::sort(v1);
       const auto res = split(v0, v1);
       assert(sum_sizes(res) + res[2].size() == v0.size() + v1.size());
       assert(res[2] == intersection(v0, v1));
       assert(std::ranges::is_sorted(res[0]));
       assert(std::ranges::is_sorted(res[1]));
     }
   }
  }

  {typedef std::vector<int> vi_t;
   typedef std::vector<unsigned> vu_t;
   typedef std::vector<double> vd_t;
   typedef std::vector<std::string> vs_t;

   assert(append_ranges(vi_t{}, vd_t{}) == vi_t{});
   assert(append_ranges(vi_t{}, vd_t{}, vu_t{}) == vi_t{});
   assert((append_ranges(vi_t{0,3,2}, vd_t{1.1,-2.2}) == vi_t{0,3,2,1,-2}));
   assert((append_ranges(vi_t{0,3,2}, vd_t{1.1,-2.2}, vu_t{77,99}) ==
           vi_t{0,3,2,1,-2,77,99}));
   assert((append_ranges(vs_t{"abc","def"}, vs_t{"ghi","jkl"}) ==
          vs_t{"abc","def","ghi","jkl"}));
   assert((append_ranges(vs_t{"abc","def"}, vs_t{"ghi","jkl"}, vs_t{"mno"}) ==
           vs_t{"abc","def","ghi","jkl","mno"}));

   struct X {
     X() {}
     X(const std::string&) {};
     bool operator ==(const X&) const noexcept = default;
   };
   typedef std::vector<X> vx_t;
   assert(append_ranges(vx_t{}, vx_t{}) == vx_t{});
   assert((vx_t{{},{}}.size() == 2)); // this *needs* declaration of X()
   assert((append_ranges(vx_t{{}}, vx_t{{}}) == vx_t{{},{}}));
   assert((append_ranges(vx_t{{}}, vx_t{{}}, vx_t{{}}) == vx_t{{},{},{}}));
   assert((append_ranges(vx_t{{}}, vx_t{{}}, vx_t{{}}) == vx_t{{""},{""},{""}}));
   assert((append_ranges(vx_t{{}}, vs_t{"A"}, vs_t{"B"}) == vx_t{{""},{""},{""}}));
  }
  {typedef std::vector<double> v1t;
   typedef std::vector<v1t> vv1t;
   typedef std::vector<int> v2t;
   typedef std::vector<v2t> vv2t;
   assert(append2d_ranges(vv1t{}, vv2t{}) == vv1t{});
   assert(append2d_ranges(vv1t{}, vv2t{{1,2},{}}) == vv1t{});
   assert((append2d_ranges(vv1t{{1.5,2.5}}, vv2t{{1,2},{}}) ==
           vv1t{{1.5,2.5,1,2}}));
   assert((append2d_ranges(vv2t{{1,2},{7,8},{9}}, vv1t{{3.5,4.5},{}}) ==
           vv2t{{1,2,3,4},{7,8}}));
  }

  {typedef std::vector<int> v_t;
   v_t v;
   const auto tr = [](const auto&){return true;};
   const auto fa = [](const auto&){return false;};
   assert(erase_if_unstable(v, tr) == 0);
   assert(v.empty());
   v.assign({0});
   assert(erase_if_unstable(v, fa) == 0);
   assert(eqp(v, {0}));
   assert(erase_if_unstable(v, tr) == 1);
   assert(v.empty());
   v.assign({1,2,3});
   assert(erase_if_unstable(v, fa) == 0);
   assert(eqp(v, {1,2,3}));
   assert(erase_if_unstable(v, tr) == 3);
   assert(v.empty());
   v.assign({1,2,3,4,5});
   const auto t35 = [](const auto& x){return x==3 or x==5;};
   assert(erase_if_unstable(v, t35) == 2);
   assert(eqp(v, {1,2,4}));
   v.assign({3,5,1,2,3,3,5});
   assert(erase_if_unstable(v, t35) == 5);
   assert(eqp(v, {2,1}));
   v.assign({77,3,5,1,2,3,3,5,7});
   assert(erase_if_unstable(v, t35) == 5);
   assert(eqp(v, {77,7,2,1}));
   v.assign({3,77,3,5,1,2,3,3,5,7,3});
   assert(erase_if_unstable(v, t35) ==7);
   assert(eqp(v, {7,77,2,1}));
   const auto odd = [](const auto x){return x%2 == 1;};
   for (unsigned n = 0; n < 100; ++n) {
     v_t v = RandGen::random_permutation<v_t>(n, {});
     assert(erase_if_unstable(v, odd) == n / 2);
     assert(v.size() == (n+1)/2);
     std::ranges::sort(v);
     for (unsigned i = 0; i < (n+1)/2; ++i) assert(v[i] == int(2*i));
   }
   RandGen::RandGen_t g(RandGen::vec_eseed_t{1683393744725523748ULL});
   for (unsigned round = 0; round < 1000; ++round) {
     auto choice0 = RandGen::choose_kn(10, 40, g);
     auto choice = append_ranges(std::move(choice0),
                                 RandGen::choose_kn(10, 40, g));
     auto copy(choice);
     const auto r1 = std::erase_if(copy, odd); std::ranges::sort(copy);
     const auto r2 = erase_if_unstable(choice, odd); std::ranges::sort(choice);
     assert(r1 == r2);
     assert(choice == copy);
   }
  }

  {typedef std::vector<unsigned> v_t;
    v_t v;
   erase_indices(v, v_t{});
   assert(v.empty());
   v = {77};
   erase_indices(v, v_t{});
   assert(eqp(v, {77}));
   erase_indices(v, v_t{0});
   assert(v.empty());
   v = {77,88,99};
   erase_indices(v, v_t{0});
   assert(eqp(v, {88,99}));
   erase_indices(v, v_t{1});
   assert(eqp(v, {88}));
   v = {77,88,99};
   erase_indices(v, v_t{0,1,2});
   assert(v.empty());
   // v.assign_range(make_uint_iterator_range(0,10)); ERROR GCC 14.2
   v = std::ranges::to<v_t>(make_uint_iterator_range(0,10));
   erase_indices(v, v_t{1,3,5});
   assert(eqp(v, {0,2,4,6,7,8,9}));
   erase_indices(v, v_t{0,1});
   assert(eqp(v, {4,6,7,8,9}));
   erase_indices(v, v_t{2,4});
   assert(eqp(v, {4,6,8}));
   v = std::ranges::to<v_t>(make_uint_iterator_range(0,11));
   erase_indices(v, v_t{0,2,4,7,9});
   assert(eqp(v, {1,3,5,6,8,10}));
  }

  {RandGen::RandGen_t g;
   typedef std::vector<unsigned> v_t;
   v_t V;
   erase_krandom(V, 0, g); erase_krandom(V,1,g);
   assert(g == RandGen::RandGen_t{});
   V = {0};
   erase_krandom(V,1,g);
   assert(V.empty());
   assert(g == RandGen::RandGen_t{});
   V.resize(10);
   std::ranges::iota(V,0);
   erase_krandom(V, 4, g);
   assert(eqp(V, {3,4,5,6,8,9}));
   erase_krandom(V, 3, g);
   assert(eqp(V, {3,4,6}));
   erase_krandom(V, 2, g);
   assert(eqp(V, {6}));
   erase_krandom(V,1,g);
   assert(V.empty());
   V.resize(10);
   std::ranges::iota(V,0);
   keep_krandom(V,100,g);
   assert(V == std::ranges::to<v_t>(make_uint_iterator_range<unsigned>(0,10)));
   keep_krandom(V,0,g);
   assert(V.empty());
   V.resize(10);
   std::ranges::iota(V,0);
   keep_krandom(V,4,g);
   assert(eqp(V, {4,5,6,7}));
   keep_krandom(V,3,g);
   assert(eqp(V, {4,5,7}));
   keep_krandom(V,1,g);
   assert(eqp(V, {4}));
  }
  {RandGen::RandGen_t g{123,987};
   typedef std::vector<unsigned> v_t;
   v_t V;
   erase_prandom(V, {0,1}, g);
   assert(V.empty());
   assert((g == RandGen::RandGen_t{123,987}));
   V.resize(10);
   std::ranges::iota(V,0);
   erase_prandom(V, {1,2}, g);
   assert(eqp(V, {1,3,4,6,7}));
   constexpr unsigned size = 20;
   const v_t V0(uint_iterator_t<unsigned>(1234),
                uint_iterator_t<unsigned>(1234+size));
   assert(V0.size() == size);
   v_t count(size+1);
   const auto experiment = [&g, &V0, &count]
     (const RandGen::Prob64 p, const unsigned N = 1000,
      const bool erase = true) noexcept {
     std::ranges::fill(count, 0);
     for (const auto _ : std::views::repeat(0, N)) {
       v_t V(V0);
       if (erase) erase_prandom(V, p, g);
       else keep_prandom(V, p, g);
       ++count[V.size()];
     }};
   experiment({3,4});
   assert(eqp(count,
          {1,23,61,133,199,228,162,91,59,32,9,2,0,0,0,0,0,0,0,0,0}));
   experiment({1,4});
   assert(eqp(count,
        {0,0,0,0,0,0,0,0,1,4,10,35,56,114,163,190,218,120,72,14,3}));
   experiment({1,4}, 10000);
   assert(eqp(count,
     {0,0,0,0,0,0,0,1,8,27,81,284,714,1153,1642,2000,1884,1277,685,205,39}));
   experiment({3,4}, 1000, false);
   assert(eqp(count,
        {0,0,0,0,0,0,0,0,0,2,11,25,65,129,173,191,188,132,58,25,1}));
   experiment({1,4}, 1000, false);
   assert(eqp(count,
          {7,19,69,135,193,193,153,118,64,26,16,6,0,1,0,0,0,0,0,0,0}));
  }

  {typedef std::vector<unsigned> v_t;
   assert(eqp(complement_uint(v_t{}, 0), {}));
   assert(eqp(complement_uint(v_t{}, 1), {0}));
   assert(eqp(complement_uint(v_t{}, 2), {0,1}));
   assert(eqp(complement_uint(v_t{2,3,4}, 2), {0,1}));
   assert(eqp(complement_uint(v_t{0,1,2,3,4}, 2), {}));
   assert(eqp(complement_uint(v_t{0,2,3,4}, 2), {1}));
   assert(eqp(complement_uint(v_t{1,2,3,4}, 2), {0}));
   assert(eqp(complement_uint(v_t{1,2,3,4}, 5), {0}));
   assert(eqp(complement_uint(v_t{1,2,3,4}, 6), {0,5}));
   assert(eqp(complement_uint(v_t{0,2,4}, 6), {1,3,5}));
   assert(eqp(complement_uint(v_t{0,2,4,7,9}, 10), {1,3,5,6,8}));
   assert(eqp(complement_uint(v_t{0,2,4,7,9}, 11), {1,3,5,6,8,10}));
  }

  {typedef std::vector<int> va_t;
   typedef std::vector<double> vb_t;
   assert(complement_subsequence(va_t{}, vb_t{}) == vb_t{});
   assert((complement_subsequence(va_t{}, vb_t{2,-1,0.5}) == vb_t{2,-1,0.5}));
   assert((complement_subsequence(va_t{-1}, vb_t{2,-1,0.5}) == vb_t{2,0.5}));
   assert((complement_subsequence(va_t{2}, vb_t{2,-1,0.5}) == vb_t{-1,0.5}));
   assert((complement_subsequence(va_t{2,-1}, vb_t{2,-1,0.5}) == vb_t{0.5}));
   assert((complement_subsequence(va_t{2,-1,4,3},
                                  vb_t{2,-1,0.5,1.5,4,2.5,4,3,5.5}) ==
           vb_t{0.5, 1.5, 2.5, 4, 5.5}));
  }

  {using vt = std::vector<std::vector<int>>;
   assert(sum_sizes(vt{}) == 0);
   assert(sum_proj(vt{}, [](auto){return 1;}) == 0);
   assert(sum_sizes(vt{{},{1,2},{5,6,7}}) == 5);
   assert(sum_proj(vt{{},{1,2},{5,6,7}}, [](auto){return 1;}) == 3);
   assert(prod_sizes(vt{{},{1,2},{5,6,7}}) == 0);
   assert(prod_proj(vt{{},{1,2},{5,6,7}}, [](auto){return 2;}) == 8);
  }
  {using vt = std::array<int [5], 2>;
   assert(sum_sizes(vt{}) == 10);
   assert(prod_sizes(vt{}) == 25);
  }

  {typedef std::vector<int> v_t;
   assert(is_strictly_ascending(v_t{}));
   assert(is_strictly_ascending(v_t{-1}));
   assert(is_strictly_ascending(v_t{-1,0}));
   assert(not is_strictly_ascending(v_t{0,0}));
   assert(is_strictly_ascending(v_t{-1,0,1}));
   assert(not is_strictly_ascending(v_t{0,0,1}));
   assert(not is_strictly_ascending(v_t{-1,0,0}));
  }

  {typedef std::vector<int> v_t;
   typedef std::vector<v_t> vv_t;
   assert(eqp(nt_eqel_bysort(vv_t{}), {}));
   assert(eqp(nt_eqel_bysort(vv_t{v_t{}}), {}));
   assert(eqp(nt_eqel_bysort(vv_t{v_t{},v_t{}}), {{0,1}}));
   assert(eqp(nt_eqel_bysort(vv_t{v_t{},v_t{0},v_t{},v_t{2},v_t{0}}),
                             {{0,2},{1,4}}));
  }
  {RandGen::UniformVectors V(4,17,{});
   for (size_t s = 0; s <= 30; ++s)
     for (size_t i = 0; i < 200; ++i) {
       const auto S = V(s);
       const auto res1 = nt_eqel_bydef(S);
       auto res2 = nt_eqel_bysort(S);
       std::ranges::sort(res2);
       assert(res1 == res2);
       auto res3 = nt_eqel_byhash(S);
       std::ranges::sort(res3);
       assert(res1 == res3);
     }
  }
  {RandGen::UniformVectors V(6,7,{333});
   for (size_t i = 0; i < 200; ++i)
     for (size_t s = 0; s < 33; ++s) {
       std::vector<std::vector<size_t>> v2; v2.reserve(s);
       for (size_t j = 0; j < s; ++j) v2.push_back(V(5));
       const auto res1 = nt_eqel_bydef(v2);
       auto res2 = nt_eqel_bysort(v2);
       std::ranges::sort(res2);
       assert(res1 == res2);
       const auto res3 = nt_eqel_byhash(v2);
       assert(res1 == res3);
       const auto res4 = nt_eqel_byhash(v2, FloatingPoint::hash_UInt_range());
       assert(res1 == res4);
     }
  }

  {const auto pred = [](const unsigned i, const unsigned j){return i==j;};
   typedef std::vector<unsigned> v_t;

   assert(is_independent(v_t{}, pred));
   assert(is_independent(v_t{0}, pred));
   assert(not is_independent(v_t{0,0}, pred));
   assert(is_independent(v_t{0,1}, pred));
   assert(not is_independent(v_t{0,1,0}, pred));
   assert(is_independent(v_t{0,1,2}, pred));

   assert(orderly_max_independent_unstable(v_t{}, pred) == v_t{});
   assert(orderly_max_independent_unstable(v_t{0}, pred) == v_t{0});
   assert((orderly_max_independent_unstable(v_t{0,1}, pred) == v_t{1,0}));
   assert((orderly_max_independent_unstable(v_t{0,1,2}, pred) == v_t{2,1,0}));
   assert(orderly_max_independent_unstable(v_t{0,0}, pred) == v_t{0});
   assert(orderly_max_independent_unstable(v_t{0,0,0}, pred) == v_t{0});
   assert((orderly_max_independent_unstable(v_t{0,0,1}, pred) == v_t{1,0}));
   assert((orderly_max_independent_unstable(v_t{0,0,1,1}, pred) == v_t{1,0}));
   assert((orderly_max_independent_unstable(v_t{1,0,2,1}, pred) == v_t{1,0,2}));

   assert(orderly_max_independent(v_t{}, pred) == v_t{});
   assert(orderly_max_independent(v_t{0}, pred) == v_t{0});
   assert((orderly_max_independent(v_t{0,1}, pred) == v_t{1,0}));
   assert((orderly_max_independent(v_t{0,1,2}, pred) == v_t{2,1,0}));
   assert(orderly_max_independent(v_t{0,0}, pred) == v_t{0});
   assert(orderly_max_independent(v_t{0,0,0}, pred) == v_t{0});
   assert((orderly_max_independent(v_t{0,0,1}, pred) == v_t{1,0}));
   assert((orderly_max_independent(v_t{0,0,1,1}, pred) == v_t{1,0}));
   assert((orderly_max_independent(v_t{1,0,2,1}, pred) == v_t{1,2,0}));
  }

  {using uit = uint_iterator_t<unsigned>;
   static_assert(uit().current == 0);
   static_assert(uit(77).current == 77);
   static_assert(uit{77}.current == 77);
   static_assert(uit(uit(88)).current == 88);
   uit u;
   ++u; assert(u.current == 1);
   --u; assert(u.current == 0);
   assert(u++.current == 0); assert(u.current == 1);
   assert(u--.current == 1); assert(u.current == 0);
   assert((u += 7).current == 7); assert(u.current == 7);
   assert((u += -2).current == 5); assert(u.current == 5);
   assert((u -= 3).current == 2); assert(u.current == 2);
   assert((u -= -8).current == 10); assert(u.current == 10);
   assert(u[0] == 10); assert(u[1] == 11); assert(u[-1] == 9);
   assert(u.current == 10); assert(*u == 10);
   assert(u == uit(10)); assert(u != uit());
   assert(u < uit(11)); assert(u > uit(9));
   assert(u+5 == uit(15)); assert(u-2 == uit(8));
   assert(7+u == uit(17)); assert(-7 + u == uit(3));
   assert(u - 10 == uit()); assert(u - -2 == uit(12));
   assert(u - uit(5) == 5); assert(u - uit(10) == 0);
   assert(u - (u + 1) == unsigned(-1));
  }
  {assert(make_uint_iterator_range(unsigned(-1), unsigned(1)).size() == 2);
   assert(make_uint_iterator_range(0,0).empty());
   assert(make_uint_iterator_range(unsigned(1), unsigned(0)).size() == unsigned(-1));
  }

  {typedef std::vector<unsigned> v_t;
   assert(not strict_subsumption_test_sized<v_t>({}, {}));
   assert(strict_subsumption_test_sized<v_t>({}, {0}));
   assert(not strict_subsumption_test_sized<v_t>({0}, {}));
  }
  {typedef std::vector<unsigned> v_t;
   typedef std::vector<v_t> r_t;
   r_t R;
   forward_strictsubsumption_by_erase(R);
   assert(R.empty());
   R = {{1,2},{2}};
   forward_strictsubsumption_by_erase(R);
   assert(eqp(R, {{1,2},{2}}));
   R = {{1,2},{1,2}};
   forward_strictsubsumption_by_erase(R);
   assert(eqp(R, {{1,2},{1,2}}));
   R = {{1,2},{1,2,3}};
   forward_strictsubsumption_by_erase(R);
   assert(eqp(R, {{1,2}}));
  }

}
