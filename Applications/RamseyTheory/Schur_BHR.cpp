// Oliver Kullmann, 27.8.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/RamseyTheory/Schur_BHR.cpp
  \brief Program from [BHR 2006], adapted to C++

  Code from [Partitions into sum-free sets; Peter F. Blanchard, Frank Harary,
  Rogerio Reis; INTEGERS: Electronic Journal of combinatorial number theory 6,
  2006], http://www.integers-ejcnt.org/vol6.html .

  To redefine the main parameters COLS, UPB, LOWB, set them via CPPFLAGS, e.g.,
  \code
    > CPPFLAGS="-DCOLS=2 -DUPB=10 -DLOWB=7" oklib all
  \endcode


  \bug Assertion in add_col fails after enumerating some solutions for upb=24
  <ul>
   <li> Perhaps it must hold that upb-1 is a true upper bound on wschur(cols).
   </li>
  </ul>

*/

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <string>

namespace {

  const std::string program = "Schur_BHR";
  const std::string version = "0.1.7";

#ifndef COLS
# define COLS 3
#endif
#ifndef UPB
# define UPB 24+1
#endif
#ifndef LOWB
# define LOWB 23-1
#endif

  const unsigned int cols = COLS;
  const unsigned int upb = UPB; // upper bound+1 for n for expected solutions
  const unsigned int lowb = LOWB; // lower bound-1 for n on enumerated solutions

  long unsigned int count = 0;

  short unsigned int Base[cols][upb+1] = {};
  short unsigned int wsol[upb];
  unsigned int nums[upb+1][upb+1];
  unsigned int wmax = 0, max = lowb;

  inline void updatesol() {
    // there seems to be an error in the original code: "max" is redefined
    static unsigned int lprint = 0;
    max = wmax;
    assert(max < upb);
    if (max > lowb and max >= lprint) {
      for (unsigned int i = 1; i <= max; ++i) std::printf("%u", wsol[i]+1);
      std::printf(" %u %lu\n", wmax, ++count); // also here apparently errors in original
      lprint = max;
    }
    std::cout.flush();
  }

  inline bool excludedp(const unsigned int n) {
    assert(n <= upb);
    bool res = true;
    // apparently error in original:
    for (unsigned int i = 0; i < cols; ++i) res &= Base[i][n];
    return res;
  }

  inline unsigned int add_col(const unsigned int col) {
    assert(wmax + 1 < upb);
    wsol[++wmax] = col;
    if (wmax >= max) updatesol();
    unsigned int ex = 0;
    {unsigned int j = 1;
     assert(wmax <= upb);
     for (unsigned int i = 1; i < std::min(wmax, upb-wmax); ++i) {
       const unsigned int sum = wmax + i;
       if (wsol[i] == col) {
         nums[wmax][j++] = sum;
         ++Base[col][sum];
         if ((sum < std::max(lowb, max)) and excludedp(sum)) ++ex;
       }
     }
    }
    return ex;
  }

  inline void del_col() {
    assert(wmax < upb);
    const unsigned short int col = wsol[wmax];
    {unsigned int i = 1;
     while (nums[wmax][i]) {
       assert(Base[col][nums[wmax][i]] >= 1);
       --Base[col][nums[wmax][i]];
       nums[wmax][i++] = 0;
       assert(i <= upb or nums[wmax][i] == 0);
     }
    }
    --wmax;
  }

  inline bool colp(const unsigned int x) {
    assert(x < cols);
    assert(wmax < upb);
    return not Base[x][wmax+1];
  }

  inline void brute_force(const unsigned int b) {
    for (unsigned int i = 0; i < std::min(cols,b+1); ++i)
      if (colp(i)) {
        if (not add_col(i)) brute_force(std::max(i+1, b));
        del_col();
      }
  }
  
}

int main() {
  brute_force(0);
}
