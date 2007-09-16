// Oliver Kullmann, 1.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/plans/OKsolver_2_0.hpp
  \brief Plans for the module for the new OKsolver;
  the goal is that OKsolver_2 is among the best three solvers
  for all 3 * 3 subcompetitions of the SAT 2009 competition.


  \todo Once we are ready to start, a new sub-module is needed.


  \todo Update namespaces


  \todo Planing:
  Set up milestones etc.


  \todo How to refer to the different versions of OKsolver ?
  Perhaps
   - OKsolver_1.0, OKsolver_1.1
   - OKsolver_2, OKsolver_2.0, OKsolver_2.1 ?


  \todo OKsolver_2 extends OKsolver_1 by using alliances of
  active clause-sets with (at least) three members :
   - the original clause-set
   - the active clause-set for compressed r_2-learning
     (see module Learning)
   - the active clause-set for equivalence reasoning
     (see module Equivalences).


  \todo Furthermore, the "original clause-set" is split into active clause-sets with
   - the 2-clauses
   - the 3-clauses
   - the >=4-clauses.
   The active clause-set for 2-clauses handles r_2 (and strengthening), and extracts equivalences
   (passed to the active clause-set for equivalence reasoning).
   The active clause-set for 3-clauses  combines all clauses with the same variable-set,
   and extracts 2-clauses and equivalences.


  \todo An interesting option is to compress learned clauses further (using
  for example r_3); see module Learning : "After-burner".


  \todo Furthermore we perform reductions on the input (at
  least r_1, subsumption elimination and equivalence
  extraction; perhaps some forms of DP-reductions; and some
  forms of autarky reductions).


  \todo For failed literal reductions and strengthenings see the module "FailedLiteralReduction".


  \todo It would be good if instead of just using r_2 we could plug in other (stronger) forms
  (like r_3 or r_2 with local learning).


  \todo Regarding equivalence reasoning, the minimum is the detection of (a or b) and (not a or not b),
  with subsequent replacement of not a by b (or not b by a --- just go for the smaller substitution
  effort). Since "equivalence reasoning" is "equality reasoning", in general we obtain equalities between
  literals, which are "performed" by substitution. For (clause) learning purposes these substitution can 
  be ignored, while for the reconstruction of satisfying assignments bookkeeping is necessary (as well as
  for determining the set of variables used in the resolution refutation under construction). And also when trying
  to push autarkies found to higher levels in the search tree, the substitutions applied have to be
  considered (the autarkies have to be extended by the substituted variables).


  \todo For the right-most branch in the search tree we can apply partial assignment "destructively"
  (irreversible). Potentially the memory savings (if returned to the pool) are considerable.
  \todo This brings up the question of memory management: The Boost Pool library must be
  investigated.


  \todo Autarky search (see module AutarkySearch) is integrated with global learning via the
  fundamental duality between resolution and autarkies: The global
  learning component manages the set of variables used in the current resolution
  tree fragment (for this node(!)), and at every node of the search tree after reduction this set of
  variables is crossed out (from the current clause-set, and removing the empty clause)
  and a local search solver is employed on the resulting clause-set (searching for
  a satisfying assignment --- these are autarkies for the current clause-set).
  Autarky search is like learning (global or local) the responsibility of the overall
  algorithm (managing the alliance of active clause-sets), and all members of the
  alliance are taken into account (this emphasis the requirement, that the members
  are active *clause-sets*, "faking" to be clause-sets via returning appropriate statistics).


  \todo Likely, autarky search should ignore learned clauses (when applying the autarky,
  learned clauses might get shortened).


  \todo So at each node a satisfiability problem is attacked by incomplete methods: Mainly
  local search, but one could also use additional measures (like reductions), which are
  especially appropriate once variables are being crossed out.


  \todo There are two points when autarky search can (and shall) be applied:
   - first before entering the first branch (no variables are crossed out);
   - then before entering the second branch (if no tree pruning can be
     applied; now the resolution variables from the first branch are crossed out);
   After returning from the second branch (now the sub-formula has found to
     be unsatisfiable, and all the variables used by the resolution refutation are
     crossed out) it doesn't make sense anymore to search for an autarky (since we
     solved the sub-problem, and it seems from finding an autarky we don't gain
     anything for the remaining problems).


  \todo Autarkies can also be checked whether they extend further up the search tree (in the best
  of all cases they yield, composed together with the corresponding partial assignments, global
  autarkies). A classical situation, exploited in the Luckhardt
  algorithm is when the autarky only leaves clauses of length equal to the maximal clause
  length in the input. Marijn Heule had a trick here? Perhaps one marks the new clauses
  with the creation levels --- then one can go up until before the first level where new clauses
  are left. Marijn used these levels for his locality principle (the new branching variable must
  shorten at least one clause from the previous level).


  \todo Even if the local search algorithm only searches for a satisfying assignment, perhaps it's
  still worth trying in case no satisfying assignment was found if the assignment nevertheless
  contained an autarky (see AutarkySearch/plans/AnalyseTotalAssignment.hpp).


  \todo The heuristics for the branching variable is a  main open problem (while
  choosing the first branch is done like in the old OKsolver). Here likely a lot of
  experimentation is needed (starting with the heuristics from the old OKsolver).
  But basically we follow the old OKsolver, performing a full look-ahead, and choosing
  the best-looking future situation; likely this look-ahead should only involve unit-propagations
  --- it would be nice to do something stronger, but where to stop? The components should
  allow to do stronger things in the look-ahead, so that we can experiment.
  The simplest (and quite good) criterion for the "best-looking future situation" is
  the weighted number of new clauses (as in OKsolver); an interesting question here is
  whether new (that is, shortened) clauses from learned clauses should have a higher weight
  than new clauses from old clauses?!


  \todo Another main problem is the choice of the local search algorithm and the settings
  of its parameters:
   - Are some algorithms more suitable than others ?!
   - How to integrate the data structures ?!
   - How to choose the parameters for the local search ?! A canonical first choice for the
     number of random assignments is 1; the number of rounds then needs to fiddled.
     However a canonical choice here would be to run the local search until it reaches
     its first local optimum. Only doing this would likely simplify the data structures.
     Makes sense to me: In this way we take from the local search only "the best part".
     Furthermore we only search for a local move until we find an improvement; in this way
     we need only to search through the whole space of local moves in the last step (where no
     further improvement is possible). We also require a positive improvement, avoiding
     in this way any kind of book keeping about repeated moves.
   So everything boils down here to the choice of possible moves, to the choice of the
   optimisation function, and to the choice of the data structures.
   And possibly there is the question about the random assignment: Every variable 1/2 ?
   Or something else? Or some weights?? Yet I don't see something here, so we just
   use the plain form of random assignments.


  \todo If the "total" assignment computed is not satisfying, then still one can see whether
  it contains an autarky (as explained in AutarkySearch/plans).


  \todo The final algorithmic aspect is the restart policy: Somehow a "hopeless" situation
  has to be figured out, gathering then everything worthwhile learning and building
  a new initial problem (an alliance of active clause-sets) with which we start again:
  We should only restart if at least one clause has been learnt; then the partial assignment
  is emptied (backtracking to level zero), initial reductions are applied if possible,and we
  start again --- some members of the alliance will have been strengthened.
  Progress measurements are important: Every search component
  must somehow estimates its progress, so that a global statistic is possible. Also from
  restart to restart a memory must be kept, so that one can see whether there is overall
  progress (or not). The most fundamental statistics here seems to be the proportion
  of the search space already covered; see Statistics/TimeSeriesAnalysis.hpp. With this
  measure we should be able to get some evaluation of the overall progress and restart
  if progress is too meager (compared to some envisaged running time).


  \todo Likely for OKsolver_2_0 the simplest prediction strategy should be used (as
  outlined in Statistics/TimeSeriesAnalysis.hpp), which just takes progress as constant
  over time. This leaves open the question about the restart policy. Most basic seems
  the following possibility: Given the total available time T, the number k of restart
  decisions, and the threshold alpha, for i = 1, ..., k at time T * i/(k+1) the prediction
  is computed, and if this is more than alpha * T (for some alpha >= 1), and something
  has been learned, then we restart. One could make alpha varying over time, starting with
  alpha = 3 for example, and then letting it go to 1, reflecting that over time the precision
  of the measurements increase, while the matter becomes more urgent. There must also
  be a threshold for what constitutes a substantial enough amount of learning so that
  a restart seems reasonable (to avoid stammering). Of course, one could use randomisation,
  but yet I don't see a sensible way to do so, and so perhaps we leave it out (or leave it for
  OKsolver_3_0).


  \todo If restarts don't yield clear benefits, then they are not incorporated into
  OKsolver_2_0, but the whole issue is thoroughly treated with OKsolver_3_0.


  \todo The above resource management strategies use prediction; another possibility
  are the monitoring schemes similar to those investigated by Heule and van Maaren:
  Considering some observation level d in the search tree and a given total time R,
  the time per observation node is R / 2^d. If the time is out, then the search is
  interrupted, and search jumps to next node on the observation level, mainly
  in the hope to find a satisfying assignment there, but also good learned clauses
  could be obtained there. Since mainly we hope for the satisfying assignment,
  and we assume that the first branch in general is better than the second branch,
  and furthermore we assume that deeper down the tree the prediction of the
  first branch becomes better, the rules for determining the next search node
  are just 1) left before right 2) assume the "error" as far up as possible.
  The old search nodes can be stored via their partial assignments (possibly only
  the decision variables). If we tried all nodes at the monitoring level, and time
  remains, then for the remaining nodes we compute the time available, and
  repeat the procedure.
  An option here is of course to use parallelisation (could be wasteful, if for example
  the first branch is really better than the second one).
  One could also use processes/threads just to implement the jumping (in case of
  learning all these processes/threads need to send the learned clauses to a central control.


  \todo All the parameter settings depend on the input distribution, and so we should
  learn good parameter values by evaluating the SAT 2005 competition. The conceptually
  simplest way to achieve this is to optimise the score the solver would have achieved
  in the SAT 2005 competition. Since this needs running the solver on the whole
  competition many times, we need decent computational resources.

*/

/*!
  \namespace OKlib::OKsolver_2_0
  \brief Applications implementing the new OKsolver (for SAT 2009)
*/

namespace OKlib {
  namespace OKsolver_2_0 {
  }
}

