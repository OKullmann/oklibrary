// Oliver Kullmann, 5.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/Sudoku.hpp
  \brief Plans for Maxima-generators related to Sudoku-like problems


  \todo Relations to other modules
  <ul>
   <li> Compare with Applications/LatinSquares/plans/Sudoku.hpp. </li>
  </ul>


  \todo Formulating "Sudoku problem instances"
  <ul>
   <li> We need an easy method for specifying partial assignments to
   fields. </li>
   <li> Likely a "Sudoku problem instance" is just given by a partial
   assignments to the variables. </li>
   <li> However this would be most natural for variables with domain
   {1,...,N}. Using the boolean variables corresponds to the use of
   signed literals (where we can enable/disable specific values for
   for specific fields). </li>
   <li> So, there are different forms of Sudoku-problems for different types
   of literals used by the partial assignment. </li>
  </ul>


  \todo Improve implementation
  <ul>
   <li> One should try whether using "create_list" in sdk_different_boxes,
   without storing the intermediate value, really slows down the computation
   (possibly the optimisation is detected?). </li>
   <li> DONE Can something be done about a stack-overflow for
   output_strong_sdk(6,"~/sdk_s_6.cnf") on csltok? Works on cs-wsok, and so
   it just seems to be a question of main memory (though csltok has 2GB). </li>
   <li> Surjectivity constraints: DONE
    <ol>
     <li> For rows, columns, and boxes translate the surjectivity
     constraints. </li>
     <li> Create a "dual weak sdk". </li>
     <li> The union of weak sdk and dual weak sdk then deserves the
     name "strong sdk". </li>
    </ol>
   </li>
   <li> Add the conditions, that a field doesn't get several numbers.
   DONE. </li>
   <li> DONE (except of sdk_different_boxes, where it doesn't seem possible
   to store an intermediate value)
   Replace, if possible, all loops by the use of "create_list". </li>
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
  </ul>


  \todo Sampling of all solutions
  <ul>
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
     <li> However it might be worthwile to first restrict the solution
     space. </li>
    </ol>
   </li>
  </ul>


  \todo Sampling of minimally uniquely satisfiable problems
  <ul>
   <li> Here the task is to sample (count, enumerate etc.) all Sudoku problems
   which are minimally uniquely satisfiable (they are uniquely satisfiable,
   and removing any assignments creates further solutions). </li>
   <li> The natural approach is to create a random solution (see "Sampling of
   all solutions" above), which is trivially uniquely satisfiable and then
   randomly remove assignments (i.e., literals) which can be removed without
   creating further solutions, until we obtain a minimally uniquely satisfiable
   problem. </li>
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
   <li> What is the meaning of "generator" here? Is this just a file-format, or,
   more meaningfully, do we first create a constraint-satisfaction-problem,
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
 

  \todo %Generators for Latin square completion problems
  <ul>
   <li> Compare Applications/LatinSquares/plans/general.hpp. </li>
  </ul>
   
*/

