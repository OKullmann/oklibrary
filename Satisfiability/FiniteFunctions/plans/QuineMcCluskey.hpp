// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp
  \brief Plans regarding the direct implementation of the computation of the minimal elements of the resolution closure of full boolean clause-sets


  \todo Quine/McCluskey in general
  <ul>
   <li> A special situation for the computation of prime implicates/implicants
   is given when we start with the full table, that is, with all full
   implicates/implicants given. </li>
   <li> For the boolean case the algorithm derived from the general
   resolution algorithm together with some obvious simplifications regarding
   subsumption-elimination is called the "Quine-McCluskey algorithm". </li>
   <li> Actually, the algorithm only needs either all full implicates
   (for the prime implicates) or all full implicants (for the prime
   implicants), and both together are only used to make the algorithm
   polynomial time (since then, %e.g. in the boolean case, the input
   is of size 2^n, while a trivial essential upper bound is 3^n). </li>
   <li> The algorithm in more detail:
    <ol>
     <li> Input is a clause-set F (either CNF or DNF). </li>
     <li> In general a "subsumption-resolution" is one resolution step where
     the resolvent subsumes at least one parent clause. A
     "2-subsumption-resolution" is where both parent clauses are subsumed
     (that is, iff both parent clauses are identical except of the clashing
     literal). </li>
     <li> Let k := n(F), F_k := F. </li>
     <li> While k > 0 do
      <ol>
       <li> F_{k-1} := all 2-subsumption-resolvents from F_k. </li>
       <li> Remove from F_k all clauses which were involved in a
       resolution; and furthermore check whether "by chance" a clause
       in F_k is otherwise subsumed by some clause in F_{k-1} (then that
       clause is also removed). </li>
      </ol>
     </li>
     <li> The union of F_n, ..., F_0 then is the set of prime implicates
     (for CNF) resp. prime implicants (for DNF), where F_k contains the
     clauses of length k. </li>
    </ol>
   </li>
   <li> Correctness follows by the simple fact that when F_k is first
   computed, then it contains exactly all implicates (implicants) of
   size k. and furthermore if a clause of size k was not involved in a
   resolution step when going to level k-1, then it cannot become useful
   later, since we already knew all implicates of size k-1. </li>
   <li> The main point for the efficient implementation is finding the
   2-subsumption-resolutions fast.
    <ul>
     <li> One could think of representing the clauses in the levels F_k
     not directly, but indirectly, how they were created. This should
     save space, and could also be more time efficient. </li>
     <li> The most promising approach seems to me (OK):
      <ol>
       <li> The underlying observation is, that a clause of length k
       has exactly k possible partners for 2-subsumption resolution. </li>
       <li> So at level k (where all clauses have length k) each clause has
       an associated vector of booleans of length k, showing whether on
       the associated variable of the clause a 2-subsumption resolution
       has been done. </li>
       <li> Now processing of level k happens by running through the list
       of clauses, for each clause finding the available 2-subsumption
       resolution partners, performing the resolutions and entering this
       information into the vectors. </li>
       <li> The main (potential) saving lies in actively searching for
       the resolution partners, which must be supported by some appropriate
       search data structure. Either a search tree or a hash table.
        <ul>
         <li> For small n (<= 16) one can treat clauses as vectors of
         length n with entries 0,1,2, which can be hashed to a bit-vector
         of length 3^n by interpretation as a positional representation
         of a ternary number. </li>
         <li> This vector for n = 16 has 3^16 / 8 = 5380840 bytes, and
         initialising this data structure once shouldn't be a problem
         (setting the bits for existing clauses to 1, others to 0).
         (The hash map could be implemented via template
         metaprogramming.) </li>
         <li> For n=20 we need 436 MB; best to use a macro, with
         default value 18 (48 MB), for the maximal n where such a hash
         table is to be used. </li>
         <li> This hash map can also be used for computing the prime
         implicates via the resolution method in general, however
         since it doesn't solve the subsumption problem it is perhaps
         less useful. </li>
        </ul>
       </li>
       <li> Perhaps one could also use the 2-subsumption resolution graph,
       where the edges are labelled with the resolution variables. </li>
       <li> The vectors saves a factor of 2 (possibly more, since the
       resolvent needs not to be considered for the next level) due to
       not attempting the second time a resolution. When using the
       above hash table, then perhaps it is not worth the effort. </li>
       <li> Further savings could be achieved by more generally not
       attempting resolution steps where the resolvent has already
       been obtained. </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> One also has to think about the additional subsumption
   checks. (We also need an example to see that these additional checks
   are indeed needed.) </li>
   <li> Many implementations of the Quine-McCluskey algorithm seem to 
   include an additional minimisation step to calculate all essential prime 
   implicates/implicants, removing certain clauses after the steps listed 
   above.
    <ol>
     <li> For example, the above algorithm given 
     {{- 3, - 1, 2}, {- 3, 1, 2}, {- 2, - 1, 3}, {- 1, 2, 3}} returns
     {{- 3, 2}, {- 1, 2}, {- 1, 3}}, however the input clause set is
     equivalent to {{-3,2},{-1,3}} and the R implementation mentioned in 
     Buildsystem/ExternalSources/SpecialBuilds/plans gives {{-3,2},{-1,3}
     as well. </li>
     <li> These are likely computed by the "Carnaugh maps", which we also
     need to do. </li>
    </ol>
   </li>
   <li> Is there a way of using both the satisfying and the falsifying
   total assignments? Likely the way to do this is to combine the resolution
   algorithm with the dualisation algorithm. </li>
   <li> Can the Quine/McCluskey-algorithm be generalised from boolean
   clause-sets to more general forms of clause-sets?
    <ol>
     <li> For clause-sets with non-boolean variables it should be similar
     (only that for a variable of domain size m then m parent
     clauses are needed). </li>
     <li> Questionable for general signed clause-sets. </li>
    </ol>
   </li>
  </ul>


  \todo Names and documentation
  <ul>
   <li> All names have to be changed: typenames are like
   "ClauseSet", functions are like "compute_something" (or just
   "something"); so non-type-names have no upper-case-letters in them. </li>
   <li> All introduced functions and types need doxygen-documentation. </li>
  </ul>


  \todo Application tests
  <ul>
   <li> The application tests have to be extended (using test cases where the
   output is known). </li>
   <li> Also the full and perhaps the extensive level needs to be present.
   </li>
   <li> The program needs to be able to cope with incorrect inputs. </li>
   <li> And all these error cases need to be tested. </li>
  </ul>


  \todo Precise definitions, and basic types
  <ul>
   <li> Use asserts for example to check for assumptions on function
   arguments. </li>
   <li> std::vector<bool>::size_type is a central integral type here,
   determining the limits of the computation; starting from this a
   (small) resource management (at compile-time!) has to be created. </li>
   <li> Of course, at least typedefs for the basic types of variables,
   literals, clauses and clause-sets need to be provided. </li>
  </ul>


  \todo Input and output
  <ul>
   <li> The monitoring output should be optional. </li>
   <li> Components from Satisfiability/Interfaces/InputOutput should be
   used. </li>
   <li> Also options like "--version" or "--help" should be available. </li>
   <li> See Programming/ProgramOptions/plans/general.hpp. </li>
  </ul>


  \todo Unit tests
  <ul>
   <li> Use the (new) higher-order unit test framework. </li>
  </ul>

*/

