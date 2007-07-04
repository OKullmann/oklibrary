// Oliver Kullmann, 4.7.2007 (Swansea)

/*!
  \file LogicalReflection/FormalisedProofs/plans/UpperBounds.hpp
  \brief Plans on how to formalise (and automatise) upper bounds proofs


  \todo Partitionings of processes
  <ul>
   <li> A fundamental problem is, that given an instance F, a certain
   process F -> F', and a measure mu, one wants to obtain a lower bound
   on mu(F) - mu(F'). </li>
   <li> The fundamental technique is to subdivide the transition F -> F'
   into
    <center>
    F_0 -> F_1 ... -> F_k -> ... -> F_m
    </center>
   with F_0 = F and F_m = F' such that mu(F_k) >= mu(F_0), and then
    <center>
    mu(F) - mu(F') >= mu(F_0) - m(F_k) = sum_{i=0}^{k-1} mu(F_{i+1}) - mu(F_i)
    </center>
    The first k steps are the focus of the considerations, and the remaining
    steps are not taken into consideration. </li>
    <li> Examples are for applications phi*F of partial assignments
    the consideration of literal occurrences removed in clauses not satisfied
    complemented by the consideration of satisfied clauses. </li>
    <li> If the division into steps F_i -> F_{i+1} is fine enough, then
    typically F_i is divided into disjoint parts F_i^j such that
    the process F_i -> F_{i+1} can be applied to each F_i^j, obtaining
    F_i^j', such that
     <center>
     mu(F_i) - mu(F_{i+1}) = sum_j mu(F_i^j) - mu(F_i^j').
     </center>
    </li>
    <li> %Tools and methods have to be developed. </li>
  </ul>

*/

