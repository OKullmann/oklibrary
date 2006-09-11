// Oliver Kullmann, 17.2.2006

/*!
  \file AllSolutions/plans/AllSolutions.hpp
  \brief Plans for the module on components for finding all
  satisfying assignments.

  Computing representations for the set of all satisfying assignments.

  \todo The first approach is to find an equivalent DNF
  by DPLL (without further simplification it will actually
  compute a hitting DNF).

  \todo What are measurements for minimisation of the DNF? The tau-value is
  a possibility, but it's 2 for (boolean) hitting clause-sets, and
  greater otherwise, so on its own it just says "use hitting cls".
  The number of partial assignments (the number of clauses in the
  DNF), or some other size measurement of the DNF are possibilities.
  We need a literature survey here. There should be something in
  the computer learning literature (which studies conversion between
  CNF and DNF).

  \todo See HypergraphTransversals/DirectTransversalEnumeration.hpp
  for the problem of finding all minimal satisfying assignments.

  \todo Another approach is using OBDD's (this should likely go
  into its own module).

  \todo What is the relation to the counting problem? (Likely counting
  should go into it's own module; techniques seem to be different, and
  approximation makes sense for counting (not for enumeration).)
*/

/*!
  \namespace OKlib::AllSolutions
  \brief Components for extracting all satisfying assignments (in
  some form).
*/

namespace OKlib {
  namespace AllSolutions {
  }
}

