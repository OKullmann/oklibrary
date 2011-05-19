// Oliver Kullmann, 17.6.2008 (Swansea)
/* Copyright 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/InverseSingularDP.hpp
  \brief Plans on transforming clause-sets by applying inverse singular DP-reduction


  \todo Transfer from Orthogonal.mup
  <ul>
   <li> See ComputerAlgebra/Mupad/plans/general.hpp. </li>
   <li> MUSAT1 </li>
   <li> SMUSAT1 </li>
   <li> MMUSAT1 </li>
   <li> DONE (si_inverse_singulardp_fcs; M -> FF, k -> b, p, q as before)
   SingExt </li>
   <li> DONE (it_si_inverse_singulardp_fcs)
   ISingExt </li>
  </ul>


  \todo More intelligent iteration
  <ul>
   <li> Compare "Randomised extensions" below. </li>
   <li> The function it_si_inverse_singulardp_fcs is very simple,
   and perhaps we can provide more intelligent functionality. </li>
   <li> The problem is that the parameters p,q,a,b (where "a" now
   generalises the old variable "nonempty") are fixed in this approach.
   </li>
   <li> "Typically", at the beginning only very low values of
   a are possible (if b > 1). </li>
   <li> So perhaps a kind of gradient is introduced. </li>
   <li> Or we try to fullfil the given values of a, b (while p,q are
   fixed), and if not possible, then we look at the "best"
   approximation. </li>
   <li> Different strategies for such a "best approximation" are
   possible, lowering b, lowering a, or both. </li>
  </ul>


  \todo Creation of random subsets:
  <ul>
   <li> Looks as if random_sublist, random_subset_si should go to
   the submodule on set-systems (or hypergraphs?). </li>
   <li> random_subset_si is very inefficient:
    <ol>
     <li> If the number of s-subsets is large, then the dual approach
     can be (much) more efficient, searching through all i-subsets
     of literals which have a common occurrence and checking whether
     there are actually s such common occurrences. </li>
    </ol>
   </li>
  </ul>


  \todo We want three types of extensions:
  <ul>
   <li> DONE (arbitrary p,q-values)
   The non-degenerated extensions, characteristic for singular DP-reductions
   on MU. </li>
   <li> The saturated extensions, characteristic for singular DP-reductions
   on SMU.
    <ol>
     <li> The condition is that, additionally to p=q=1, for the main clause,
     given by I in the implementation, there is no other clause, not involved
     in the extension, such that I subsumes this clause (one could then
     add -v to this clause). </li>
     <li> First we just implement this condition, via
     "basic_inverse_saturated_singulardp_fcs(FF,G)", and return false if the
     condition is not fulfilled. </li>
    </ol>
   </li>
   <li> The hitting extensions, characteristic for singular
   DP-reductions on UHIT.
    <ol>
     <li> These are special saturated extensions. </li>
     <li> The additional criterion is that the intersection must clash with
     all old clauses except of the choosen one. </li>
     <li> Here we have special problems with the control, since in most
     cases such an extension won't be possible. </li>
    </ol>
   </li>
  </ul>


  \todo All isomorphism types of extensions
  <ul>
   <li> We should have systematic versions, which try to generate
   all isomorphism types for a given number of steps. </li>
   <li> Just one step:
    <ol>
     <li> All MU-extensions are given by
      <ul>
       <li> all non-empty sub-clause-sets </li>
       <li> for the main clause all possibilities between empty
       and "saturated" </li>
       <li> then for each side clause as possibilities between
       the needed literals and all. </li>
      </ul>
     </li>
     <li> The isomorphism types are obtained by removing all isomorphic
     cases. </li>
     <li> All SMU-extensions are given by all non-empty sub-clause-sets
     which full the additional condition, that their intersection does
     not subsume any other clause. </li>
     <li> All UHIT-extensions are given by those SMU-extensions which
     full the condition that their intersection clashes with all
     other clauses. </li>
    </ol>
   </li>
   <li> Having all MU-, SMU or UHIT-extensions, one can then choose
   a random extension, w.r.t. all extensions or all isomorphism types.
   </li>
   <li> W.r.t. MU-extensions, this seems infeasible, but there should be
   substantiallly less SMU- and UHIT-extensions, so that here it should
   be interesting to choose a random SMU-extension resp. a random
   UHIT-extension (either treating all extensions equally likely, or
   treating all isomorphism types as equally likely). </li>
   <li> Are there more intelligent algorithms to compute all SMU or all
   UHIT extensions?
    <ol>
     <li> For a bit more than ten clauses the simple enumeration should
     be alright. </li>
     <li> We should reduce the problem of finding suitable types of
     extensions to other problems (of course, SAT, etc.). </li>
     <li> Especially UHIT-extensions should be interesting.
      <ul>
       <li> There is always one, introducing a unit-clause, but the rest
       might not exist. </li>
       <li> One should first compute the conflict matrix. </li>
       <li> Then the common neighbours yield first approximations of what
       is possible. </li>
      </ul>
     </li>
     <li> One could also investigate fixed parameter tractability. </li>
    </ol>
   </li>
  </ul>


  \todo Randomised extensions
  <ul>
   <li> Compare todo "More intelligent iteration" above. </li>
   <li> Attractive is, as discussed in todo "All types of extensions",
   to choose amongst all possible extensions. </li>
   <li> The two basic problems are
    <ol>
     <li> a random extension in one step </li>
     <li> the repetition of this process. </li>
    </ol>
    If parameter for a single random extension are involved, then in the
    course of the iteration these parameters might evolve.
   </li>
   <li> For MU-extensions likely to choose amongst all possibilities is
   infeasible, and thus one needs to introduce parameters.
    <ol>
     <li> p, q as we have it. </li>
     <li> For the size b of the sub-clause-set one can choose any probability
     distribution. </li>
     <li> Given the size, we choose a random such subset. </li>
     <li> And for "a" we choose another probability distribution. </li>
     <li> The problem then is that a,b might be inconsistent. </li>
     <li> It would be easier to first choose b, then determine the
     intersection and let a be the size of the intersection. </li>
     <li> Then however in many cases the intersection might be empty
     or very small. </li>
    </ol>
   </li>
  </ul?


  \todo Investigations
  <ul>
   <li> The first questions concern the characterisation of MU(delta=2),
   SMU(delta=2) and UHIT(delta=2) (always looking at all clause-sets, not
   just the non-singular one). </li>
   <li> For the two non-singular elements of UHIT(delta=2) we should try
   to obtain all isomorphism types of extensions for a number of steps. </li>
   <li> Then we can try to guess the isomorphism types of all of
   UHIT(delta=2).
    <ol>
     <li> The simplest guess is a triple (t,a,b), where t in {2,3} is the
     major isomorphism type (whether reduction yields musatd2_fcs(2) or
     musatd2_fcs(3)), where a is the number of variables of degree >= 3,
     and where b is the number of variables of degree 2. </li>
     <li> We need a catalogue where the known examples are entered. </li>
     <li> This catalogue could contain lists of known isomorphism types
     for each (t,a,b). </li>
    </ol>
   </li>
   <li> We know them already if the min-var-degreee is at least 3. Here
   we should write generators. </li>
  </ul>

*/

