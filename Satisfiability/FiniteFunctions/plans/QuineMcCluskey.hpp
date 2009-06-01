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


  \todo Docus
  <ul>
   <li> A docus-file is needed. </li>
   <li> Specifying input and output, with various explained examples. </li>
   <li> Explaining the algorithm. </li>
   <li> Explaining also the various connections to the rest of the
   library. </li>
  </ul>


  \todo Application tests
  <ul>
   <li> The application tests have to be extended (using test cases where the
   output is known). </li>
   <li> Given the need for more extensive tests on larger number of variables,
   it will be necessary to offer a binary (by default) for the tests which
   handles larger numbers of variables. See "Different numbers of variables".
   </li>
   <li> Also the full and perhaps the extensive level needs to be present.
   </li>
   <li> The program needs to be able to cope with incorrect inputs. </li>
   <li> And all these error cases need to be tested. </li>
  </ul>

  
  \todo Different numbers of variables
  <ul>
   <li> As the runtime and space requirements of the algorithm depend on the
   number of variables, which is currently defined at compile time, then it 
   is likely necessary that several versions are provided, for different numbers
   of variables. </li>
   <li> At first, two binaries seems reasonable, the standard QuineMcCluskey and 
   a QuineMcCluskey-Large which supports 16 variables. </li>
  </ul>


  \todo Maxima level
  <ul>
   <li> The algorithm needs to be implemented also at Maxima level. </li>
   <li> There then perhaps also using lists for the clauses? </li>
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
   <li> At a higher verbosity-level the program should forecast its
   anticipated memory and time usage.
    <ol>
     <li> Memory usage likely can be rather precisely calculated. </li>
     <li> While for time complexity we likely need a model and certain
     experiments, which for a specific computer determine the coefficients
     of the model. </li>
     <li> This determination should be done by a specific script, and
     the values stored in a file read by the program. </li>
    </ol>
   </li>
   <li> Perhaps the used value of NUMBER_VARIABLES should be part of
   the name of the executable? Or at least it should be possible to
   obtain this number from the program. </li>
  </ul>


  \todo Unit tests
  <ul>
   <li> Use the (new) higher-order unit test framework. </li>
  </ul>


  \todo Improvements of the implementation
  <ul>
   <li> ipow should be replaced by an array computed at compile-time. </li>
   <li> How to dynamically initialise an array at compile time? Something
   like:
   \verbatim
ClauseHash powers[nVars+1];

template<int p, int c>
struct ipow3_s {
  static inline void ipow3_c() {
    powers[nVars-p] = c;
    ipow3_s<p-1,c*3>::ipow3_c();
  }
};

template<int c>
struct ipow3_s<0,c> {
  static inline void ipow3_c() {
    powers[nVars] = c;
  }
};
   \endverbatim
   although then, the question is where to call ipow3_s::ipow3_c()? </li>
   <li> Likely we should have a dedicated clause-type here, based on
   an array of length 16 and a size-member. </li>
   <li> One needs to connect to the general concepts of variables, literals,
   clauses and clause-sets, such that all these types here become special
   instances of general concepts. </li>
  </ul>


  \todo Performance and code analysis analysis
  <ul>
   <li> We need tools in the OKlibrary to analyse the performance of
   C/C++ programs. </li>
   <li> One central system, for making measurements, is described in
   ComplexitySystem/plans/general.hpp. </li>
   <li> Compare "Apply Valgrind", "Apply code analysis tools (like Splint)"
   and "Optimising the code" in Solvers/OKsolver/SAT2002/plans/general.hpp.
   </li>
  </ul>


  \todo Parallelisation
  <ul>
   <li> Usage of Mpi (now supported by gcc) or of threads (according to
   C++09) is to be studied. </li>
   <li> So that tackling really big tasks becomes possible. </li>
  </ul>

*/

