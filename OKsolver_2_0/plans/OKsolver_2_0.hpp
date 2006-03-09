// Oliver Kullmann, 1.2.2006 (Swansea)

/*!
  \file OKsolver_2_0/plans/OKsolver_2_0.hpp
  \brief Plans for the module for the new OKsolver;
  the goal is that OKsolver_2 is among the best three solvers
  for all 3 * 3 subcompetitions of the SAT 2007 competition.

  \todo OKsolver_2 extends OKsolver_1 by using alliances of
  active clause-sets with (at least) three members :
   - the original clause-set
   - the active clause-set for compressed r_2-learning
     (see module Learning)
   - the active clause-set for equivalence reasoning
     (see module Equivalences).

  \todo An interesting option is to compress learned clauses further (using
  for example r_3); see module Learning : "After-burner".

  \todo Furthermore we perform reductions on the input (at
  least r_1, subsumption elimination and equivalence
  extraction; perhaps some forms of DP-reductions; and some
  forms of autarky reductions).

  \todo It would be good if instead of just using r_2 we could plug in other (stronger) forms
  (like r_3 or r_2 with local learning).

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

  \todo The heuristics for the branching variable is a  main open problem (while
  choosing the first branch is done like in the old OKsolver). Here likely a lot of
  experimentation is needed (starting with the heuristics from the old OKsolver).
  But basically we follow the old OKsolver, performing a full look-ahead, and choosing
  the best-looking future situation; likely this look-ahead should only involve unit-propagations
  --- it would be nice to do something stronger, but where to stop? The components should
  allow to do stronger things in the look-ahead, so that we can experiment.

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
     further improvement is possible). We also require a postive improvement, avoiding
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

  \todo All the parameter settings depend on the input distribution, and so we should
  learn good parameter values by evaluating the SAT 2005 competition. The conceptually
  simplest way to achieve this is to optimise the score the solver would have achieved
  in the SAT 2005 competition. Since this needs running the solver on the whole
  competition many times, we need decent computational resources.
*/

/*!
  \namespace OKlib::OKsolver_2_0
  \brief Applications implementing the new OKsolver (for
  SAT 2007)
*/

namespace OKlib {
  namespace OKsolver_2_0 {
  }
}

