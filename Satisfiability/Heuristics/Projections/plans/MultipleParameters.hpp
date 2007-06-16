// Oliver Kullmann, 16.6.2007 (Swansea)

/*!
  \file Satisfiability/Heuristics/Projections/plans/MultipleParameters.hpp
  \brief Plans regarding the use of multiple parameters

  The tau-function (see Projections/plans/TauMethod.hpp) is the canonical
  method to handle one distance function d (this distance incorporates
  "all" knowledge). Here the situation is considered where several
  distance functions d_1, ..., d_m are given. "Hyper-branching tuples"
  are now tuples of m-tuples.

  \todo Linear combination
  <ul>
   <li> The simplest approach is to set
   <center>
    d := alpha_1 * d_1 + ... + alpha_m * d_m
   </center>
   for non-negative alpha_i, and then to use the tau-function.
    <ol>
     <li> See [Kullmann/Luckhardt] for some remarks. </li>
    </ol>
   </li>
   <li> Eppstein's method (see XXX) allows to find "semi-optimal" values for
   alpha_i given a fixed set of (integral) "worst-case equations".
    <ol>
     <li> We should generalise this method to handle arbitrary real-valued
     d_i (this shouldn't be too hard). </li>
     <li> The fixed set of "worst-cases" needs to be interpreted more
     generally, so that we can obtain some heuristical method to obtain
     "representative cases" at certain times, so that the alpha_i can
     be computed dynamically. </li>
     <li> Can we also include negative alpha_i? This would be needed to
     do something for the [Kullmann, TCS]-3-SAT paper. </li>
    </ol>
   </li>
  </ul>

  
  \todo Experimentation:
  <ul>
   <li> What is a good test case to start experimentation? </li>
   <li> The three simplest measures n, c, l would be a start. </li>
   <li> Then one can differentiate between variable- and clause-degrees. </li>
   <li> As usual, random formulas (in various disguises) are nice
   test instances. </li>
  </ul>

*/
