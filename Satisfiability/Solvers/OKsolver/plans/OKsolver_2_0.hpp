// Oliver Kullmann, 1.2.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/plans/OKsolver_2_0.hpp
  \brief Plans for the module for the new OKsolver

  Possibly called "OKsolver2010".


  \todo New planning
  <ul>
   <li> Outline
    <ol>
     <li> To the input F_0 amplifying preprocessing F_0 := ap(F_0) is applied,
     where the only condition is that ap(F_0) is satisfiability equivalent
     to F_0. </li>
     <li> Except of full restarts, the strongest preprocessing further applied
     is given by the reduction r_0 fulfilling that r_0(F) is satisfiability
     equivalent to F, and that r_0(F) logically follows from F. </li>
     <li> Let P_0 by the number of processors available, and let P be the
     number of parallel processes we actually run; P >= max(2,P_0), and
     perhaps P = 2*P_0. </li>
     <li> Now a wide branching (phi_1, ..., phi_W) for F_0 is computed:
      <ul>
       <li> The correctness condition is that all total assignments are
       covered by the phi_i. </li>
       <li> The width W of the branching should be quite a bit bigger
       than P, for example W = 2^6 * P. </li>
       <li> The branches i with instance r_0(phi_i * F_0) are to be processed
       in this order. </li>
       <li> Each branch i yields an independent process p_i. </li>
      </ul>
     </li>
     <li> Each process p_i can be queried for its current learned clause
     lc(p_i), and for its current adapted heuristics h(p_i). </li>
     <li> The processes p_i run otherwise completely independent of each other,
     as a look-ahead solver with reduction r ~ r_2 and look-ahead reduction
     r' ~ r_1, and with optimisation of the heuristics (also only considering
     process p_i). </li>
     <li> Similar to OKsolver2002, but now with adaptive optimisation of the
     distance functions, autarky search via resolution duality, and active
     clauses. </li>
     <li> If a process p_i completes, then lc(p_i) (as partial assignment
     contained in phi_i) is added to F_0, and h(p_i) is somehow stored. </li>
     <li> A process p_i can also decide that its execution takes too long,
     and abort itself, where again lc(p_i) is added to F_0, while perhaps
     h(p_i) is not stored. </li>
     <li> If a new process p_i is to be started (as the next in the list, or
     as a re-start of some aborted process (if such exist then they have,
     according to the order, priority)), then all active processes p_j are
     queried for lc(p_j) which are added to F_0, and by some weighted average
     of the (current) heuristics h(p_j) plus the stored ones the heuristics
     for p_i is computed. </li>
     <li> p_i then starts with r_0(phi_i * F_0) (where phi_i is the fixed
     partial assignment from the wide branching determined above, while
     F_0 now contains learned clauses). </li>
     <li> It might happen that due to tree pruning finishing one process
     can lead to directly finishing other processes (since they must also
     be unsatisfiable). </li>
     <li> Finally, if the overall management decides that it takes too long,
     then a complete restart is performed, where then ap is re-applied to F_0
     (which contains all learned clauses, including those obtained by aborting
     all processes), and where the new basic heuristics is some weighted
     average of all (relevant) obtained until then. </li>
    </ol>
   </li>
   <li> Clause learning:
    <ol>
     <li> Learning here means "global clause learning". </li>
     <li> In the spirit of "radical learning", learning of a new clause always
     means a restart (only in this way does the search tree stay consistent
     with the reductions). </li>
     <li> During the execution of a process p_i (see above), no clause learning
     is performed; more precisely: F_0 will grow by clause learning, but the
     already started processes are not affected by this, only new ones. </li>
     <li> An important observation is that r_2-decision-learning (see
     "Semantical trees" and "A very attractive form of learning is
     r_k-compressed learning" in
     Satisfiability/Algorithms/Learning/plans/general.hpp) is very easy for
     OKsolver2002, and this yields the clauses lc(p_i) (which in a certain
     sense summarise the current achievement of p_i). </li>
     <li> This learning is not "shallow and forgetful learning" as with
     conflict-driven solvers, but "deep and permanent learning" (the learned
     clauses are expressed as much as possible in the primary decision, using
     strong reductions, and learned clauses are never removed). </li>
     <li> Since only decision literals are learned, the locality aspect so
     often emphasised for conflict-driven learning is lost, but this seems to
     be appropriate here, since learning is always (only) for the *other*
     processes --- and they don't know about *locality* for the process
     where the learned clause originated, but they want to learn which paths
     to avoid. </li>
    </ol>
   </li>
   <li> Restarts:
    <ol>
     <li> We do not have "restarts" in the sense often used nowadays, as a
     kind of ordinary thing to do (rather often). </li>
     <li> The only forms of "restarts" are the aborting of a current process,
     and the full restart. Both are emergency measures, to be taken when
     the situation looks quite bad (for some time already). </li>
     <li> Aborting a process p_i looks only at the target for p_i and its
     current prediction. </li>
     <li> While for a full restart all current predictions are taken into
     account. </li>
     <li> A full restart can also be triggered by a signal (send by the
     user). </li>
    </ol>
   </li>
   <li> Preprocessing (via reduction):
    <ol>
     <li> The minisat2-preprocessing seems always to help OKsolver_2002; and
     often quite a lot. </li>
     <li> Preprocessing as amplification is put into ap (see above); since
     it is only experimental yet, likely we concentrate on preprocessing as
     reduction (see
     ComputerAlgebra/Satisfiability/Lisp/Preprocessing/plans/general.hpp).
     </li>
     <li> The old joined reports with Luckhardt should serve as a starting
     point for the reductions used. </li>
     <li> Definitely "harmless" DP-reduction, and local resolution-reductions.
     </li>
     <li> Perhaps also r_3? But for big inputs, this could take a very long
     time. </li>
    </ol>
   </li>
   <li> Boolean versus non-boolean:
    <ol>
     <li> It seems that doing all the new algorithms together with non-boolean
     literals is too much. </li>
     <li> So OKsolver version 2 should still be boolean. </li>
     <li> But OKsolver version 1 should be generalised (in parallel) to
     non-boolean literals, so that later we can catch up. </li>
    </ol>
   </li>
   <li> Distance optimisation:
    <ol>
     <li> This is an important new aspect, and must be fully integrated into
     version 2. </li>
     <li> Already with OKsolver2002 we make first experiences in this
     direction. </li>
     <li> Open yet, how to do online optimisation? </li>
     <li> Each process p_i optimises only its own heuristics according to its
     own search tree, but for new processes one somehow needs to integrate
     the various heuristics from all the different processes. </li>
    </ol>
   </li>
   <li> Autarky search:
    <ol>
     <li> The "dual autarky search", done for each new %node and after
     backtracking, where then the variables used in the refutation of the
     first branch are crossed out (see "Autarky search" below), is an
     important aspect. </li>
     <li> It should also enable us to handle satisfiable instances with much
     bigger success. </li>
    </ol>
   </li>
   <li> Generalised SAT:
    <ol>
     <li> We do not consider different types of literals yet (since there are
     only boolean literals). </li>
     <li> So no "active clause-sets" are handled, only "active clauses". </li>
     <li> And for these active clauses the fundamental assumption is that
     counting and creating the prime implicates is easy (taking the size
     of the output into account for the latter task). </li>
     <li> And thus we truly have "virtual clauses": At the general level
     the solver behaves as if there are only clauses, and the active clauses
     fully simulate their representations via the set of prime implicates.
     </li>
     <li> Important example here are cardinality constraints. </li>
     <li> While equivalence reasoning is not employed yet (except possibly
     in the preprocessing). </li>
    </ol>
   </li>
   <li> Computing the wide branching (phi_1, ..., phi_W):
    <ol>
     <li> The simplest scheme is just to choose a depth d, W = 2^d,
     and to let the partial assignments correspond to the branching tree
     developed by a normal look-ahead solver, for r_0(F_0). </li>
     <li> More precisely, at the root one has r_0(F_0), there one determines
     the first branching, applies to both children again r_0, determines
     the next branchings, and so on. </li>
     <li> In order that one doesn't need later to apply r_0 after each single
     assignment, but just after the full partial assignment, one needs that r_0
     is compatible with application of partial assignment. </li>
     <li> Now the order on the leaves of this tree of depth d is not the order
     "from left to right", but the one as introduced by [van Maaren, Heule].
     </li>
     <li> Consider d=3. A leaf at depth 3 is associated with a binary word
     of length 3, where 0 means "first branch", 1 means "second branch", and
     where the word is read from left to right (corresponding to the movement
     from the root towards the leaves). </li>
     <li> Then the natural order to visit the leaves, from left to right,
     just corresponds to binary counting; in this example the order of
     the leaves is 000, 001, 010, 011, 100, 101, 110, 111. </li>
     <li> Now vanMaaren/Heule's idea is that (i) 0 is better than 1 (that's
     the heuristics for the first branch), and (ii) later is better than
     earlier (since more precise due to the smaller problems). </li>
     <li> So the processing order is obtained by reading the words from
     right to left as binary words. </li>
     <li> Thus for this example the processing order is (000, 100, 010,
     110, 001, 101, 011, 111). </li>
     <li> Given rank 1 <= i <= i in the list [phi_1, ..., phi_W], the
     following Maxima function path(i,d) computes the original branching word,
     while by rank(path) from an original branching word we obtain its
     rank regarding execution:
     \verbatim
rank(path) := polyadic2int(reverse(path),2)+1$
path(i,d) := block([L:reverse(int2polyadic(i-1,2))], append(L,create_list(0,i,1,d-length(L))))$

create_list(path(i,3),i,1,8);
  [[0,0,0],[1,0,0],[0,1,0],[1,1,0],[0,0,1],[1,0,1],[0,1,1],[1,1,1]]
all_tuples_l([0,1],3);
  [[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1],[1,1,0],[1,1,1]]
map(rank,all_tuples_l([0,1],3));
  [1,5,3,7,2,6,4,8]
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Simple parallelism:
    <ol>
     <li> The master process constitutes the branching scheme at the beginning
     (fixed for the whole run, unless a full restart takes place), and then
     deactivates itself, while the P-many processes run. </li>
     <li> Each time a process terminates (by completion, or by
     self-abortion), the master process is activated again, and computes a new
     process. </li>
     <li> Given that W is considerably bigger than P_0 (the number of
     processors), only at the very end of the whole run do we get into the
     situation that not all processors are utilised. </li>
     <li> And the jobs should be fairly small. </li>
     <li> However, right at the end possibly more work is to be done, and so
     some adaptation should take place. </li>
     <li> It should be a fairly simple procedure. </li>
    </ol>
   </li>
  </ul>


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


  \todo DONE (there are no alliances anymore in version 2, and also no need
  to distinguish between original and learned clauses, since learning is
  always accompanied by a restart (in a sense))
  OKsolver_2 extends OKsolver_1 by using alliances of
  active clause-sets with (at least) three members :
  <ol>
   <li> the original clause-set </li>
   <li> the active clause-set for compressed r_2-learning (see
   module Learning, Satisfiability/Algorithms/Learning/plans/general.hpp).
   </li>
   <li> the active clause-set for equivalence reasoning (see
   module Equivalences). </li>
  </ol>


  \todo DONE (not done, since there are no active clause-sets)
  Further sub-division of the instance
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
   <li> DONE (this problem doesn't exist anymore, since during the run no
   clauses are learned)
   A related question is about "Re-working the tree" (see there).
    <ol>
     <li> According to the general guideline of avoiding "pure heuristics", we
     should always rework the tree. </li>
     <li> However, similar to the experience with r_2 and OKsolver2002, where
     completing a round of reduction was found to be more efficient then
     immediate restart of the reduction process, it is quite conceivable that
     the solver starts "stammering" due to all this clean-up. </li>
    </ol>
   </li>
   <li> DONE (in a sense, now clause learning is always combined with a
   restart)
   As discussed in OK's article [Present and future of practical SAT
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


  \todo DONE (this topic should be moved to version 3)
  Equivalence reasoning
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


  \todo DONE (in the form as discussed no longer appropriate, since extracting
  learned clauses is now just a trivial extension of OKsolver2002, based on
  the set of variables used in the refutation (as shall be the extended
  autarky search), and there are no alliances of active clause-sets anymore)
  Autarky search
  <ul>
   <li> Autarky search (see module Autarkies/Search) is integrated with global
   learning via the fundamental duality between resolution and autarkies: The
   global learning component manages the set of variables used in the current
   resolution tree fragment (for this %node(!)), and at every %node of the
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
   <li> So at each %node a satisfiability problem is attacked by incomplete
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


  \todo DONE (no longer appropriate here, since there is no "dynamic"
  clause learning (on the fly) anymore; the activity-based ideas could be
  considered for OKsolver version 3)
  Heuristics
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
     <li> The number of unit-clauses with variable v found during reduction.
     </li>
     <li> The number of failed literals with variable v. </li>
    </ol>
   The intuition is that if we can branch on a variable which was "recently"
   active in a lot of inferences, then this variable might again provide
   a lot of action, a kind of culmination point, and it might be good to branch
   on it. To quantify "recent" one can use some decay scheme. </li>
   <li> A different approach takes the dynamic changes into account, where
   (as discussed above) the second branch might happen under very different
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
     a look-ahead solver spends a lot of work on the decisions, it might be
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


  \todo DONE (the open questions now are about self-abortion and full restart,
  and that's a question of the time-prediction)
  Restart policy
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


  \todo DONE (the context changed now, but the basic principle is the same:
  consider a given total time as given, and if the prediction is more than
  that by a certain factor, one must restart, i.e., abort the process)
  Time-prediction
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


  \todo DONE
  If restarts don't yield clear benefits, then they are not incorporated
  into OKsolver_2_0, but the whole issue is thoroughly treated with
  OKsolver_3_0.


  \todo Monitoring the search process : needs UPDATE (relevance unclear)
  <ul>
   <li> The above resource management strategies use prediction; another
   possibility are the monitoring schemes similar to those investigated by
   Heule and van Maaren:
    <ol>
     <li> Considering some observation level d in the search tree and a given
     total time R, the time per observation %node is R / 2^d. If the time is
     out, then the search is interrupted, and search jumps to the next %node on
     the observation level, mainly in the hope to find a satisfying assignment
     there, but also good learned clauses could be obtained there. </li>
     <li> Actually, the scheme of Heule and van Maaren is simpler, since it
     always completes the computation for the branches encountered at level d.
     See below for further discussions. </li>
    </ol>
   </li>
   <li> Since mainly we hope for a satisfying assignment, and we assume that
   the first branch in general is better than the second branch, and
   furthermore we assume that deeper down the tree the prediction
   of the first branch becomes better, the rules for determining the next
   search %node are just 1) left before right 2) assume the "error" as far up
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
     <li> The guiding principle should be, that we label each %node with
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
   completely finishing the %node --- which of course needs less management,
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

