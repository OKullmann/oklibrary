// Oleg Zaikin, 6.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

A Gecode-based solver for the Euler square completion problem.

*/

/* TODOS:

0. Design for general k
    - Based on the "constraint" LS(a,b,c) for a,b,c in {1, ..., k+2}.
    - Just posting all binom(N+2,3) such LS-constraints, plus the equalities
      (element-constraints) between these LS's.

1. Handle special types of Latin squares.
    - Diagonal Latin square: in the main diagonal and antidiagonal all elements are distinct.
    - Constant diagonal Latin square: in the main diagonal all elements are equal to each other, the same stands for elements of the main antidiagonal.
    - Idempotent Latin square: A(i,i) = i.
    - Symmetric Latin square: it is equal to its transpose.
    - Semisymmetric Latin square: it is equal to (at least) three of its conjugates, which must necessarily include the (1, 2, 3), (3, 1, 2) and (2, 3, 1)-conjugates.
    - Totally symmetric Latin square: all six its conjugates are equal.

2. Handle special types of orthogonality.
    - Self-orthogonal Latin square: it is orthogonal to its transpose.
    - Doubly self-orthogonal Latin square: it is orthogonal to both its transpose and antitranspose.

*/


#include <iostream>

#include <ProgramOptions/Environment.hpp>

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.1",
        "6.3.2022",
        __FILE__,
        "Oleg Zaikin and Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/Mols.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;

  return 0;
}
