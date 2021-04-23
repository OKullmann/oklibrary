// Oliver Kullmann, 23.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#ifndef WINDOWS_DtyrxAKxZG
#define WINDOWS_DtyrxAKxZG

#include <string>

#include <cassert>

#include <GL/glew.h>
#include <GL/glut.h>

namespace Windows {

  struct WinPar {
    static constexpr int s = 800, maxx = 2000, maxy = 1100;
    const int x, y, w, h;
    const std::string n;

    WinPar(const int x, const int y, const int w, const int h,
           const std::string n) noexcept : x(x), y(y), w(w), h(h), n(n) {}
    WinPar(const unsigned i) noexcept : x(xi(i)), y(yi(i)), w(wi(i)), h(hi(i)), n(ni(i))  {}

    static int xi(const unsigned i) noexcept {
      if (i % 2 == 0) return (i/2) * 100;
      else return 1200 - (i/2) * 100;
    }
    static int yi(const unsigned i) noexcept {
      if (i % 4 <= 1) return (i/4) * 50;
      else return 300 - (i/4) * 50;
    }
    static int wi(const unsigned) noexcept { return s; }
    static int hi(const unsigned) noexcept { return s; }
    static std::string ni(const unsigned i) noexcept {
      return "ODE: " + std::to_string(i);
    }

    int create() const noexcept {
      assert(number == 0);
      glutInitWindowSize(w, h);
      glutInitWindowPosition(x, y);
      return number = glutCreateWindow(n.c_str());
    }
    int index() const noexcept { return number; }
  private :
    mutable int number = 0;
  };

  template <class LW>
  void init_windows(const unsigned nw, LW& lw) noexcept {
    assert(nw >= 1);
    for (unsigned i = 0; i < nw; ++i) lw[i] = WinPar(i).create();
    glutSetWindow(lw[0]);
  }

}

#endif
