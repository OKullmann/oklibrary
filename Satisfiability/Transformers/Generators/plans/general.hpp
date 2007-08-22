// Oliver Kullmann, 20.11.2005 (Swansea)

/*!
  \file Generators/plans/general.hpp
  \brief Plans for the module %Generators (for creating SAT problems)


  \todo Update namespaces.


  \todo Update
  <ul>
   <li>
    <ol> 
     <li> FaronsPuzzle.hpp : </li>
     <li> Filter.cpp : what is the purpose of this?? </li>
     <li> LinInequal.hpp : </li>
     <li> Ramsey.cpp </li>
     <li> Ramsey.hpp </li>
     <li> Generators/Test.cpp (the main procedure for Faron's Puzzle) : </li>
     <li> Tests_Ramsey : </li>
     <li> Test_Transformations.cpp : </li>
     <li> VanderWaerden.cpp : </li>
     <li> VanderWaerden.hpp : </li>
     <li> OKsolver/Experimental/AllgKlassen200203/TransformationsBiclique.hpp : </li>
    </ol>
   </li>
   <li> Transfer OKsolver/Experimental/AllgKlassen200203/TransformationsBiclique to here. </li>
   <li> The main generators should produce generalised clause-sets.
   If output and/or boolean formulas is needed, then further
   transformers are to be used.
   The variables are tuples of values.
   </li>
  </ul>
  


  \todo We need active clause-sets for biclique-transformations for
  interesting classes of graphs (including complete graphs).


  \todo The HKB-formulas (MUSAT_{delta=2}, where every literal occurs
  at least once) have to be constructed.


  \todo Generator for PHP, and more generally for matching formulas,
  given an arbitrary graph and a set of vertices to be matched.
  Directly, and as active clause-sets.


  \todo %Generators for finding incidence structures and designs:
  Naturally modelled by boolean variables I(p, b) ("I" like
  "incident", "p" like "point", "b" like "block").
  Searching for example for balanced designs.
  Very simple the search for projective planes of order n:
  <ul>
   <li> n^2 + n + 1 p's and b's </li>
   <li> for all p <> p' there is exactly one b with I(p,b) and I(p',b) </li>
   <li> for all b <> b' there is exactly one p with I(p, b) and I(p, b') </li>
   <li> for all b there are two p, p' of p1,p2,p3,p4 (fixed variables!) with not I(p, b)
   and not I(p', b). </li>
  </ul>
  Modelled directly or via active clause-sets (especially that every two points
  are on one line, and every two lines intersect in one point!).
  QUESTION OK: Where does this topic belong to? To module LatinSquares?
  Or perhaps we should have a dedicated module on projective plans etc.?
  Sounds reasonable, but only after we really have something to start.


  \todo Very interesting are also the Extended Resolution extensions of PHP
  (and potentially other classes): Seeing the performance of SAT solvers
  on them would be interesting (or perhaps one can tune the SAT solvers
  to make good use of the extension clauses?!).


  \todo Transformers from non-boolean clause-sets to boolean
  clause-sets are needed (the standard translation, with or without
  the at-most-one-clauses, and also the other translations known from
  the literature; see [Prestwich; Local Search on SAT-encoded Colouring
  Problems; SAT 2003] for an overview, and also [Frisch,Peugniez;
  Solving Non-Boolean Satisfiability Problems with Stochastic Local
  Search; 2001]), and [Ansotegui,Manya; Mapping Problems with
  Finite-Domain Variables to Problems with Boolean Variables].


  \todo Implement the general construction of Soeren Riis (especially
  interesting when we know that the formulas have short refutations).

*/

/*!
  \namespace OKlib::Generators
  \brief Generators for (active) clause-sets (which are not yet in their own module).
*/

namespace OKlib {
  namespace Generators {
  }
}

