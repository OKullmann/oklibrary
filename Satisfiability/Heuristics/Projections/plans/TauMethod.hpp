// Oliver Kullmann, 16.6.2007 (Swansea)

/*!
  \file Satisfiability/Heuristics/Projections/plans/TauMethod.hpp
  \brief Plans regarding the tau-projection

  \todo Efficient computation of the tau-function:
  <ol>
   <li> Two arguments. </li>
   <li> At most three arguments. </li>
   <li> At most k arguments (k constant; using metaprogramming). </li>
   <li> Arbitrary many arguments. </li>
  </ol>
  Using tables should speed up the computation.

  \todo Experimentation
  <ul>
   <li> More experimentation about the difference between tau with
   2 arguments and the product. </li>
  </ul>

  \todo Decision
  <ul>
   <li> For theoretical purposes we should also implement a decision
   algorithm for comparing tau-values of branching tuples. </li>
  </ul>

  \todo Theory
  <ul>
   <li> First collect all the tau-theory from the [Kullmann/Luckhardt]-papers. </li>
  </ul>

*/
