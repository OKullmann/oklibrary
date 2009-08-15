// Oliver Kullmann, 1.2.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/plans/OKsolver_2_0.hpp
  \brief Plans for the module for the new OKsolver

  Possibly called "OKsolver2010".


  \todo Once we are ready to start, a new sub-module is needed.


  \todo Update namespaces


  \todo Planing:
  Set up milestones etc.


  \todo Goals
  <ul>
   <li> The old goals were: "OKsolver_2 is among the best three solvers
   for all 3 * 3 sub-competitions of the SAT 2009 competition". </li>
   <li> However, because of the overheads incurred by the planned integration
   of the 3 solver paradigms, where the SAT competition is geared towards
   speed (giving only a bit of time on each instance), perhaps better is
   to seek a solver which can crack "really hard" instances from all
   the 3 * 3 sub-competitions (i.e., over the full range of instances). </li>
   <li> A lot of experimentation will be needed. Perhaps first we go for
   an "experimental suite" ? </li>
  </ul>


  \todo OKsolver_2 extends OKsolver_1 by using alliances of
  active clause-sets with (at least) three members :
  <ol>
   <li> the original clause-set </li>
   <li> the active clause-set for compressed r_2-learning (see
   module Learning, Satisfiability/Algorithms/Learning/plans/general.hpp).
   </li>
   <li> the active clause-set for equivalence reasoning (see
   module Equivalences). </li>
  </ol>


  \todo Further sub-division of the instance
  <ul>
   <li> The "original clause-set" is split into active clause-sets with
    <ol>
     <li> the 2-clauses </li>
     <li> the 3-clauses </li>
     <li> the >=4-clauses. </li>
    </ol>
   </li>
   <li> The active clause-set for 2-clauses handles r_2 (and strengthening),
   and extracts equivalences (passed to the active clause-set for equivalence
   reasoning). </li>
   <li> The active clause-set for 3-clauses combines all clauses with the
   same variable-set, and extracts 2-clauses and equivalences. </li>
  </ul>


  \todo After-burner for learning
  <ul>
   <li> An interesting option is to compress learned clauses further (using
   for example r_3); see module Learning
   (Satisfiability/Algorithms/Learning/plans/general.hpp): "After-burner".
   </li>
   <li> A related question is about "Re-working the tree" (see there).
    <ol>
     <li> According to the general guideline of avoiding "pure heuristics", we
     should always rework the tree. </li>
     <li> However, similar to the experience with r_2 and OKsolver2002, where
     completing a round of reduction was found to be more efficient then
     immediate restart of the reduction process, it is quite conceivable that
     the solver starts "stammering" due to all this clean-up. </li>
    </ol>
   </li>
   <li> As discussed in OK's article [Present and future of practical SAT
   solving, Oliver Kullmann, 2008], the view of learning as still working
   with a tree might be misleading, and an iterative view more appropriate.
   But the tree-view corresponds to the "look-ahead part", and thus cannot
   be ignored. </li>
  </ul>


  \todo Preprocessing
  <ul>
   <li> We perform reductions on the input (at
   least r_1, subsumption elimination and equivalence
   extraction; perhaps some forms of DP-reductions; and some
   forms of autarky reductions). </li>
  </ul>


  \todo Reductions
  <ul>
   <li> For failed literal reductions and strengthenings see the
   module "FailedLiteralReduction". </li>
   <li> It would be good if instead of just using r_2 we could plug in
   other (stronger) forms (like r_3 or r_2 with local learning). </li>
   <li> The reduction should solve 2-CNF efficiently. </li>
   <li> Perhaps the only other class of "easy problems" handled by the
   reduction are "small formulas", using special algorithms for small
   formulas, where "small" is the threshold where these simple algorithms
   don't have an advantage anymore. </li>
  </ul>


  \todo Equivalence reasoning
  <ul>
   <li> The minimum is the detection of (a or b) and (not a or not b), with
   subsequent replacement of not a by b (or not b by a --- just go for the
   smaller substitution effort). </li>
   <li> Since "equivalence reasoning" is "equality reasoning", in general we
   obtain equalities between literals, which are "performed" by substitution.
   </li>
   <li> For (clause) learning purposes these substitution can be ignored, while
   for the reconstruction of satisfying assignments bookkeeping is necessary
   (as well as for determining the set of variables used in the resolution
   refutation under construction). </li>
   <li> And also when trying to push autarkies found to higher levels in the
   search tree, the substitutions applied have to be considered (the autarkies
   have to be extended by the substituted variables). </li>
   <li> The best solution is to to introduce equivalence literals "a=b". </li>
  </ul>


  \todo Memory management
  <ul>
   <li> For the right-most branch in the search tree we can apply partial
   assignment "destructively" (irreversible). </li>
   <li> Potentially the memory savings (if returned to the pool) are
   considerable. </li>
   <li> This brings up the question of memory management: The Boost Pool
   library must be investigated. </li>
  </ul>


  \todo Autarky search
  <ul>
   <li> Autarky search (see module Autarkies/Search) is integrated with global
   learning via the fundamental duality between resolution and autarkies: The
   global learning component manages the set of variables used in the current
   resolution tree fragment (for this node(!)), and at every node of the
   search tree after reduction this set of variables is crossed out
   (from the current clause-set, and removing the empty clause) and a local
   search solver is employed on the resulting clause-set (searching for a
   satisfying assignment --- these are autarkies for the current clause-set).
   </li>
   <li> Autarky search is like learning (global or local) the responsibility
   of the overall algorithm (managing the alliance of active clause-sets), and
   all members of the alliance are taken into account (this emphasis the
   requirement, that the members are active *clause-sets*, "faking" to be
   clause-sets via returning appropriate statistics). </li>
   <li> Likely, autarky search should ignore learned clauses (when applying the
   autarky, learned clauses might get shortened). </li>
  </ul>


  \todo Incomplete methods
  <ul>
   <li> So at each node a satisfiability problem is attacked by incomplete
   methods: Mainly local search, but one could also use additional measures
   (like reductions), which are especially appropriate once variables are
   being crossed out. </li>
  </ul>


  \todo Organising autarky search
  <ul>
   <li> There are two points when autarky search can (and shall) be applied:
    <ol>
     <li> first before entering the first branch (no variables are crossed
     out); </li>
     <li> then before entering the second branch (if no tree pruning can be
     applied; now the resolution variables from the first branch are crossed
     out). </li>
    </ol>
    After returning from the second branch (now the sub-formula has found to
    be unsatisfiable, and all the variables used by the resolution refutation
    are crossed out) it doesn't make sense anymore to search for an autarky
    (since we solved the sub-problem, and it seems from finding an autarky we
    don't gain anything for the remaining problems).
   </li>
  </ul>


  \todo Autarky extension
  <ul>
   <li> Autarkies can also be checked whether they extend further up the search
   tree (in the best of all cases they yield, composed together with the
   corresponding partial assignments, global autarkies). </li>
   <li> A classical situation, exploited in the Luckhardt algorithm is when the
   autarky only leaves clauses of length equal to the maximal clause length in
   the input. </li>
   <li> Marijn Heule had a trick here?
    <ol>
     <li> Perhaps one marks the new clauses with the creation levels --- then
     one can go up until before the first level where new clauses are left.
     </li>
     <li> Marijn used these levels for his locality principle (the new
     branching variable must shorten at least one clause from the previous
     level). </li>
    </ol>
   </li>
  </ul>


  \todo More autarky search
  <ul>
   <li> Even if the local search algorithm only searches for a satisfying
   assignment, perhaps it's still worth trying in case no satisfying
   assignment was found if the assignment nevertheless contained an autarky
   (see Autarkies/Search/plans/AnalyseTotalAssignment.hpp). </li>
  </ul>


  \todo Heuristics
  <ul>
   <li> The heuristics for the branching variable is a main open problem.
    <ol> 
     <li> Perhaps the central problem is, that for a look-ahead heuristics we
     look at *both* branches (in the boolean case), which if not a satisfying
     assignment were found, both will be executed. </li>
     <li> So we need to balance both branches. </li>
     <li> However for a conflict-driven solver the second branch happens
     under different circumstances, having learned clauses added. And
     potentially, due to non-chronological backtracking, the second branch
     doesn't happen at all. </li>
    </ol>
   </li>
   <li> Choosing the first branch perhaps is still done like in the old
   OKsolver, seeking for a satisfying assignment. (The alternative is to seek
   for "good resolutions" ?) </li>
   <li> Here likely a lot of experimentation is needed (starting with the
   heuristics from the old OKsolver). </li>
   <li> But basically we follow the old OKsolver, performing a full
   look-ahead, and choosing the best-looking future situation; likely this
   look-ahead should only involve unit-propagations --- it would be nice to do
   something stronger, but where to stop? </li>
   <li> The components should allow to do stronger things in the look-ahead,
   so that we can experiment. </li>
   <li> The simplest (and quite good) criterion for the "best-looking future
   situation" is the weighted number of new clauses (as in OKsolver); an
   interesting question here is whether new (that is, shortened) clauses from
   learned clauses should have a higher weight than new clauses from old
   clauses?! </li>
   <li> The new theory, based on my (OK's) SAT-Handbook article and handled in
   module Satisfiability/Heuristics, should be applied! See module
   Satisfiability/Heuristics/StatisticalAnalysis. </li>
   <li> In analogy to the activity-based heuristics for conflict-driven
   solvers, we can consider such quantities for variable v, where we have now
   richer possibilities:
    <ol>
     <li> The number of learned clauses containing v; since we go "far back",
     it might be appropriate here to consider v's involvement in the whole
     resolution process leading to the learned clause in the end. </li>
     <li> The number of unit clauses with variable v found during reduction.
     </li>
     <li> The number of failed literals with variable v. </li>
    </ol>
   The intuition is that if we can branch on a variable which was "recently"
   active in a lot of inferences, then this variable might again provide
   a lot of action, a kind of culmination point, and it might be good to branch
   on it. To quantify "recent" one can use some decay scheme. </li>
   <li> A different approach takes the dynamic changes into account, where
   (as discussed above) the second branch might hapen under very different
   circumstances:
    <ol>
     <li> Instead of the "pessimistic" view, that both branches have to be
     balanced, we could take an "optimistic" (or "greedy") view, and just to
     seek for a *single* branch with the best prediction. </li>
     <li> One could also try to change the branching strategy over the tree:
      <ul>
       <li> One could say, that near the root the second branch will have
       a while to go, a lot will be learned until then, and so better we
       are being greedy. While towards the leaves changes might be smaller,
       and we balance more. </li>
       <li> On the other hand, balancing is most effective near the root ?
       So we could be more balancing towards the root, and the more we progress
       towards the leaves, the greedier we become? </li>
       <li> The latter proposal is in line with the view of having more
       breadth-first near the root (generalised look-ahead). </li>
       <li> This has to be tried out; it also depends on how we update (and
       (partially) restart). </li>
      </ul>
     </li>
     <li> The question is whether to use prediction for both branches, or
     to seek for high activity (just for one assignment). </li>
     <li> From the branching point of view, we compute a branching
     phi_1, ..., phi_k (covering all possibilities), and then we process
     the branches, where the only question is the order of branches. Now,
     under those dynamic changes, a conflict-driven solver doesn't want to
     be fixed to these decisions, but wants to re-evaluate them later. Since
     a look-ahead solver spents a lot of work on the decisions, it might be
     painful to throw them all over board. </li>
    </ol>
   </li>
  </ul>


  \todo Local search
  <ul>
   <li> Another main problem is the choice of the local search algorithm and
   the settings of its parameters:
    <ol>
     <li> Are some algorithms more suitable than others ?! </li>
     <li> How to integrate the data structures ?! </li>
     <li> How to choose the parameters for the local search ?! </li>
     <li> A canonical first choice for the number of random assignments is 1;
     the number of rounds then needs to fiddled. </li>
     <li> However a canonical choice here would be to run the local search
     until it reaches its first local optimum. Only doing this would likely
     simplify the data structures. </li>
     <li> Makes sense to me: In this way we take from the local search only
     "the best part". </li>
     <li> Furthermore we only search for a local move until we find an
     improvement; in this way we need only to search through the whole space
     of local moves in the last step (where no further improvement is
     possible). </li>
     <li> We also require a positive improvement, avoiding
     in this way any kind of book keeping about repeated moves. </li>
    </ol>
   </li>
   <li> So everything boils down here to the choice of possible moves, to the
   choice of the optimisation function, and to the choice of the data
   structures. </li>
   <li> And possibly there is the question about the random assignment: Every
   variable 1/2 ? </li>
   <li> Or something else? Or some weights?? Yet I don't see something here,
   so we just use the plain form of random assignments. </li>
  </ul>


  \todo Restart policy
  <ul>
   <li> Somehow a "hopeless" situation has to be figured out, gathering then
   everything worthwhile learning and building a new initial problem
   (an alliance of active clause-sets) with which we start again:
   We should only restart if at least one clause has been learnt; then the
   partial assignment is emptied (backtracking to level zero), initial
   reductions are applied if possible,and we start again --- some members of
   the alliance will have been strengthened. </li>
   <li> Progress measurements are important: Every search component
   must somehow estimates its progress, so that a global statistic is
   possible. </li>
   <li> Also from restart to restart a memory must be kept, so that one can see
   whether there is overall progress (or not). </li>
   <li> The most fundamental statistics here seems to be the proportion
   of the search space already covered; see Statistics/TimeSeriesAnalysis.hpp.
   </li>
   <li> With this measure we should be able to get some evaluation of the
   overall progress and restart if progress is too meager (compared to some
   envisaged running time). </li>
  </ul>


  \todo Time-prediction
  <ul>
   <li> Likely for OKsolver_2_0 the simplest prediction strategy should be
   used (as outlined in Statistics/TimeSeriesAnalysis.hpp), which just takes
   progress as constant over time. </li>
   <li> This leaves open the question about the restart policy. </li>
   <li> Most basic seems the following possibility: Given the total available
   time T, the number k of restart decisions, and the threshold alpha, for
   i = 1, ..., k at time T * i/(k+1) the prediction is computed, and if this
   is more than alpha * T (for some alpha >= 1), and something has been
   learned, then we restart. </li>
   <li> One could make alpha varying over time, starting with alpha = 3 for
   example, and then letting it go to 1, reflecting that over time the
   precision of the measurements increase, while the matter becomes more
   urgent. </li>
   <li> There must also be a threshold for what constitutes a substantial
   enough amount of learning so that a restart seems reasonable (to avoid
   stammering). </li>
   <li> Of course, one could use randomisation, but yet I don't see a sensible
   way to do so, and so perhaps we leave it out (or leave it for
   OKsolver_3_0). </li>
  </ul>


  \todo If restarts don't yield clear benefits, then they are not incorporated
  into OKsolver_2_0, but the whole issue is thoroughly treated with
  OKsolver_3_0.


  \todo Monitoring the search process
  <ul>
   <li> The above resource management strategies use prediction; another
   possibility are the monitoring schemes similar to those investigated by
   Heule and van Maaren:
    <ol>
     <li> Considering some observation level d in the search tree and a given
     total time R, the time per observation node is R / 2^d. If the time is
     out, then the search is interrupted, and search jumps to next node on the
     observation level, mainly in the hope to find a satisfying assignment
     there, but also good learned clauses could be obtained there. </li>
     <li> Actually, the scheme of Heule and van Maaren is simpler, since it
     always completes the computation for the branches encountered at level d.
     See below for furthe discussions. </li>
    </ol>
   </li>
   <li> Since mainly we hope for the satisfying assignment,
   and we assume that the first branch in general is better than the second
   branch, and furthermore we assume that deeper down the tree the prediction
   of the first branch becomes better, the rules for determining the next
   search node are just 1) left before right 2) assume the "error" as far up
   as possible. </li>
   <li> The old search nodes can be stored via their partial assignments
   (possibly only the decision variables). If we tried all nodes at the
   monitoring level, and time remains, then for the remaining nodes we compute
   the time available, and repeat the procedure. </li>
   <li> An option here is of course to use parallelisation (could be wasteful,
   if for example the first branch is really better than the second one). </li>
   <li> One could also use processes/threads just to implement the jumping (in
   case of learning all these processes/threads need to send the learned
   clauses to a central control. </li>
   <li> Actually, a better point of view seems to me to introduce an element
   of breadth-first search at the beginning:
    <ol>
     <li> The first goal is to determine the "frontier", a complete search tree
     where all nodes are reduced, and except of the leaves all splitting are
     complete; also all global learning and tree pruning has been performed.
     </li>
     <li> One hope is that within this frontier the problem was already
     decided. </li>
     <li> If this is not the case, then the leaves (all other nodes have been
     processed) are sorted with descending approximated probabilities of being
     satisfiable, and processed in this order. </li>
     <li> If a leaf at the frontier has been processed, then the search tree up
     to the frontier is updated (regarding tree pruning and global learning),
     and if necessary probabilities are recalculated and resorted (for the
     remaining leaves). </li>
     <li> The main problem is the determination of the frontier. </li>
     <li> The guiding principle should be, that we label each node with
     the time spent on processing it, and then for all leaves the sum
     of processing times on the path towards them is roughly the same. </li>
     <li> The point is that where progress is easily achieved the frontier
     should be pushed further. </li>
     <li> Given this equal spread of workload, the problem now is the total
     workload spend on the frontier (and the storage). </li>
     <li> A principle should be that the harder the problem appears, the
     more time is spent on this bread-first look-ahead. </li>
    </ol>
   </li>
   <li> However, this breadth-first approach uses quite some space (if we
   store what we have already computed, which seems reasonable) and time,
   and thus the depth of the frontier is rather small. So perhaps we could
   add the distribution-jumping strategy of Heule/Maaren, perhaps best as
   mentioned above by a resource-bound (and not necessarily, as done by them,
   completely finishing the node --- which of course needs less management,
   since we do not need to keep the old nodes around). </li>
  </ul>


  \todo Parameter settings
  <ul>
   <li> All the parameter settings depend on the input distribution, and so we
   should learn good parameter values by evaluating the SAT 2007 competition.
   </li>
   <li> The conceptually simplest way to achieve this is to optimise the score
   the solver would have achieved in the SAT 2007 competition. </li>
   <li> Since this needs running the solver on the whole
   competition many times, we need decent computational resources. </li>
  </ul>

*/

/*!
  \namespace OKlib::OKsolver_2_0
  \brief Applications implementing the new OKsolver (for SAT 2009)
*/

namespace OKlib {
  namespace OKsolver_2_0 {
  }
}

