// Oliver Kullmann, 5.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp
  \brief Plans for Maxima-generators related to Sudoku-like problems


  \todo Relations to other modules
  <ul>
   <li> Compare with Applications/LatinSquares/plans/Sudoku.hpp (plans
   at C++ level). </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/LatinSquares.hpp. </li>
   <li> See the investigations in
   Experimentation/Investigations/LatinSquares/Sudoku/plans/general.hpp. </li>
  </ul>


  \todo Implement the four mixed cases for the direct translation
  <ul>
   <li> See "Translations to boolean CNFs based on the direct encoding" in
   Experimentation/Investigations/LatinSquares/Sudoku/plans/general.hpp. </li>
   <li> Write tests for the mixed forms (weakpl_sdk_fcl, weakpb_sdk_fcl,
   dual_weakpl_sdk_fcl, dual_weakpb_sdk_fcl). </li>
   <li> Write statistics functions for the mixed forms. </li>
  </ul>


  \todo Further improve implementation
  <ul>
   <li> Compare "Systematisation" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/LatinSquares.hpp. </li>
   <li> One should try whether using "create_list" in sdk_different_boxes,
   without storing the intermediate value, really slows down the computation
   (possibly the optimisation is detected?). </li>
  </ul>


  \todo Random Sudoku problems
  <ul>
   <li> For the creation of random Sudoku-problems the easiest approach
   seems to fix the number f of fixed fields, and then choose f random
   fields from the N^2 fields, and for each chosen field choose a random
   value from {1,...,N}. In other words, using the natural notion of
   (non-boolean) literals a random problem is just given by a random
   partial assignment with a fixed number of f variables.
    <ol>
     <li> Such random Sudoku-problems should also undergo a phase transition
     (for fixed p and appropriate f) ?! </li>
     <li> Creation of the partial assignment (with non-boolean variables)
     simply as a random clause by the OKgenerator. </li>
     <li> For the analysis one can filter out partial assignments which
     are not r_k-consistent (so r_0-consistency just means that the
     partial assignment does not (directly) falsify a clause); no
     consistency check then would be "-1-consistency". </li>
     <li> To speed up the creation, filtering out of r_k-inconsistent
     partial assignments can be done already during the creation (without
     disturbing the probability distribution). </li>
     <li> However, even r_1-reduction for box-dimension 2 takes a long
     time in Maxima (90 s). Thus this filtering cannot be done in Maxima,
     but we need C++ components. </li>
    </ol>
   </li>
   <li> It seems however, that the most basic version should already exclude
   trivial inconsistencies:
    <ol>
     <li> Thus best is likely to start with the list of all possible pairs
     [field, value]. </li>
     <li> A random element is chosen, and this field together with the fields
     with the same value in the same line is removed. </li>
     <li> This process is repeated. </li>
     <li> This yields a different random process (fields with many values have
     a higher probability for being chosen), but this process is also natural.
     </li>
    </ol>
   </li>
  </ul>


  \todo Enumerating all solutions
  <ul>
   <li> Likely this should go to a submodule of module
   CombinatorialMatrices/Lisp/LatinSquares. </li>
   <li> Apparently for box-dimension 2 we have 288 solutions; this should be
   computed and made available. </li>
   <li> There are 576 many latin squares of order 4; given that they are
   derived from just 4 reduced latin squares of order 4, it should be
   easy to make them available, and then to filter out the sudokus amongst
   them. </li>
  </ul>


  \todo Sampling of all solutions
  <ul>
   <li> Likely this should go to a submodule of module
   CombinatorialMatrices/Lisp/LatinSquares. </li>
   <li> A solution is just a total assignment (fulfilling the
   Sudoku-constraints. </li>
   <li> See Satisfiability/Enumeration/plans/general.hpp. </li>
   <li> A natural approach seems to be to create random instances,
   remove the unsatisfiable ones, and pick a solution.
    <ol>
     <li> Just picking the first solution might lead to a bias? </li>
     <li> So we could try to pick the second, third, ... solution. </li>
     <li> Perfectly, we would enumerate all solutions and pick one at
     random. But this is likely infeasible in most cases. </li>
     <li> But sampling the solution space of a problem instance
     might be more feasible. </li>
     <li> Of course, one could apply this approach just to the
     problem given by the empty partial assignment, and obtain in this
     way all solutions. </li>
     <li> However it might be worthwhile to first restrict the solution
     space. </li>
    </ol>
   </li>
   <li> Another approach just uses "random self-reduction":
    <ol>
     <li> Start with the empty partial assignment. </li>
     <li> Create the list of all pairs (empty field)/values. </li>
     <li> Remove the obviously unsatisfiable extensions by one such pair. </li>
     <li> Choose a random pair from the list, and check whether this extension
     is still satisfiable; if not, remove from the list, and choose another
     pair (as long as the list is not empty, there must exist a satisfiable
     extension). </li>
     <li> Repeat until a full solution is obtained. </li>
     <li> The question is whether this process yields perfect sampling of all
     solutions? One should check this for box-dimension 2. </li>
     <li> This process is similar to the second process above for creating
     random Sudoku problems, only that above no satisfiability-cheque is
     performed. </li>
    </ol>
   </li>
  </ul>


  \todo Sampling of minimally uniquely satisfiable problems
  <ul>
   <li> Here the task is to sample (count, enumerate etc.) all Sudoku problems
   which are minimally uniquely satisfiable (they are uniquely satisfiable,
   and removing any assignment creates further solutions). </li>
   <li> So "minimal" here is related to the partial assignment (and not
   to the problem instance). </li>
   <li> See http://magictour.free.fr/suexco.txt, where the generator "suexg"
   is described (also copied into this directory). </li>
   <li> Regarding *minimum* uniquely satisfiable problems, that is, where the
   partial assignment has minimum size, the conjecture is that this size is 17;
   all known non-isomorphic cases are made available by Gordon Royle at
   http://units.maths.uwa.edu.au/~gordon/sudokumin.php . </li>
   <li> The natural approach is to create a random solution (see "Sampling of
   all solutions" above), which is trivially uniquely satisfiable and then
   randomly remove assignments (i.e., literals) which can be removed without
   creating further solutions, until we obtain a minimally uniquely satisfiable
   problem.
    <ol>
     <li> DONE (see minUNISAT in
     Satisfiability/Lisp/Generators/plans/PlantedSolutions.hpp)
     We should create a general procedure, which starts with some unique
     solution phi (could be a partial assignment in general), and then picks
     variables by some heuristics and removes them from phi. </li>
     <li> Using the boolean literals, in general we get a "fractured Sudoku
     instance", where fields have forbidden values. </li>
     <li> But if we use just a positive partial assignment, which only states
     the occupied fields (not the other forbidden values, which follow by
     UCP), then actually we get standard Sudoku problems, and don't need
     to use negatively monosigned literals. </li>
     <li> For Sudoku it should be most natural to start with some given
     total solution (represented by a partial assignment containing
     only positive literals). </li>
     <li> Interesting questions here are what heuristics to use for removing
     a variable (if there are several choices), and whether the solution we
     start with makes a difference. </li>
     <li> The first objective could be to use the OKsolver-2002, and to first
     maximise the number of nodes in the tree, second the number of
     2-reductions. </li>
     <li> There should be instances which are not completely decided by
     2-reduction; but we can also try maximise the number of 2-reductions
     for those instances decided by 2-reduction (where the OKsolver-2002
     only needs one node). </li>
     <li> For specification purposes the Maxima functions are fine (of course),
     but they will be too slow, so that later a little C++ program is needed.
     </li>
    </ol>
   </li>
   <li> Is the discussion and implementation of a Sudoku generator at 
   http://skas-blog.blogspot.com/2007/07/end-of-sudoku-road.html
   relevant for us?
    <ol>
     <li> To me (OK) this looks rather amateurish; and, of
     course, completely ignored is the main perspective for us, that
     of generalised Sudoku as an NP-complete problem. </li>
     <li> In those discussions the "hardness" of a problem always plays
     a role; I think a good way to measure it is the least k such that
     r_k yields the solution (this only works for unique solutions;
     otherwise generalised reductions have to be used). </li>
    </ol>
   </li>
  </ul>


  \todo Sudoku generator for the Minion constraint solver.
  <ul>
   <li> This should be placed into a module for constraint satisfaction. </li>
   <li> What is the meaning of "generator" here? Is this just a file-format,
   or, more meaningfully, do we first create a constraint-satisfaction-problem,
   using a suitable Maxima-representation of "constraints", and then output
   this in Minion-format? </li>
   <li> Parameters: n (box size), N = n^2, M = N^2  </li>
   <li> Cells variables : {x_1,...,x_M}. dom(x_i) = {1,...,N} for 
   i = 1...M. x_i = j means j is assigned to cell i. </li>
   <li> Boxes are represented by vectors v_i (i = 1..N) of cell 
   variables. </li>
   <li> The Sudoku then consists of a single N x N matrix of cell variables.
   </li>
   <li> %Implementation: 
    <ul>
     <li> Row constraints:
     \verbatim
alldifferent(row(m_0),i)
     \endverbatim
     for i = 1..N  
     </li>
     <li> Column constraints: 
     \verbatim
alldifferent(col(m_0,i))
     \endverbatim
     for i = 1..N
     </li>
     <li> Box constraints:
     \verbatim
alldifferent(v_i)
     \endverbatim
     for i = 1..N
     </li>
    </ul>
   </li>
   <li> Syntax:
   (what syntax?? the file-content?)
   \verbatim
sudoku_minion_format(boxsize, filename)
   \endverbatim
   </li>
  </ul>
 
*/

