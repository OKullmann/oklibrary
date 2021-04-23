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
    const int x, y, w, h;
    const std::string n;
    WinPar(int x, int y, int w, int h, std::string n) noexcept :
      x(x), y(y), w(w), h(h), n(n) {}
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

  const WinPar wpar1(100, 2000, 800,800, "Solution");
  const WinPar wpar2(1100,2000, 800,800, "Accuracy");

  template <class LW, class LWP>
  void init_windows(const unsigned nw, LW& lw, const LWP lwp) noexcept {
    assert(nw >= 1);
    for (unsigned i = 0; i < nw; ++i) lw[i] = lwp[i].create();
    glutSetWindow(lw[0]);
  }

}

#endif
