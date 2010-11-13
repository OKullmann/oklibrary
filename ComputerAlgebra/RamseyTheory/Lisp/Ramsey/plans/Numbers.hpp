// Oliver Kullmann, 26.11.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/Numbers.hpp
  \brief Plans regarding %Ramsey numbers


  \bug Incorrect parameter checking
  <ul>
   <li> The current implementation of ramsey_p is obviously incorrect. </li>
   <li> DONE The parameter transformations should be informative, and thus 
   should not use ramsey_p (they should be understandable on their own, and 
   should exactly specify the case). </li>
   <li> DONE All implementations of parameter-transformers (like 
   "ramseytrivle_a") are incorrect: Every such function must work on 
   every possible list. </li>
   <li> DONE Before performing the correction, first the tests need to be
   updated to detect the errors (as usual). </li>
  </ul>


  \todo Weak specifications
  <ul>
   <li> There are often no explanations about the meaning of the different
   cases. </li>
   <li> The "_a"-functions need a general explanation about their usage,
   and then their specification should just state (as clearly as possible)
   the special case handled. </li>
  </ul>


  \todo Write docus

  
  \todo Add known %Ramsey bounds
  <ul>
   <li> Some bounds are in the current system but all bounds known should be
   represented in the system. </li>
   <li> See the running report of [Radziszowski, Small %Ramsey Numbers]. </li>
   <li> Also in the eis-database we should find Ramsey-numbers. </li>
   <li> References for the bounds; compare "Bibtex-database" in
   OKlib/plans/Annotations.hpp. </li>
  </ul>


  \todo Proofs of numbers
  <ul>
   <li> The question here is how to provide a method of extracting a proof
   of the given bounds, as there are various different methods:
    <ol>
     <li> Trivial bounds based on reasoning regarding small parameter values
     </li>
     <li> Proven bounds with references. </li>
     <li> Bounds (not implemented). </li>
    </ol>
   </li>
   <li> How to handle annotation with references for bounds? </li>
  </ul>
  

  \todo %Ramsey numbers
  <ul>
   <li> DONE
   The following needs to be updated according to the general naming
   conventions discussed in "Systematic notations for the numbers in %Ramsey
   theory" in ComputerAlgebra/RamseyTheory/plans/general.hpp.
    <ol>
     <li> For a "Ramsey parameter tuple" as below, the number of "colours" is
     not mentioned, while the size of the hyperedges comes last. </li>
     <li> Perhaps we should just put the size of the hyperedges as
     first component, but leave it otherwise. </li>
     <li> So the tuple of clique-sizes is always written out (while
     the redundant information on the number of colours then is not
     needed). </li>
     <li> Or should be provide the (redundant) number of colours?
     Then it would use the same conventions as the function-arguments? </li>
     <li> Should we then also allow the hyperedge-size to be left out
     (which implies the value 2)? </li>
     <li> The current tuple [[3,3,3],2] would then become [3,2,[3,3,3]]
     or [3,[3,3,3]]. But there such defaults seem useless, so perhaps
     we should just use the full version. </li>
     <li> Leaving out the number of colours yields [2,[3,3,3]]; perhaps
     this is best. </li>
    </ol>
   </li>
   <li> DONE
   A "Ramsey parameter tuple" is a tuple [r, [q_1,...,q_s]], where s
   is the number of colours (all q_i and r are natural numbers). </li>
   <li> The function ramsey([r, par_tuple]) computes a pair, consisting of
   a lower and an upper bound on the %Ramsey number for this tuple. </li>
   <li> This function is now available, but needs to be improved:
    <ul>
     <li> The known general upper and lower bounds need to be integrated:
      <ol>
       <li> The system must be extensible. </li>
       <li> Perhaps one "bound function" delivers:
        <ul>
         <li> the function which from the given parameter tuple computes
         the other parameter tuple for which bounds are needed </li>
         <li> and the function which from these other bounds computes
         the bounds. </li>
        </ul>
        Then ramsey recursively invokes all registered bound functions,
        and uses minimum resp. maximum.
       </li>
       <li> Either we provide pairs [lower bound, upper bound], or precise
       values (just integers). </li>
       <li> DONE (ramsey_hm has been removed)
       The system needs to general enough, so that also the values given
       by ramsey_hm can be checked (possibly improved).
        <ol>
         <li> It might be good to extend the hash-map ramsey_hm to contain
         information on how currently the best bounds are obtained. </li>
         <li> One possibility is for the interesting small parameter values
         just to use comments. </li>
        </ol>
       </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> Important that the system developed here can later be generalised to
   yield bounds on all sorts of combinatorial parameters.
    <ol>
     <li> Compare
     ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/uhit_def.hpp
     for another catalogue of data (and instances). </li>
    </ol>
   </li>
  </ul>


  \bug DONE Incorrect handling of Ramsey parameter tuples
  <ul>
   <li> DONE The superfluous parameter r needs to be removed everywhere
   (except, of course, where explicitly diagonal-versions are treated). </li>
   <li> DONE It is actually not clear at all what "r" stands for: standardisation
   of the names used ("k" and "r"; perhaps others?) is needed. </li>
   <li> DONE At this time also the testobjects-file needs a make-over: The
   editorial structure of tests- and testobjects-files should always be the
   same as the basic file (for which tests are provided). </li>
   <li> DONE And specifications for most functions in
   ComputerAlgebra/RamseyTheory/Lisp/Ramsey/Numbers.mac are missing. </li>
   <li> DONE The function "ramsey" should take as parameter exactly a Ramsey
   parameter tuple. </li>
   <li> DONE The predicate "ramsey_p" is needed. </li>
  </ul>

*/

