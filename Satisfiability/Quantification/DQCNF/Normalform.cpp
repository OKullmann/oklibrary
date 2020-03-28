// Oliver Kullmann, 27.3.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Quantification/DQCNF/Normalform.cpp
  \brief Transforming a (D)QCNF into a kind of normal form


TODOS:

0. Transfer the related todos from AutarkiesL1.cpp to here.

1. QCNF:
    (a) no repeated clauses or literals
    (b) no tautological clauses
    (c) clauses and literals sorted
    (d) every variable occurs
    (e) no repeated or empty a/e-lines
    (f) no comments, parameter-values are exact, one clause per line, always
        using exactly one space for separation, no superfluous spaces
    (g) the largest literal in a clause is existential (thus no pseudo-empty
        clauses)
    (h) consecutive variable-indices 1, ..., n are used, and this in the
        blocks, i.e., the first block is 1,...,p, the second p+1,...,q,
        and so on.

2. DQCNF:
    - (a) - (f) as for QCNF
    - (g) is generalised to: every universal variable in a clause is in the
      dependency-set of some existential literal in the clause
    - the universal variables are 1, ..., na, the existential are na+1, ...,
      n = na+ne
    - It remains the question of the presentation of the dependencies:
     - Fix some maximal chain in the partial order of dependency-sets (each
       labelled with its multiplicity).
     - Then use this for the a/e-lines, plus a final a-line for the uncovered
       elements, and with uncovered e-variables added via d lines as soon
       as all their variables are available (after an e-line); this all in
       numerical order, and this makes this well-defined.
     - So the remaining question is how to find a well-defined maximal chain,
       quickly computable.
     - Perhaps a greedy algorithm: find a dependency set with maximal product
         size * weight;
       if there are several, choose the last in the order (which in the C++
       program is likely lexicographic order).
       Now extend this greedily.

3. The renaming is strictly monotonic on universal variables and on
   existential variables.

4. Are both QCNF and DQCNF handled here?

5. From the input-file a directory is created, with five files (or six --
   see Point 8 below):
    - a symbolic link to the original file
    - the new (d)qdimacs-file
    - the transcript of changes
    - the renaming
    - the statistics.

6. If the original file has a pseudo-empty clause, then perhaps the
   translated formula is:
p cnf 1 2
e 1
1 0
-1 0

7. For the transcript of changes, most important is the difference
in the dimacs-parameter-values.
    - Perhaps this pair of natural numbers >= 0 is the output to
      standard output.
    - Perhaps as third difference the difference in literal occurrences.
    - If a pseudo-empty clause was found, then none of these numbers,
      but an alert for this.
    - In the (most) silent mode of the program, only these three numbers
      are output (or the alert), nothing else (to just see quickly whether
      the instance contains trivial redundancies).
    - Otherwise the directory-name is also output.

8. The statistics are also made available in a simple machine-readable
   format, so that other programs can read besides the file containing the
   normal form also statistics.
    - Easiest seems one variable per line, where each line starts with the
      name of the variable (an identifier), followed by the value.

*/
