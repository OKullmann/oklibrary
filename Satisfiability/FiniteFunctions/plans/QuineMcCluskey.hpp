// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp
  \brief Plans regarding the direct implementation of the computation of the minimal elements of the resolution closure of full boolean clause-sets


  \todo Rename QuineMcCluskey to QuineMcCluskeyPrimes
  <ul>
   <li> The QuineMcCluskey algorithm includes the prime implicates/
   set covering step, therefore, as the current QuineMcCluskey application
   only generates the prime implicates, the name is not correct and
   may lead to confusion. </li>
  </ul>


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
     the resolvent subsumes at least one parent clause. </li>
     <li> A "2-subsumption-resolution" is where both parent clauses are
     subsumed (that is, iff both parent clauses are identical except of the
     clashing literal). </li>
     <li> Let k := n(F), F_k := F. </li>
     <li> While k > 0 do
      <ol>
       <li> F_{k-1} := all 2-subsumption-resolvents from F_k. </li>
       <li> Remove from F_k all clauses which were (potentially) involved in a
       resolution. </li>
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
     <li> "Push" versus "pull":
      <ol>
       <li> The above approach could be characterised as the "push approach",
       from the parents to the resolvents. </li>
       <li> The "pull approach" starts with the potential resolvent, and
       checks whether both parent clauses are present (in which case the
       resolvent is actually created). </li>
       <li> In principle both approaches have the same complexity, since both
       consider exactly all resolvable pairs. </li>
       <li> However the details differ, especially with respect to the
       necessity of computing the various hashes. </li>
       <li> It seems that the push approach has certain advantages, since
       it easier to detect that one of the parent clauses is not there. </li>
       <li> On the other hand, there is no need to run through the clauses in
       numerical order, but one should run through them such that the
       hash-values are easily computed, and then the situation might change.
       </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> Is there a way of using both the satisfying and the falsifying
   total assignments? Likely the way to do this is to combine the resolution
   algorithm with the dualisation algorithm. </li>
   <li> Exploiting "sparse" inputs?
    <ol>
     <li> The algorithm currently available at Maxima/Lisp level,
     min_2resolution_closure_cs (see
     ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac)
     has the potential advantage that it could run asymptotically faster
     if the input is has few clauses (relative to the number of variables).
     </li>
     <li> See "min_2resolution_closure_cs" in
     ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp
     for the underlying open (at least for us) question about the complexity
     of QuineMcCluskey (in principal). </li>
    </ol>
   </li>
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


  \todo Make QuineMcCluskey read from standard input
  <ul>
   <li> If it is possible (i.e. an application only takes a single
   file), then all applications should read from stdin, as then
   one can pipe the output from one application to the next, 
   allowing for in-memory transfer of the result. </li>
  </ul>


  \todo Various versions for different values of NUMBER_VARIABLES
  <ul>
   <li> Yet to use a number of variables different than 4, 16 (the current
   defaults) one has to use e.g.
   \verbatim
FiniteFunctions> oklib all CXXFLAGS="-DNUMBER_VARIABLES=15" programs=QuineMcCluskey
   \endverbatim
   which produces OKplatform/system_directories/bin/QuineMcCluskey(-O3-DNDEBUG)
   with n=15 built-in. </li>
   <li> This is of course rather cumbersome.
    <ol>
     <li> Perhaps we introduce a configuration variable for that purpose, say
     "qmc_number_variables_okl", such that one can simply use
     "oklib all qmc_number_variables_okl=16". </li>
     <li> Depending on this variable, "programs" and "CXXFLAGS" is defined.
     </li>
     <li> Where to put this configuration variable? </li>
     <li> In BuildSystem/Configuration somewhere, and then set CXX_FLAGS
     based on this in the definitions.mak? </li>
     <li> No: the obvious place is the definitions.mak here! </li>
     <li> With a small change of the generic buildsystem one could also
     obtain the executable named correctly (with n in the name); see
     "Target all" in Buildsystem/OKlibBuilding/plans/TargetSpecifications.hpp.
     </li>
    </ol>
   </li>
   <li> Still the problem of how make different n-values available
   (automatically):
    <ol>
     <li> Perhaps for version 2.0 we improve the algorithm such that without
     time or space overhead an actual n smaller than the maximal value can
     be used. </li>
     <li> Until then we use different versions. </li>
     <li> The n-value likely needs to be part of the name. But with the
     build system yet we can't automatically incorporate this into the name.
     </li>
     <li> One could simply compile all versions for n=1, ..., 20 in
     advance. However, currently we needed then for each version two
     different files, which is too much (for this case). </li>
     <li> So a wrapper-script "QuineMcCluskey" has to be written, which
     "on demand" creates the version needed if not already provided.
      <ol>
       <li> This wrapper script performs the above oklib-call in case
       the required version is not provided. </li>
       <li> The executable produced by the buildsystem is renamed (putting
       the n-value into the name), if the above mentioned extension of
       the buildsystem is not already available. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> DONE So for version 2.0 a todo needs to be created, making sure that
   for smaller n there is no space and time overhead, while for now
   we only provide two versions (which are then tested --- basic tests for
   all versions, full tests only for the 16-bit version). </li>
   <li> DONE And until then perhaps we just provide two default variants,
   for n=4 and n=16. </li>
  </ul>


  \todo Docus
  <ul>
   <li> Explaining the algorithm (with all details). </li>
   <li> Explaining also the various connections to the rest of the
   library. </li>
   <li> DONE A docus-file is needed. </li>
   <li> DONE Specifying input and output, with various explained examples.
   </li>
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
   <li> How to include larger tests such as the Sbox, where the input
   clause-set itself is very large? Should generators be called? Where to
   store the result? </li>
  </ul>


  \todo Maxima level
  <ul>
   <li> The algorithm needs to be implemented also at Maxima level. </li>
   <li> There then perhaps also using lists for the clauses? </li>
  </ul>


  \todo Input and output
  <ul>
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
   <li> DONE Correct Dimacs output should be generated: currently the parameter
   line is missing. </li>
   <li> DONE Components from Satisfiability/Interfaces/InputOutput should be
   used. </li>
  </ul>


  \todo Performance analysis and code analysis
  <ul>
   <li> We need tools in the OKlibrary to analyse the performance of
   C/C++ programs. </li>
   <li> One central system, for making measurements, is described in
   ComplexitySystem/plans/general.hpp. </li>
   <li> Compare "Apply Valgrind", "Apply code analysis tools (like Splint)"
   and "Optimising the code" in Solvers/OKsolver/SAT2002/plans/general.hpp.
   </li>
  </ul>


  \todo Fixed number of variables
  <ul>
   <li> The number of variables in the input clause-set is ignored and the
   number of variables, and the size of the hash table used by the application
   are (should be) determined at compile-time. </li>
   <li> So one needs a "preprocessor", which calls the appropriate
   instantiation (see above). </li>
   <li> Now one needs to improve the implementation to actually take advantage
   of the constant number of variables; yet for example the hash table
   is dynamically allocated, but it could use an array instead of a vector.
   </li>
   <li> Though then one needed an array of bits, which we needed to write
   ourselves. So perhaps we stick to std::vector<bool>. </li>
   <li> But it seems worth to exploit that the number of variables is a
   compile-time constant (which enables loop-unrolling, and potentially
   many other compiler-optimisations). </li>
   <li> Later one should try out how much it actually costs to replace the
   constant number of variables with a variable number of variables. </li>
  </ul>


  \todo Improvements of the implementation
  <ul>
   <li> Likely we keep the following basic structure:
    <ol>
     <li> k runs from n to 1. </li>
     <li> Running through all clauses of length k, and determining whether
     2-subsumption resolution is possible. </li>
     <li> Since we run through all clauses of length k, hashing schemes
     are possible for determining the k potential (2-subsumption)
     resolution partners. </li>
     <li> The whole hash table is of size 3^n. </li>
     <li> However, it might be worth to always only run through the
     existing clauses of a given level:
      <ol>
       <li> We keep the hash table, and ignore the cost for the initialisation
       once at the beginning. However later one might think about using a
       "real" hash table, where then we can save the effort 3^n at all
       (think of an example with say 30 variables but only a few hundred
       clauses). </li>
       <li> We need then the list of all existing clauses for the current
       level and for the next level. </li>
       <li> This can be easily computed (given that we have the hash table,
       and thus can avoid multiple entries). </li>
       <li> This should save considerable time for those cases where we don't
       have many clauses. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> However we need to try out whether different variable orderings have
   an influence on the running time (for example sorting by ascending or
   descending variable degrees, or perhaps by minimising the product
   of the variable degrees). One would guess that trying to avoid creating
   many new clauses is preferable. For this all to work, the algorithm must
   only look at the clauses actually present. </li>
   <li> ipow should be replaced by an array computed at compile-time.
    <ol>
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
    </ol>
   </li>
   <li> Likely we should have a dedicated clause-type here, based on
   an array of length 16 and a size-member. </li>
   <li> One needs to connect to the general concepts of variables, literals,
   clauses and clause-sets, such that all these types here become special
   instances of general concepts. </li>
   <li> The handling of hash values is poor:
    <ol>
     <li> Of course, completely unacceptable that for every run the algorithm
     runs through all 3^n clauses! </li>
     <li> The key is a sensible organisation of the computation of hash values,
     integrated with the general flow. </li>
     <li> And the computation of hash values needs to be highly optimised ---
     though this depends on the context, since the aim must be to organise
     the computation in such a way as to make handling of hash values as
     efficient as possible. </li>
    </ol>
   </li>
  </ul>


  \todo Preparations for computing optimum representations
  <ul>
   <li> Optionally the necessary prime clauses should be output (in Dimacs
   format). </li>
   <li> Also optionally, the hypergraph as computed by subsumption_ghg
   (see
   ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac
   ) should be created (the minimum transversals of this hypergraph yield the
   minimum representations), in strict Dimacs format, where the meaning of
   variables is explained in the comments. </li>
   <li> The ordering of the prime clauses likely should be lexicographical.
   </li>
   <li> For that hypergraph, the unit-clause-propagation should have
   been performed already (just involving the necessary clauses, which just
   correspond to unit-hyperedges).
    <ol>
     <li> See rsubsumption_hg_full_cs(F) in
     ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac
     for the Maxima-specification. </li>
    </ol>
   </li>
  </ul>


  \todo Parallelisation
  <ul>
   <li> Usage of Mpi (now supported by gcc) or of threads (according to
   C++09) is to be studied. </li>
   <li> So that tackling really big tasks becomes possible. </li>
  </ul>


  \todo DONE Unit tests
  <ul>
   <li> Use the (new) higher-order unit test framework. </li>
  </ul>


  \todo DONE Precise definitions, and basic types
  <ul>
   <li> DONE Use asserts for example to check for assumptions on function
   arguments. </li>
   <li> DONE std::vector<bool>::size_type is a central integral type here,
   determining the limits of the computation; starting from this a
   (small) resource management (at compile-time!) has to be created. </li>
   <li> DONE Of course, at least typedefs for the basic types of variables,
   literals, clauses and clause-sets need to be provided. </li>
  </ul>

*/

