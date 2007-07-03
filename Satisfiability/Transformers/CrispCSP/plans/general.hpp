// Oliver Kullmann, 20.11.2005 (Swansea)

/*!
  \file CrispCSP/plans/general.hpp
  \brief Plans for the module CrispCSP


  \todo The CSP formats
  <ol>
   <li> http://cpai.ucc.ie/05/xml.html </li>
   <li> http://cpai.ucc.ie/05/table.html </li>
  </ol>
  are to be transformed into alliances of active P-clause-sets (or specialised
  versions; perhaps just one P-clause-set is also alright; "mono-signed"
  full clauses per constraint are exactly enough).
  The benchmarks from http://cpai.ucc.ie/05/CallForSolvers.html should
  all be tried!


  \todo The "relations" are just "monosigned" full clause-sets, to be used
  to specify the constraints (which each just specify a specific set
  of variables). So it should be worth using some preprocessing
  for the relations. We can use equivalence-simplifications (and we
  should exploit that for us constraints can be arbitrary P-clause-sets).


  \todo List of equivalence reductions:
   - resolution clause-reductions


  \todo One should study the problem, which clause-sets are especially
  suitable for the representation of relations. If the relation is small,
  then for example all prime implicants can be computed. And all
  symmetries (but perhaps this doesn't help much?!).

*/

