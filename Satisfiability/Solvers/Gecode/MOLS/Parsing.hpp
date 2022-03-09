// Oliver Kullmann, 8.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Parsing of the problem specification


1. Example:


squares A B c10 # comment; k=3

# empty lines possible (always optional)

A        rls cls symm # like the simple format for graphs
at B     cls diag # these are conditions for at(B)
c231 A   idem rred # these variants will be introduced (basic encoding)
A        uni cls # as with programming languages, space-symbols are collapsed

= c213 c10  c321 A
= B B # we allow that (in the basic encoding)

ortho A c213 B at A
ortho A # perhaps this we disallow?


The order above is irrelvant except of that "squares" acts like a declaration
in programming languages (several declarations possible, only affecting
following lines; redeclarations yield errors).

The search for three MOLS is formulated as:

squares A B C
A rls cls
B rls cls
C rls cls
ortho A B C


A self-orthogonal ls:

squares A
ortho A c213 A
ortho A at A


Totally symmetric Latin square: all six conjugates are equal:

squares A
A symm
= A c312 A
= A c321 A
= A c213 A
= A c231 A
= A c132 A


QUESTION: it seems that for N>=3 one can not have both diag and antidiag
at the same time? But easiest to allow such combinations (and such tests
can be useful for performance evaluation).


1. Conditions not included:
    - Semisymmetric Latin square: it is equal to (at least) three of its
      conjugates, which must necessarily include the (1, 2, 3), (3, 1, 2) and
      (2, 3, 1)-conjugates.

      Due to the "or" we don't handle that.

*/

#ifndef PARSING_W8d7RNyiKu
#define PARSING_W8d7RNyiKu

#include <cassert>

namespace Parsing {


}

#endif
