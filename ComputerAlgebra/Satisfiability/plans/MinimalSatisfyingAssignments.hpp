// Oliver Kullmann, 29.6.2007 (Swansea)

/*!
  \file Transitional/ComputerAlgebra/Satisfiability/plans/MinimalSatisfyingAssignments.hpp
  \brief Plans for computing the set of minimal satisfying assignments


  \todo Implement the trivial algorithm
  <ul>
   <li> The algorithm is as follows:
    <ol>
     <li> Input is a set F of conditions. </li>
     <li> Split F into F_1, F_2. </li>
     <li> Solve (recursively) F_1, F_2. </li>
     <li> Compute all consistent compositions. </li>
     <li> Perform subsumption-elimination. </li>
    </ol>
  </ul>

*/

