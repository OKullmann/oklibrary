// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/VanderWaerdenProblems.hpp
  \brief Plans for Maxima-generators related to van der Waerden problems (and generalisations)


  \todo Connections
  <ul>
   <li> See Satisfiability/Transformers/Generators/plans/VanderWaerden.hpp
   for general plans at C++ level. </li>
   <li> See
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp
   for experiments with vdW-problems. </li>
   <li> See Satisfiability/Transformers/Generators/ and
   Satisfiability/Transformers/Generators/plans/general.hpp for several
   generators for vdW- and GT-problems. </li>
  </ul>


  \todo Organisation : DONE
  <ul>
   <li> DONE
   Likely we should create a submodule "Lisp/Generators/RamseyTheory".
   </li>
   <li> DONE
   There then the Green-Tao functions should go into a separate file.
   </li>
   <li> DONE
   Perhaps we should split the module, as we have it now with
   Investigations/RamseyTheory; a question whether we should use
   "Problems" as part of the module names or not (as in
   ComputerAlgebra/RamseyTheory). </li>
  </ul>


  \todo Standardisation
  <ul>
   <li> As we have it now in output_vanderwaerden, standardisation for
   all non-standardised clause-sets should be done explicitly. </li>
  </ul>


  \todo Statistics
  <ul>
   <li> As discussed in "Accompanying statistics" in
   Satisfiability/Lisp/Generators/plans/general.hpp in general, we need
   statistics for all main measurements (and also for all others, if
   possible). </li>
   <li> DONE
   First the hypergraph measurements needs to be established; see
   "Statistics" in Hypergraphs/Lisp/plans/Generators.hpp. </li>
  </ul>


  \todo Systematic output and output-functions
  <ul>
   <li> Likely, the outputs for van-der-Waerden and for Green-Tao problems
   should be very similar. </li>
   <li> Regarding the naming of files, for the general (non-boolean, non-
   diagonal) form we should provide another form which translates for
   example [2,2,2,2,3,3,4] in "_7_2:4_3:2_4". </li>
   <li> DONE
   What about the diagonal forms? Yet output_greentao2_stdname uses
   the length of the arithmetic progression in the filename twice,
   while output_greentaod_stdname writes out that length just once. This
   seems better. </li>
   <li> The part "_sb" in the name (for symmetry-breaking) should perhaps
   be placed at the very end. </li>
   <li> The output-function should use the clause-list-forms, not the
   clause-set-forms. </li>
  </ul>


  \todo Palindromic versions with arithmetic progressions of length 3
  <ul>
   <li> For progression length k=3 and odd n, if the middle point is
   included, then all other points must be excluded (otherwise one gets
   an arithmetic progression). </li>
   <li> This follows by r_2-reduction, but might pose a problem for some
   solver, so perhaps should be supplied by the translation. </li>
  </ul>


  \todo Alternative handling of parameter-values 2
  <ul>
   <li> A parameter value 2 means that the corresponding part can contain
   at most one element. </li>
   <li> Currently this gets (automatically) translated using the standard
   representation of AMO for the vertices, using binomial(n,2) binary clauses
   (since two different vertices make always a progression of length 2). </li>
   <li> An alternative is presented in "At-most-one as a CNF" in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/Thresholds.hpp,
   and one should try this. </li>
   <li> We should also provide standard generators for these two
   AMO-representations. </li>
   <li> Another alternative generalises the treatment of the transversal
   case (all parameters except of one equal 2):
    <ol>
     <li> Let the parameter tuple be [2,..,2,t], where t contains no 2, and
     where we have p 2's. And let the length of t be q (so that the length
     of the whole tuple is p + q). </li>
     <li> One could use non-boolean variables with q+1 values, where
     we have on the first q values the ordinary constraints coming from t,
     while using some additional parameter B for the first value we
     add the condition that the number of variables with that value is
     equal to B. </li>
     <li> B is here the number of variables *not* available to satisfy the
     t-requirements. </li>
     <li> The task is to maximise B; using as generalised transversal number
     tau(F) of an arbitrary satisfiable clause-set F the minimum of n(phi)
     for satisfying partial assignments phi, this corresponds to compute
     tau(F) = n - B. </li>
     <li> Such generalised transversal problems (for arbitrary F) can be
     handled as above, by introducing the additional value to indicate
     variables not to be used. </li>
     <li> However, just having negative monosigned literals is not enough,
     but we need to consider monosigned literals. And actually, since for
     q >= 3 negative monosigned literals must be used w.r.t. t, we have a
     problem w.r.t. handling the value "undefined"! Perhaps an extension
     of monosigned literals is needed, allowing to express "v<>val but
     v is defined" ?! For q=2 the t-part can be expressed using positive
     monosigned literals, and so monosigned literals are altogether
     enough here. <li>
     <li> See "Parameterisation" in
     Algorithms/AllSolutions/plans/MinimalAssignments.hpp for general plans
     at the C++ level. </li>
    </ol>
   </li>
  </ul>


  \todo DONE
  More than two parts
  <ul>
   <li> Create non-boolean clause-sets for the van der Waerden problems and
   the Green-Tao problems with more than two parts. </li>
   <li> Compare "Generalised colouring problems" in
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/general.hpp. </li>
  </ul>


  \todo Symmetry breaking for vdW-problems
  <ul>
   <li> The symmetry-breaking should happen as with the Green-tao problems,
   only that the distinguished vertex is a central one. </li>
   <li> This needs to be investigated; it seems that floor(n/2)-1 is
   always a vertex with maximal degree. </li>
  </ul>

*/
