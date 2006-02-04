// Oliver Kullmann, 1.2.2006 (Swansea)

/*!
  \file OKsolver_2_0_Plans.hpp
  \brief Plans for the module for the new OKsolver;
  the goal is that OKsolver_2 is among the best three solvers
  for all 3 * 3 subcompetitions.
  \todo OKsolver_2 extends OKsolver_1 by using alliances of
  active clause-sets with (at least) three members :
   - the original clause-set
   - the active clause-set for compressed r_2 learning
     (see module Learning)
   - the active clause-set for equivalence reasoning
     (see module Equivalences).
  \todo Furthermore we perform reductions on the input (at
  least r_1, subsumption elimination and equivalence
  extraction; perhaps some forms of DP-reductions; and some
  forms of autarky reductions).
  \todo Autarky search is integrated with global learning via the
  fundamental duality between resolution and autarkies: The global
  learning component manages the set of variables used in the current resolution
  tree fragment (for this node(!)), and at every node of the search tree after reduction this set of
  variables is crossed out (from the current clause-set, and removing the empty clause)
  and a local search solver is employed on the resulting clause-set (searching for
  a satisfying assignment --- these are autarkies for the current clause-set).
  Autarky search is like learning (global or local) the responsibility of the overall
  algorithm (managing the alliance of active clause-sets), and all members of the
  alliance are taken into account (this emphasis the requirement, that the members
  are active *clause-sets*, "faking" to be clause-sets via returning appropriate statistics.
  \todo So at each node a satisfiability problem is attacked by incomplete methods: Mainly
  local search, but one could also use additional measures (like reductions), which are
  especially appropriate once variables are being crossed out.
  \todo There are actually two points when autarky search can be applied:
   - first before entering the first branch (no variables are crossed out);
   - and then before entering the second branch (if no tree pruning can be
     applied; now the resolution variables from the first branch are crossed out).
  \todo The heuristics for the branching variable is a  main open problem (while
  choosing the first branch is done like in the old OKsolver). Here likely a lot of
  experimentation is needed (starting with the heuristics from the old OKsolver).
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
  \todo The final algorithmic aspect is the restart policy: Somehow a "hopeless" situation
  has to be figured out, gathering then everything worthwhile learning and building
  a new initial problem (an alliance of active clause-sets) with which we start again.
  This is intertwined with the problem of how much resources should be allocated to
  the autarky search. Progress measurements are important: Every search component
  must somehow estimates its progress, so that a global statistic is possible. Also from
  restart to restart a memory must be kept, so that one can see whether there is overall
  progress (or not). The most fundamental statistics here seems to be the proportion
  of the search space already covered; see Statistics/TimeSeriesAnalysis.hpp. With this
  measure we should be able to get some evaluation of the overall progress and restart
  if progress is too meager.
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

