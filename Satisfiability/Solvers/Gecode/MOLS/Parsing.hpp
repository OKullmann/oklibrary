// Oliver Kullmann, 8.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Parsing of the problem specification


0. Design for general k
    - N is constant.
    - We have primary squares A, B, C, ... (any identifier).
        intro A B C
    - Then we have secondary squares given by some unary operation on the
      primary squares:
       - We have t(A), at(A) (transposed and antitransposed) --- for these we
         likely do not introduce new variables.
         We could use "tA" (forbidding that an identifier starts with "t").
       - The question is here where t(A) can be used; likely we want a very
         restricted usage.
         diag(tA) is equivalent to diag(A), and the same for antidiag and
         at.
         So t(A) and at(A) are only used in lists for orthogonality?

         QUESTION: But perhaps it is actually easier to formally treat t(A)
         and at(A) as new squares, and then to actually eliminate them
         when posting the constraints?
       - Other types of "conjugates" likely need the introduction of new
         squares X = conj(A); expressed like
           conj A X
         introducing X.
       - A more specific syntax is needed:
           conj231 A A231
       - For the latter it is relevant that we can express the relation easily
         (likely via the elementship-constraint)>
    - Then we have conditions on those primary and secondary squares:
     - diag(A), antidiag(X), written
         diag A
         antidiag X
     - ortho A, X, t(B) ..., written
         ortho A X tB
       (here for the formulation we use pairwise the witness-squares; only
       here can we use say tB, atB).
     - = A X .

1. Handle special types of Latin squares (unary conditions):
    - Diagonal Latin square: in the main diagonal and antidiagonal all
      elements are distinct.
      We want two special conditions:
        diag A
        antidiag A
    - Constant diagonal Latin square: in the main diagonal all elements are
      equal to each other, similar for elements of the main antidiagonal.
      These ls's are called "unipotent" (and similarly "antiunipotent").
      Conditions:
        uni A
        antiuni A
    - Idempotent Latin square: A(i,i) = i.
        idem A
      Similarly "row-reduced", "column-reduced":
        rred A
        cred A
    - Symmetric Latin square: it is equal to its transpose.
        symm A
        antisymm A

        QUESTION: it seems that for N>=3 one can not have both at the same
        time? But perhaps easiest to allow such combinations.
    - Later (does not seem to be realisable):
      Semisymmetric Latin square: it is equal to (at least) three of its
      conjugates, which must necessarily include the (1, 2, 3), (3, 1, 2) and
      (2, 3, 1)-conjugates.
    - Totally symmetric Latin square: all six conjugates are equal:
      can be expressed by
       conj321 A A321
       ,,.
       symm A
       = A A321
       ...


2. Handle special types of orthogonality.
    - For example the search for 3 MOLS is expressed by
        intro A B C
        ortho A B C
    - Self-orthogonal Latin square: it is orthogonal to its transposition:
        intro A
        ortho A t(A)
    - Doubly self-orthogonal Latin square: it is orthogonal to both its
      transposed and antitransposed:
        intro A
        ortho A t(A)
        ortho A at(A).

*/

#ifndef PARSING_W8d7RNyiKu
#define PARSING_W8d7RNyiKu

#include <cassert>

namespace Parsing {


}

#endif
