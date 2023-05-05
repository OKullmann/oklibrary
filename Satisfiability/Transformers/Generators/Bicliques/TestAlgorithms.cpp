// Oliver Kullmann, 26.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
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

#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>

#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "5.5.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestAlgorithms.cpp",
        "GPL v3"};

  using namespace Algorithms;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

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

  {typedef std::vector<int> v_t;
   v_t v;
   const auto tr = [](const auto&){return true;};
   const auto fa = [](const auto&){return false;};
   assert(erase_if_byswap(v, tr) == 0);
   assert(v.empty());
   v.assign({1,2,3});
   assert(erase_if_byswap(v, fa) == 0);
   assert(v.size() == 3);
   assert(erase_if_byswap(v, tr) == 3);
   assert(v.empty());
   v.assign({1,2,3,4,5});
   const auto t35 = [](const auto& x){return x==3 or x==5;};
   assert(erase_if_byswap(v, t35) == 2);
   assert(eqp(v, {1,2,4}));
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

}
