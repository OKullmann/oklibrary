// Oliver Kullmann, 6.12.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/FiniteFunctions/plans/general.hpp
  \brief Super-module for finite functions (especially boolean functions)

  Considered are functions f: D_1 x ... X D_n -> {0,1}, where D_i
  are finite sets (in the boolean case we have F_i = {0,1}).


  \todo Rename applications
  <ul>
   <li> All that "SubsumptionHypergraph", "Full", "With" etc. says nothing,
   and is also far too long. </li>
   <li> All these applications (perhaps also QuineMcCluskey) have to be
   renamed. </li>
   <li> Perhaps also reorganisation is needed. </li>
  </ul>


  \todo Links and plans
  <ul>
   <li> Establish links to other modules (especially computer algebra).
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp.
     </li>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/general.hpp.
     </li>
    </ol>
   </li>
   <li> See
   Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp. </li>
   <li> Create a development plan and further milestones. </li>
  </ul>


  \todo Prime implicants and implicates
  <ul>
   <li> Literature:
    <ol>
     <li> [Ron Rymon, 1994, An SE-tree-based Prime Implicant Generation
     Algorithm] contains algorithms for computing prime implicants (via
     transversals). </li>
    </ol>
   </li>
   <li> This whole topic seems to deserve its own module (and plans-file).
   </li>
   <li> What terminology to use for *clause-sets*, where we could have
   CNF's or DNF (thus prime implicates or prime implicants) ? </li>
   <li> We consider the whole range of generalised clause-sets, from boolean
   clause-sets to signed clause-sets. </li>
   <li> Implicates for F are generalised clauses which follows from F,
   while implicants are generalised clauses which imply F. </li>
   <li> Between generalised clauses we have the partial order "implies",
   which is quickly decidable. </li>
   <li> Prime implicates are implicates which are minimal %w.r.t. to the
   partial order of implication amongst the type of generalised clauses
   considered. </li>
   <li> The algorithms which works always is to create all resolvents and
   keep the minimal ones (the "resolution algorithm").
    <ol>
     <li> For given F as CNF, this computes the prime implicates/implicants.
     </li>
     <li> See ProofSystems/Resolution/plans/ResolutionClosure.hpp. </li>
     <li> So in this module we only connect the resolution algorithm
     (which likely shouldn't care about "DNF" or "CNF") to the
     concept of finite functions. </li>
    </ol>
   </li>
   <li> The other main alternative is to start with a CNF or DNF (generalised)
   given, and then compute the minimal satisfying partial assignments (using
   the given literal type), which yields the prime implicants resp.
   the prime implicates (call it the "dualisation algorithm").
    <ol>
     <li> See AllSolutions/plans/MinimalAssignments.hpp. </li>
     <li> So again, likely the general algorithm is not implemented in
     this module. </li>
    </ol>
   </li>
   <li> Given clause-sets F, G (as CNF), deciding whether G is the
   set of prime implicates of F:
    <ol>
     <li> G must be subsumption-free. </li>
     <li> For every clause C in F there must be D in G with D <= C. </li>
     <li> Given two resolvable clauses D, D' in G, their resolvent must
     be subsumed by some element from G. </li>
     <li> These conditions are all easily checkable; in general the last
     condition, that all C in G follow from F, however is hard to establish.
     </li>
     <li> If for F and partial assignments phi one can decide satisfiability
     of phi*F in polynomial time, then also this check is easy. </li>
     <li> Examples are given by hitting F, Horn F or F in 2-CNF. </li>
    </ol>
    One should establish a test algorithm for these basic cases, so that
    for example we can verify the results of QuineMcCluskey.cpp.
   </li>
  </ul>


  \todo Minimisation
  <ul>
   <li> Literature:
    <ol>
     <li> [Coudert, O; 1994, Two-level logic minimization: An overview;
     Integration, the VLSI Journal, vol. 17, pages 97-140] could serve
     as a start for the area of optimisation of boolean functions. </li>
    </ol>
   </li>
   <li> The fundamental task is, given the function table, compute CNF
   resp. DNF representations of minimum "size", where the most fundamental
   notion of size here is the number of clauses. </li>
   <li> Minimising this size is equivalent to finding a smallest (w.r.t.
   number of clauses) irredundant core of the set of prime implicates resp.
   implicants. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Cores.mac.
   </li>
   <li> In the given situation, where we have all implied full clauses, it
   is easier to check whether a clause is necessary or implied by the
   others, by using the set of full clauses it subsumes. </li>
   <li> So first Quine/McCluskey is used, followed by minimisation. </li>
   <li> Minimisation is discussed in the literature in the context of
   "Karnaugh-maps"; though they are trivial, and one needs to look for the
   literature for more interesting algorithms. </li>
   <li> See package "QCA" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp.
    <ol>
     <li> Likely this just performs computation of minimum transversals (see
     below) in the trivial way via integer linear programming. </li>
     <li> Using this package in the following way:
     \verbatim
######## In Maxima #######
generate_full_aes_sbox_tt() :=  
  map(
     lambda([ce],
       append(
         int2polyadic_padd(ce[1],2,8),
         int2polyadic_padd(ce[2],2,8),
         if rijn_lookup_sbox(ce[1]) = ce[2] then [1] else [0]))
     ,cartesian_product(setmn(0,255),setmn(0,255)))$

with_stdout("Sbox.tt", block(
  apply(print, endcons("O",create_list(i,i,1,16))),
  for tt_line in generate_full_aes_sbox_tt() do
    apply(print,tt_line)
  ))$

######## In R ###########

oklib_load_all()
library(QCA)

sbox_tt = read.table("Sbox.tt",header=TRUE)
eqmcc(sbox_tt, outcome="O", expl.0=TRUE)
     \endverbatim
     leads to the following error:
     \verbatim
Error in vector("double", length) : 
  cannot allocate vector of length 2130706560
     \endverbatim
     </li>
     <li> Note that using "expl.1=TRUE" instead of "expl.0=TRUE" to minimise
     the DNF representation returns the following:
     \verbatim
Solution: abcdefghiJKlmnOP + abcdefgHiJKLMNop + abcdefGhiJKLmNOP + 
          abcdefGHiJKLMnOP + abcdeFghIJKLmnOp + abcdeFgHiJKlMnOP + 
          abcdeFGhiJKlMNOP + abcdeFGHIJklmNoP + abcdEfghijKLmnop + 
          abcdEfgHijklmnoP + abcdEfGhiJKlmNOP + abcdEfGHijKlMnOP + 
          abcdEFghIJKLMNOp + abcdEFgHIJkLmNOP + abcdEFGhIjKlMnOP + 
          abcdEFGHiJKLmNOp + abcDefghIJklMnOp + abcDefgHIjklmnOp + 
          abcDefGhIJklMnoP + abcDefGHiJKLMNoP + abcDeFghIJKLMnOp + 
          abcDeFgHiJkLMnoP + abcDeFGhiJklmNOP + abcDeFGHIJKLmnop + 
          abcDEfghIjKlMNoP + abcDEfgHIJkLmNop + abcDEfGhIjKlmnOp + 
          abcDEfGHIjKlMNOP + abcDEFghIjkLMNop + abcDEFgHIjKlmNop + 
          abcDEFGhiJKLmnOp + abcDEFGHIJklmnop + abCdefghIjKLmNOP + 
          abCdefgHIJKLMNoP + abCdefGhIjkLmnOP + abCdefGHijKlmNOp + 
          abCdeFghijKLmNOp + abCdeFgHijKLMNOP + abCdeFGhIJKLmNOP + 
          abCdeFGHIJklMNop + abCdEfghijKLmNop + abCdEfgHIjKlmNoP + 
          abCdEfGhIJKlmNoP + abCdEfGHIJKLmnoP + abCdEFghiJKLmnoP + 
          abCdEFgHIJkLMnop + abCdEFGhijKLmnoP + abCdEFGHijkLmNoP + 
          abCDefghijklmNop + abCDefgHIJklmNOP + abCDefGhijKlmnOP + 
          abCDefGHIJklmnOP + abCDeFghijkLMnop + abCDeFgHIjkLmNOp + 
          abCDeFGhijklmNoP + abCDeFGHIjkLMnOp + abCDEfghijklmNOP + 
          abCDEfgHijkLmnOp + abCDEfGhIjklmnop + abCDEfGHIJKlmnOp + 
          abCDEFghIJKlMnOP + abCDEFgHijKlmNOP + abCDEFGhIjKLmnOp + 
          abCDEFGHiJKLmNoP + aBcdefghijklMnoP + aBcdefgHIjklmnOP + 
          aBcdefGhijKlMNop + aBcdefGHijkLMnOp + aBcdeFghijkLMnOP + 
          aBcdeFgHiJKlMNOp + aBcdeFGhiJkLMnOp + aBcdeFGHIjKlmnop + 
          aBcdEfghiJkLmnOp + aBcdEfgHijKLMnOP + aBcdEfGhIJkLmNOp + 
          aBcdEfGHIjKLmnOP + aBcdEFghijKlMnoP + aBcdEFgHIJKlmnOP + 
          aBcdEFGhijKlMNOP + aBcdEFGHIjklmNop + aBcDefghiJkLmnOP + 
          aBcDefgHIJkLmnoP + aBcDefGhijklmnop + aBcDefGHIJKlMNoP + 
          aBcDeFghijKlmnop + aBcDeFgHIJKLMNop + aBcDeFGhIjKLmnoP + 
          aBcDeFGHiJkLMnOP + aBcDEfghiJKlMnOp + aBcDEfgHIJklMnOP + 
          aBcDEfGhIjKLMNOp + aBcDEfGHijKLMnoP + aBcDEFghiJklMnOp + 
          aBcDEFgHiJklMNop + aBcDEFGhiJkLMnop + aBcDEFGHIJklMNOP + 
          aBCdefghIJkLmnop + aBCdefgHIJKlMNOP + aBCdefGhIjKlMnOp + 
          aBCdefGHIJKLMnOP + aBCdeFghiJklmnOP + aBCdeFgHiJklMNoP + 
          aBCdeFGhijKLmnOP + aBCdeFGHIjklmNoP + aBCdEfghiJklmNoP + 
          aBCdEfgHIJKLMnoP + aBCdEfGhijklmnOp + aBCdEfGHiJKLMNOP + 
          aBCdEFghiJkLmnop + aBCdEFgHijKLMNop + aBCdEFGhIjkLMNOP + 
          aBCdEFGHIjKlMnop + aBCDefghiJkLmnoP + aBCDefgHIjKlmnOP + 
          aBCDefGhiJklmnop + aBCDefGHIjklMNOP + aBCDeFghIjkLmnOp + 
          aBCDeFgHIjkLMNoP + aBCDeFGhijKLMnop + aBCDeFGHIJKLmNoP + 
          aBCDEfghIjKLMNop + aBCDEfgHIjKLmNOp + aBCDEfGhIJkLMnOp + 
          aBCDEfGHijKlmnoP + aBCDEFghijkLmnop + aBCDEFgHIJKLMNOP + 
          aBCDEFGhIJKLmnOP + aBCDEFGHIJkLmnOp + AbcdefghIJklMNoP + 
          AbcdefgHijklMNop + AbcdefGhijkLmnOP + AbcdefGHIJKlMNop + 
          AbcdeFghiJkLMNOP + AbcdeFgHIjkLmNOP + AbcdeFGhiJklmNop + 
          AbcdeFGHijkLmNOP + AbcdEfghIJklmNop + AbcdEfgHIjKlmNOP + 
          AbcdEfGhiJKLMNOp + AbcdEfGHijKLMNoP + AbcdEFghiJKlmNop + 
          AbcdEFgHiJkLMNoP + AbcdEFGhijkLMnoP + AbcdEFGHiJKLmnOP + 
          AbcDefghiJKlmnop + AbcDefgHIjklmnoP + AbcDefGhiJklMNOP + 
          AbcDefGHIJkLMNop + AbcDeFghijKlmnOp + AbcDeFgHijKlMnOp + 
          AbcDeFGhIjkLmnop + AbcDeFGHIjklMnop + AbcDEfghiJklmNOp + 
          AbcDEfgHIJKlMNOp + AbcDEfGhIjKLMnop + AbcDEfGHijkLmNop + 
          AbcDEFghIJkLMNOp + AbcDEFgHiJkLMNOp + AbcDEFGhijklMnOP + 
          AbcDEFGHIJkLMnOP + AbCdefghIJKlmnop + AbCdefgHijKLmnOp + 
          AbCdefGhijKLMnOp + AbCdefGHijklMnOp + AbCdeFghiJklMnoP + 
          AbCdeFgHijklmNOp + AbCdeFGhijKlmNop + AbCdeFGHiJkLMNop + 
          AbCdEfghIJklmnOp + AbCdEfgHIJkLmnOP + AbCdEfGhIjKlMNop + 
          AbCdEfGHiJKlmnOp + AbCdEFghIjkLmnoP + AbCdEFgHIjkLmNoP + 
          AbCdEFGhIJKlmNop + AbCdEFGHiJKLMnoP + AbCDefghIJKlmNOP + 
          AbCDefgHIJklMnop + AbCDefGhijKLmNOP + AbCDefGHiJKlMNoP + 
          AbCDeFghIjklMNoP + AbCDeFgHIJkLmNoP + AbCDeFGhiJklMNOp + 
          AbCDeFGHIjKlMnoP + AbCDEfghiJKlMNop + AbCDEfgHiJkLmNOp + 
          AbCDEfGhIJKLmNop + AbCDEfGHIJKlMnOp + AbCDEFghiJKlmNoP + 
          AbCDEFgHiJKLMnOp + AbCDEFGhIjKlMNOp + AbCDEFGHijklMnop + 
          ABcdefghIjKLMnOp + ABcdefgHiJKLMnop + ABcdefGhijKlmNoP + 
          ABcdefGHijKlMNOp + ABcdeFghijkLMNop + ABcdeFgHIjKlmNOp + 
          ABcdeFGhIjKLmNop + ABcdeFGHIJklmNOp + ABcdEfghIJKlMnop + 
          ABcdEfgHIJkLMNoP + ABcdEfGhiJKLmNop + ABcdEfGHijkLMNOP + 
          ABcdEFghiJklMnOP + ABcdEFgHIjKLMNoP + ABcdEFGhIjklMnOP + 
          ABcdEFGHIjklMnOp + ABcDefghiJKLmnop + ABcDefgHijKLMNOp + 
          ABcDefGhIjKLmNoP + ABcDefGHiJKlmNOp + ABcDeFghiJklMnop + 
          ABcDeFgHijklmnOP + ABcDeFGhIJKLmNOp + ABcDeFGHijklMNOp + 
          ABcDEfghiJKlmnoP + ABcDEfgHijKLmNoP + ABcDEfGhiJkLmNOP + 
          ABcDEfGHIjKLMnoP + ABcDEFghIjklmNOp + ABcDEFgHIJklmnoP + 
          ABcDEFGhijkLMNoP + ABcDEFGHIjkLMNOp + ABCdefghIJKlmnoP + 
          ABCdefgHIJKLMnop + ABCdefGhIjkLMnop + ABCdefGHijkLmnoP + 
          ABCdeFghiJKlMnoP + ABCdeFgHIJkLMnoP + ABCdeFGhIjklMNOp + 
          ABCdeFGHIjkLmNop + ABCdEfghIjkLMnOP + ABCdEfgHijkLMNOp + 
          ABCdEfGhIjklmNOP + ABCdEfGHIJKlMnoP + ABCdEFghIJklMNOp + 
          ABCdEFgHiJkLmNoP + ABCdEFGhijKlMnop + ABCdEFGHIJkLMNOP + 
          ABCDefghIjklMNop + ABCDefgHIjKlmnoP + ABCDefGhIjklMnoP + 
          ABCDefGHijklMNoP + ABCDeFghIjKLMNOP + ABCDeFgHIJKlmNOp + 
          ABCDeFGhiJklmnOp + ABCDeFGHiJKlMnop + ABCDEfghiJklmnoP + 
          ABCDEfgHIjkLMnoP + ABCDEfGhijKlMNoP + ABCDEfGHijklMNOP + 
          ABCDEFghIjKLmnop + ABCDEFgHiJkLmNop + ABCDEFGhIjKLMnOP + 
          ABCDEFGHijkLmNOp

a/A is the absence/presence of X1 
b/B is the absence/presence of X2 
c/C is the absence/presence of X3 
d/D is the absence/presence of X4 
e/E is the absence/presence of X5 
f/F is the absence/presence of X6 
g/G is the absence/presence of X7 
h/H is the absence/presence of X8 
i/I is the absence/presence of X9 
j/J is the absence/presence of X10 
k/K is the absence/presence of X11 
l/L is the absence/presence of X12 
m/M is the absence/presence of X13 
n/N is the absence/presence of X14 
o/O is the absence/presence of X15 
p/P is the absence/presence of X16 
     \endverbatim
     which is a representation of all of the original DNF clauses as expected.
     </li>
     <li> According to "Allan Engelhardt", the size of a vector in R is limited
     to 2^31 - 1 (on all platforms), and hence we get this error, as QCA seems 
     to require a larger vector. This is also mentioned here -
     https://stat.ethz.ch/pipermail/r-help/2002-June/022342.html. </li>
     <li> However, thinking about it, the problem MG may be having may also be
     limited RAM as the allocation is of a vector of doubles, and so 4 billion+
     bytes at least would be more than his available 4 GB memory. A 64 bit
     machine might do better but even then, likely the aforementioned limit
     on the size of a vector would still be a problem. </li>
     <li> Running the qmcc minimisation on a machine with 8GB memory yields
     a similar error, complaining that a vector of size 15.9GB can not be
     generated. Using this function then seems infeasible? </li>
     <li> DONE Perhaps MG should e-mail the R mailing list? </li>
    </ol>
   </li>
   <li> The easiest first thing to do is to compute the set of all necessary
   prime clauses (as refinement of Quine/McCluskey).
    <ol>
     <li> One needs to consider each prime clause C and the set F(C) of full
     clauses subsumed by C: C is necessary iff F(C) is not covered by the
     other F(C'). </li>
     <li> One needs some intelligent algorithm for that. </li>
     <li> In the transversal hypergraph context (see below) this is just
     the computation of the set of unit hyperedges. </li>
    </ol>
   </li>
   <li> Regarding "approximations", one can also use a greedy algorithm, which
   first computes all necessary prime implicates, and if this doesn't suffice,
   chooses a clause which covers a maximum number of not yet covered implied
   full clauses, and repeats this process. This is just a special case of
   the greedy computation of "small" transversals (see below). </li>
   <li> All the above is covered by the observation, that, using
   ComputerAlgebra/Satisfiability/Lisp/Primality/PrimeImplicatesImplicants.mac,
   the miminum sets of prime-implicates representing the underlying boolean
   functions are exactly the minimum transversals of the subsumption hypergraph
   subsumption_ghg(min_2resolution_closure_cs(F), F). </li>
  </ul>


  \todo BDDs
  <ul>
   <li> In general, the decision diagrams are no longer "binary".
   Perhaps it's called "FDD" ("finite decision diagram") ? </li>
   <li> But at the beginning, especially at the Maxima/Lisp level, we only
   consider the binary case. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::FiniteFunctions
  \brief %Algorithms and representations for finite (especially boolean) functions

  Alias "FiF".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {
    }
    namespace FiF = FiniteFunctions;
  }
}

