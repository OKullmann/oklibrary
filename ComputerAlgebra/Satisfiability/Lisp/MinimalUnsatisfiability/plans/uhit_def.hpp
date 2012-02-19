// Oliver Kullmann, 17.5.2008 (Swansea)
/* Copyright 2008, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/uhit_def.hpp
  \brief Plans regarding the catalogue of unsatisfiable non-singular hitting clause-sets


  \todo Shall uhit_def be redefined with every loading?
  <ul>
   <li> We used to have "kill(uhit_def)$" in data/uhit_def.mac. </li>
   <li> Was that really needed? </li>
  </ul>


  \todo Problems with evaluation
  <ul>
   <li> If at the time of a definition of an uhit_def-value via an expression,
   a function in that expression is not yet defined, only later, then when
   calling e.g. all_uhit_def(k) that expression is not evaluated (even though
   the function-definition is now available). </li>
   <li> Is this a Maxima-bug? I (OK) haven't encountered such behaviour
   before. </li>
   <li> A solution is to use ev(all_uhit_def(k),eval). </li>
   <li> Applied with apply_uhit and max_min_var_deg_uhit_def,
   max_min_var_deg_uhit_def_mem. </li>
   <li> Potentially this behaviour could be useful, since one could leave
   terms in the uhit_def-catalogue unevaluated, and evaluation only happens
   when needed. </li>
   <li> However it seems hard to control: for example in
   Satisfiability/Lisp/ConflictCombinatorics/testobjects/HittingClauseSets.mac
   an extra load-command had to be inserted to make the test run! </li>
   <li> An example session:
   \verbatim
(%i1) a[0]:f(0);
(%o1) f(0)
(%i2) f(x):= x+77;
(%o2) f(x):=77+x
(%i3) is(a[0] > 0);
(%o3) unknown
(%i4) is(ev(a[0]) > 0);
(%o4) true
   \endverbatim
   This seems strange behaviour (same for 5.23.2 and 5.24.0). This does not
   depend on arrays:
   \verbatim
(%i1) a:f(0);
(%o1) f(0)
(%i2) f(x):= x+77;
(%o2) f(x):=77+x
(%i3) is(a > 0);
(%o3) unknown
(%i4) is(ev(a) > 0);
(%o4) true
(%i5) is(a > 0);
(%o5) unknown
(%i6) a:ev(a);
(%o6) 77
(%i7) is(a > 0);
(%o7) true
   \endverbatim
   </li>
   <li> Another problem is that some functions used to define members of
   uhit_def are only partially given; see for example
   Lisp/MinimalUnsatisfiability/Deficiency2.mac and
   Lisp/ConflictCombinatorics/HittingClauseSets.mac. </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Rename this file to Uhit_def.hpp, and move data/uhit_def.hpp to
   MinimalUnsatisfiabilility/Uhit_def.hpp. </li>
   <li> Loading the data-set takes too long:
    <ol>
     <li> Some form of oklib_include is needed, which by default is
     off, and is turned on by some switch. </li>
     <li> See "File load and include" in ComputerAlgebra/plans/Maxima.hpp.
     </li>
     <li> Also the tests then need to be activated resp. deactivated.
     So perhaps the catalogue needs to contain data about activation status.
     </li>
    </ol>
   </li>
  </ul>


  \todo Connections to other modules
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
   </li>
  </ul>


  \todo Tests
  <ul>
   <li> Write tests for all functions.
    <ol>
     <li> uhit_def : DONE </li>
     <li> uhit_n : started </li>
     <li> apply_uhit </li>
     <li> collect_uhit_n </li>
     <li> classify_candidates_uhit_def </li>
     <li> isotype_uhit_def </li>
     <li> closure_uhit_def_pass </li>
    </ol>
   </li>
   <li> Testing functions which evaluate uhit_def:
    <ol>
     <li> Create mock-variations of uhit_def using dynamic binding. </li>
     <li> Perhaps we create a list of such mock-variations. </li>
    </ol>
   </li>
   <li> DONE
   At level basic, the test for uhit_def takes too long. </li>
  </ul>


  \todo uhit_def
  <ul>
   <li> We would like to be able to use "uhit_def[k]" for the
   list or set of all clause-sets of deficiency k in the catalogue. </li>
   <li> Yet we have all_uhit_def(k). </li>
   <li> Such accessors need to be rationalised; compare todo "uhit_n"
   below. </li>
  </ul>


  \todo uhit_n
  <ul>
   <li> info_data should be computed. </li>
   <li> def_data should be computed. </li>
   <li> Run through all deficiencies in uhit_def and check whether
   the given n is contained. </li>
   <li> To determine whether the classification is complete, use
   two functions which compute lower and upper bounds on possible
   deficiencies for a given n, and then check whether for all these
   deficiencies the corresponding entries are complete. </li>
  </ul>


  \todo Evaluating the catalogue
  <ul>
   <li> DONE (analyse_isorepo_defset_imprmvd)
   Similar to analyse_isorepo_defset_mvd we need a function,
   which runs through a repository and keeps those clause-sets
   whose min-var-degree is better than anything in the catalogue. </li>
   <li> One needs also to combine analyse_isorepo_defset_mvd and
   analyse_isorepo_defset_imprmvd. </li>
  </ul>


  \todo New entries
  <ul>
   <li> For n=5 add deficiencies delta = 14, ..., 27. </li>
   <li> It seems unlikely that we can get all for n=5. Nevertheless
   we should get a good variety, and for that we need a version of
   all_unsinghitting which starts from a different (random) path. </li>
   <li> The general policy about new entries should be that in case
   there is a large number of entries, and so we cannot have them all,
   then we should enter "interesting" examples:
    <ol>
     <li> Since we are investigating the maximal min-var-degree for a given
     deficiency (see "Maximal min-var-degrees" in
     ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp),
     examples where the bound is attained are interesting. </li>
     <li> Also of interest are ABDs. </li>
     <li> And examples for minimal and maximal n. </li>
    </ol>
   </li>
   <li> We should provide a mechanism for lazy evaluation.
    <ol>
     <li> Entries like max_var_hitting_def(58)[2] are just too big
     to compute every time with oklib_load. </li>
     <li> We could just store the terms, and by some function then trigger
     the evaluation. </li>
     <li> Ask on the Maxima mailing list about "lazy evaluation". </li>
    </ol>
   </li>
  </ul>


  \todo ExternalSources for larger data-sets
  <ul>
   <li> Larger data-sets should be made available as uhit_def*.mac
   data-sets under ExternalSources, loadable on demand.
    <ol>
     <li> These files, as ".tar.bz2"-files, are to be found under
     ExternalSources/data. </li>
     <li> What ending for these files? They are created by save, so
     apparently ".lisp" is appropriate? </li>
     <li> It is the responsibility of the user to unpack these packages. </li>
     <li> The path to the directory is known to our Maxima-system. </li>
     <li> Perhaps we provide some special load-functions. </li>
     <li> Single data sets can be specified, and also all of them. </li>
    </ol>
   </li>
   <li> Which data is already provided in uhit_def.mac ? </li>
   <li> Process n=5,6,7,8.
    <ol>
     <li> For n=5 add at least deficiencies delta = 4,5. </li>
     <li> For n=6 add at least deficiencies delta = 5,6,7. </li>
     <li> For n=7 add at least deficiencies delta = 5,6,7. </li>
     <li> For n=8 add at least deficiencies delta = 5,6,7. </li>
    </ol>
   </li>
   <li> We should integrate the catalogue of MU(k) isomorphism types from
   http://wwwcs.uni-paderborn.de/cs/ag-klbue/de/research/MinUnsat/index.html
   . </li>
  </ul>


  \todo Reduction by 2-subsumption resolution
  <ul>
   <li> A second catalogue should be created, which contains only
   those unsatisfiable hitting clause-sets for which
   no 2-subsumption resolution is possible. </li>
   <li> The structure of this catalogue is the same as of uhit_def. </li>
   <li> The subset of uhit_def is of interest (here we are also asking
   for non-singularity). Call it uhit_def_nsn2s.
    <ol>
     <li> Via
     \verbatim
sublist(apply_uhit(lambda([k,n,i,F],[k,n,i,redtsrp(F)])),lambda([T],T[4]));
     \endverbatim
     we obtain the current examples. </li>
     <li> We should get much smaller numbers. </li>
    </ol>
   </li>
   <li> And also the bigger catalogue uhit_def_n2s, where non-singularity
   is dropped, is of interest.
    <ol>
     <li> The question is whether for given deficiency there are only
     finitely many cases in uhit_def_n2s?! </li>
     <li> From this catalogue all of uhit_def can be created by inverse
     2-subsumption resolution. </li>
     <li> A representation involves an element F of uhit_def_n2s, a
     clause-list, and then a partial map from clause-indices to variable-sets,
     where the latter specify the expansion. </li>
     <li> The process of reduction by 2-subsumption-resolution
     is not confluent, and for a given (non-singular) hitting clause-set
     F there can exist 2 non-isomorphic hitting clause-sets reduces w.r.t.
     2-subsumption-resolution which can be expanded to F.
      <ol>
       <li> Obvious examples are given by reducing full_fcs(n). </li>
       <li> For full_fcs(n) there exist many representations, the optimal
       representing full_fcs(n) as the expansion of {{}} by setn(n). </li>
       <li> One could use shortest representations. </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>

*/

