// Oliver Kullmann, 20.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  - UnitCubes<FLOAT>
  - Draw

TODOS:

0. Generalise coord() to grid().

1. When is glFlush() needed?

2. For colours see also Queens/GenericSAT/Colour.hpp.

*/

#include <vector>
#include <array>
#include <algorithm>
#include <cmath>

#include <cassert>
#include <cstdlib>

#include <GL/glew.h>

#include <Numerics/FloatingPoint.hpp>

#ifndef PLOT_KHDZ4FlnIj
#define PLOT_KHDZ4FlnIj

namespace Plot {

  /* Translating a sequence of points into a vector of points in the unit-cube
      - Construct with
          UnitCubes uc(vector_of_points);
        or with
          UnitCubes uc(vector_of_points, xmin,xmax, ymin,ymax);
      - Thereafter the vector of translated points in in uc.pv.
  */
  template <typename FLOAT>
  struct UnitCubes {
    typedef FLOAT float_t;
    typedef std::array<float_t, 2> point_t;
    typedef std::vector<point_t> points_vt;

    const float_t xmin, xmax, ymin, ymax;
    const points_vt pv;

    template <typename FLOAT2, class VEC>
    UnitCubes(const VEC& v, const FLOAT2 xmin_, const FLOAT2 xmax_, const FLOAT2 ymin_, const FLOAT2 ymax_)
      : xmin(xmin_), xmax(xmax_), ymin(ymin_), ymax(ymax_),
        pv(transfer(v,xmin_,xmax_,ymin_,ymax_)) {}

    static constexpr auto pr1 = [](const auto p){return p[0];};
    static constexpr auto cp1 = [](const auto p1, const auto p2)
      {return pr1(p1) < pr1(p2);};
    static constexpr auto pr2 = [](const auto p){return p[1];};
    static constexpr auto cp2 = [](const auto p1, const auto p2)
      {return pr2(p1) < pr2(p2);};

    template <class VEC>
    explicit UnitCubes(const VEC& v) :
      UnitCubes(v,
        pr1(*std::min_element(v.begin(),v.end(), cp1)),
        pr1(*std::max_element(v.begin(),v.end(), cp1)),
        pr2(*std::min_element(v.begin(),v.end(), cp2)),
        pr2(*std::max_element(v.begin(),v.end(), cp2))) {}

    template <typename FLOAT2, class VEC>
    static points_vt transfer(const VEC& v, const FLOAT2 xmin, const FLOAT2 xmax, const FLOAT2 ymin, const FLOAT2 ymax) {
      assert(xmin <= xmax);
      assert(ymin <= ymax);
      points_vt res;
      res.reserve(v.size());

      const auto xtrans =
        [&xmin,&xmax](const FLOAT2 x)->FLOAT2{
          return std::lerp(FLOAT2(-1), FLOAT2(1), (x-xmin)/(xmax-xmin));};
      const auto ytrans =
        [&ymin,&ymax](const FLOAT2 y)->FLOAT2{
          return std::lerp(FLOAT2(-1), FLOAT2(1), (y-ymin)/(ymax-ymin));};

      for (const auto& [x,y] : v) {
        const float_t
          xt = xmin==xmax ? 0 : xtrans(std::clamp<FLOAT2>(x, xmin, xmax)),
          yt = ymin==ymax ? 0 : ytrans(std::clamp<FLOAT2>(y, ymin, ymax));
        res.push_back({xt,yt});
      }
      return res;
    }
  };


  struct RGB {
    double r, g, b;
  };
  constexpr RGB red{1,0,0};
  constexpr RGB green{0,1,0};
  constexpr RGB blue{0,0,1};
  constexpr RGB yellow{1,1,0};
  constexpr RGB magenta{1,0,1};
  constexpr RGB cyan{0,1,1};

  constexpr RGB black{0,0,0};
  constexpr RGB grey{0.5,0.5,0.5};
  constexpr RGB white{1,1,1};

  constexpr RGB darkgreen{0,0.5,0};
  constexpr RGB lightgrey{0.75, 0.75, 0.75};
  constexpr RGB darkgrey{0.25, 0.25, 0.25};

  constexpr std::array<RGB, 8>
  first_colours{yellow, blue, white, green, red, magenta, cyan, darkgreen};


  struct Draw {
    typedef UnitCubes<GLfloat> ucgl_t;

    void background_colour(const RGB c) noexcept { bc = c; }
    RGB background_colour() const noexcept { return bc; }
    void plot_colour(const RGB c) noexcept { pc = c; }
    RGB plot_colour() const noexcept { return pc; }

    const ucgl_t uc;

    template <typename FLOAT, class VEC>
    Draw(const VEC& v, const FLOAT xmin, const FLOAT xmax, const FLOAT ymin, const FLOAT ymax)
      : uc(v,xmin,xmax,ymin,ymax) {}

    template <class VEC>
    explicit Draw(const VEC& v) : uc(v) {}

    void clear() const noexcept {
      activate_background_colour();
      glClear(GL_COLOR_BUFFER_BIT);
    }
    void coord() const noexcept {
      set_colour(grey);
      glBegin(GL_LINES);
      glVertex2f(-1, 0);
      glVertex2f(1, 0);
      glEnd();
      glBegin(GL_LINES);
      glVertex2f(0, -1);
      glVertex2f(0, 1);
      glEnd();
    }
    void graph(const bool y0) const noexcept {
      activate_colour();
      if (y0) yzero();
      GLuint buffer = 0;
      glGenBuffers(1, &buffer);
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferData(GL_ARRAY_BUFFER,
                   uc.pv.size() * sizeof(ucgl_t::point_t), &uc.pv[0],
                   GL_STATIC_DRAW);
      constexpr GLuint attribute_coord2d = 0;
      glEnableVertexAttribArray(attribute_coord2d);
      glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
      glDrawArrays(GL_LINE_STRIP, 0, uc.pv.size());

      glDisableVertexAttribArray(attribute_coord2d);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glFlush();
    }

    void new_plot(const bool y0) const noexcept {
      clear();
      //coord();
      graph(y0);
    }
  private :

    void yzero() const noexcept {
      if (uc.ymin > 0 or uc.ymax < 0 or uc.ymin == uc.ymax) return;
      const auto y = std::lerp(GLfloat(-1), GLfloat(1),
                               (0 - uc.ymin) / (uc.ymax - uc.ymin));
      activate_colour();
      glBegin(GL_LINES);
      glVertex2f(-1, y);
      glVertex2f(1, y);
      glEnd();
    }

    RGB bc = black; // background-colour
    void activate_background_colour() const noexcept {
      glClearColor(bc.r, bc.g, bc.b, 0);
    }

    RGB pc; // plot-colour
    void set_colour(const RGB c) const noexcept { glColor3f(c.r, c.g, c.b); }
    void activate_colour() const noexcept { glColor3f(pc.r, pc.g, pc.b); }

  };

}

#endif
