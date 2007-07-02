// Oliver Kullmann, 14.2.2006 (Swansea)

/*!
  \file FrequencyAssignment/plans/general.hpp
  \brief Plans for the module on frequency assignment


  \todo Update namespaces.


  \todo First the most important notions of frequency assignments
  have to be captured and formulated as generalised satisfiability
  problems. A good entry seems to be provided by
  \verbatim
  @TechReport{ZIB2001FAP,
  author =       {Karen I. Aardal and Stan %P.M. van Hoesel and Arie %M.C.A. Koster and Carlo Mannino and Antonio Sassano},
  title =        {Models and Solution Techniques for Frequency Assignment Problems},
  institution =  {Konrad-Zuse-Zentrum f\"ur Informationstechnik Berlin (ZIB)},
  year =         2001,
  number =       {ZIB-Report 01-40},
  month =        {December},
  annote =       {pdf-Datei vorhanden.}
  }
  \endverbatim
  The most important models seem MO-FAP, MS-FAP, MB-FAP, MI-FAP.
  Likely we can make use of the greater generality provided by our framework,
  and don't need to stick to the special formulations.


  \todo The above technical report mentions several benchmark collections;
  we need parsers. Perhaps the problem structures presented here are more
  important to us than those basic theoretical forms of FAP's (since we generalise
  them anyway).


  \todo MO-FAP: (G, D, c, T, K)
   - for vertex v in V(G) with requested number c(v) of frequencies and set D(v) <= NN_0
     of possible frequencies we have variables v_1, ..., v_{c(v)}, each with
     domain D(v), and the constraint INJ({v_1, ..., v_{c(v)}}) (see module
     InjectivityConstraints);
   - for every edge {u,v} in E(G) we have an active constraint forbidding the differences
     in T({u,v}) (it seems questionable to me whether the compression enabled
     by T is worth to pursue, or whether we better consider the more general constraint,
     which allows to forbid specific pairs of frequencies (then perhaps we should gather
     all these constraints in a ("real") clause-set); if T just demands, that the distance
     is at least a certain threshold t, then T might be worth its own active clause-set (and
     other systematic situations are conceivable), if T however is just "random", then
     perhaps it's not worth a special active clause-set);
   - for all variables together (gathered in set VA) we have the upper bound constraint
     UPPER_V(VA, K) (see module LinearInequalities).


  \todo MS-FAP: We MO-FAP, but the third condition (the constraint on K) is replaced by:
   - maximal value for v in VA - minimal value for v in VA <= K.


  \todo Are there competitions on the subject? (We look only at the decision side; likely most
  activities in this field concentrate on optimisation?!)

*/

/*!
  \namespace OKlib::FrequencyAssignment
  \brief Active clause-sets for frequency assignment
*/

namespace OKlib {
  namespace FrequencyAssignment {
  }
}

