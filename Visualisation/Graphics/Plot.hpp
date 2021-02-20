// Oliver Kullmann, 20.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#include <vector>
#include <array>
#include <algorithm>
#include <cmath>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>

#ifndef PLOT_KHDZ4FlnIj
#define PLOT_KHDZ4FlnIj

namespace Plot {

  // Translating a sequence of points into a vector of points in the unit-cube:
  template <typename FLOAT>
  struct UnitCubes {
    typedef FLOAT float_t;
    typedef std::array<float_t, 2> point_t;
    typedef std::vector<point_t> points_vt;

    const points_vt pv;

    template <typename FLOAT2>
    UnitCubes(const std::vector<FLOAT2> v, const FLOAT2 xmin, const FLOAT2 xmax, const FLOAT2 ymin, const FLOAT2 ymax)
      : pv(transfer(v,xmin,xmax,ymin,ymax)) {}

    template <typename FLOAT2, class VEC>
    static points_vt transfer(const VEC& v, const FLOAT2 xmin, const FLOAT2 xmax, const FLOAT2 ymin, const FLOAT2 ymax) {
      assert(xmin <= xmax);
      assert(ymin <= ymax);
      points_vt res;
      res.reserve(v.size());

      const auto xtrans = xmin==xmax ?
        [](const FLOAT2 x)->FLOAT2{return 0;} :
        [&xmin,&xmax](const FLOAT2 x){
          std::lerp(FLOAT2(-1), FLOAT2(1), (x-xmin)/(xmax-xmin));};
      const auto ytrans = ymin==ymax ?
        [](const FLOAT2 y)->FLOAT2{return 0;} :
        [&ymin,&ymax](const FLOAT2 y){
          std::lerp(FLOAT2(-1), FLOAT2(1), (y-ymin)/(ymax-ymin));};

      for (const auto& [x,y] : v) {
        const float_t
          xt = xtrans(std::clamp<FLOAT2>(x, xmin, xmax)),
          yt = ytrans(std::clamp<FLOAT2>(y, ymin, ymax));
         res.push_back({xt,yt});
      }
      return res;
    }
  };

}

#endif
