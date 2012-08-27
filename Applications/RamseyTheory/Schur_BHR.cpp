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
  const std::string version = "0.1.2";

  const int cols = 3;
  const int upb = 24+1; // upper bound+1 for n for expected solutions
  const int lowb = 23-1; // lower bound-1 for n on enumerated solutions

  long int count = 0;

  short int Base[cols][upb+1];
  short int wsol[upb];
  int nums[upb+1][upb+1];
  int wmax = 0, max = 0;

  inline void updatesol() {
    // there seems to be an error in the original code: "max" is redefined
    static int lprint = 0;
    max = wmax;
    assert(max < upb);
    if (max > lowb and max >= lprint) {
      for (int i = 1; i <= max; ++i) std::printf("%d", wsol[i]+1);
      std::printf(" %d %d\n", wmax, ++count); // also here apparently errors in original
      lprint = max;
    }
    std::cout.flush();
  }

  inline bool excludedp(const int n) {
    assert(n >= 0);
    assert(n <= upb);
    bool res = true;
    // apparently error in original:
    for (int i = 0; i < cols; ++i) res &= Base[i][n];
    return res;
  }

  inline int add_col(const int col) {
    assert(wmax + 1 < upb);
    wsol[++wmax] = col;
    if (wmax >= max) updatesol();
    int ex = 0;
    {int j = 1;
     for (int i = 1; i < std::min(wmax, upb-wmax); ++i) {
       const int sum = wmax + i;
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
    const int col = wsol[wmax];
    {int i = 1;
     while (nums[wmax][i]) {
       --Base[col][nums[wmax][i]];
       nums[wmax][i++] = 0;
       assert(i <= upb or nums[wmax][i] == 0);
     }
    }
    --wmax;
  }

  inline bool colp(const int x) {
    assert(x >= 0);
    assert(x < cols);
    assert(wmax < upb);
    return not Base[x][wmax+1];
  }

  inline void brute_force(const int b) {
    for (int i = 0; i <= std::min(cols-1,b+1); ++i)
      if (colp(i)) {
        if (not add_col(i)) brute_force(std::max(i,b));
        del_col();
      }
  }
  
}

int main() {
  for (int i = 0; i < cols; ++i)
    for (int j = 0; j <= upb; ++j)
      Base[i][j] = 0;
  max = lowb;
  brute_force(-1);
}
