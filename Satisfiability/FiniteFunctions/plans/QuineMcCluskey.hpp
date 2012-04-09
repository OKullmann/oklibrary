// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp
  \brief Plans regarding the direct implementation of the computation of the minimal elements of the resolution closure of full boolean clause-sets


  \todo Connections
  <ul>
   <li> "Computations of all prime clauses for full clause-sets" in
   ComputerAlgebra/Satisfiability/Lisp/Primality/plans/general.hpp is the
   current general hub. </li>
   <li> "min_2resolution_closure_cs" in
   Satisfiability/Lisp/Primality/plans/PrimeImplicatesImplicants.hpp
   discusses the (current) Maxima functionality. </li>
  </ul>


  \todo Specify the order of the output-clauses
  <ul>
   <li> As usual, this needs to be synchronised with the Maxima-specification.
   </li>
  </ul>


  \todo Understanding the costs of the current implementation
  <ul>
   <li> What is the difference between the empty clause-set and the
   clause-set containing all clauses (both for the same number n of variables)
   in running-time for the current implementation? </li>
   <li> Understanding this shows us how much time is spent on merely running
   n times through all 3^n clauses (for 0 clauses we never have to
   rank/unrank, while otherwise we do it for all clauses). </li>
   <li> We should find out empirical functions f_0(n) and f_1(n) for these
   run-times (depending on one computer-dependent constant). </li>
   <li> For the current implementation, using both empty clause-sets
   and full clause-sets with n variables for each n, we get:
   \verbatim
   n  empty    full
1  1 0.0000  0.0000
2  2 0.0000  0.0000
3  3 0.0000  0.0000
4  4 0.0000  0.0000
5  5 0.0000  0.0000
6  6 0.0000  0.0000
7  7 0.0000  0.0000
8  8 0.0000  0.0000
9  9 0.0000  0.0000
10 10 0.0000  0.0205
11 11 0.0100  0.0955
12 12 0.0525  0.3175
13 13 0.1780  1.0510
14 14 0.5705  3.4200
15 15 1.8275 11.2480
16 16 5.7935 36.3640
   \endverbatim
   </li>
   <li> MG needs to add instructions on how to reproduce this. </li>
   <li> Also information is needed on the computer and on the version of
   Quine-McCluskey. At least in the Git-history we want to see how
   running-times evolve over time. </li>
  </ul>


  \todo Running only through the levels
  <ul>
   <li> Yet the implementation runs for every k = n, ..., 0 through all 3^n
   indices. </li>
   <li> Now for every k we only want to run through the clauses of length k.
   </li>
   <li> This should be faster (though that depends on how it is handled). </li>
   <li> One could use a new hash function, with contiguous intervals for
   clauses of the same length, or one could use a more complicated jump to
   the next index-value. </li>
   <li> This needs to be done first at Maxima-level. </li>
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
     Satisfiability/Lisp/Primality/PrimeImplicatesImplicants.mac)
     has the potential advantage that it runs asymptotically faster
     if the input is has few clauses (relative to the number of variables).
     </li>
     <li> See "min_2resolution_closure_cs" in
     Satisfiability/Lisp/Primality/plans/PrimeImplicatesImplicants.hpp
     for the question about the (precise) complexity of QuineMcCluskey
     (in principal). </li>
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


  \todo Extend unit tests
  <ul>
   <li> Many more unit tests are needed. </li>
   <li> Also for the various member functions. </li>
  </ul>


  \todo pow3 should be replaced by an array computed at compile-time
  <ul>
   <li> Once we use gcc version 4.5.2 with option "-std=c++0x", then we can
   simply using the following instead of the current initialisation of pow3:
   \code
...
struct QuineMcCluskey {
  ...
  const int max_num_vars = 24;
  BOOST_STATIC_ASSERT(num_vars <= max_num_vars);
  const hash_index_type pow3[max_num_vars+1];
  ...
  QuineMcCluskey() : pow3({1,3,9,27,81,243,729,2187,6561,19683,59049,
  177147,531441,1594323,4782969,14348907,43046721,129140163,387420489,1162261467,3486784401,
  10460353203,31381059609,94143178827,282429536481}) {}
   \endcode
   </li>
   <li> This is by far easiest, and the appropriateness of type hash_index_type
   is checked at compile-time (whether 3^n fits into it). </li>
   <li> Value of max_num_vars:
    <ol>
     <li> If needed, max_num_vars is increased (but much won't be needed).
     </li>
     <li> Perhaps a macro MAX_NUM_VARS is needed for the value of max_num_vars,
     so that max_num_vars could also be decreased if hash_index_type has only
     32 bits. </li>
     <li> Using the new macro facilities, it should be easy to write a macro
     for the initialiser-expression for pow3 (depending on max_num_vars). </li>
     <li> If MAX_NUM_VARS is not defined, then compute max_num_vars at
     compile-time such that it is maximal w.r.t. fitting into hash_index_type.
     </li>
    </ol>
   </li>
   <li> Const-ness of pow3(x):
    <ol>
     <li> The above seems to be the most "const" version of access to a
     function like pow3(x), where x might be a run-time variable. </li>
     <li> Though it might still be that e.g. pow3[5] is not recognised as
     a compile-time constant? </li>
     <li> One needs to check the C++0x-standard, what it says about constant
     expression (there are new possibilities). </li>
    </ol>
   </li>
   <li> Should pow3 be static?
    <ol>
     <li> One could also make pow3 static; then the definition of pow3
     (out-of-class) would become a bit more clumsy. </li>
     <li> By making pow3 static, one could make all member functions static;
     seems appropriate. </li>
    </ol>
   </li>
  </ul>


  \todo Improvements of the implementation
  <ul>
   <li> See "Computing all prime clauses for a full clause-set" in
   Experimentation/Investigations/BooleanFunctions/plans/QuineMcCluskey.hpp
   for investigations. </li>
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


  \todo Parallelisation
  <ul>
   <li> Usage of Mpi (now supported by gcc) or of threads (according to
   C++09) is to be studied. </li>
   <li> So that tackling really big tasks becomes possible. </li>
  </ul>


  \todo Precise definitions, and basic types
  <ul>
   <li> Use asserts for example to check for assumptions on function
   arguments. </li>
   <li> It needs to be determined whether the current checks against
   improper prerequisites (domains of types etc.) are sufficient. </li>
   <li> Perhaps more compile-time checks are needed? </li>
  </ul>

*/

