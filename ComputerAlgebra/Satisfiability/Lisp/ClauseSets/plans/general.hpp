// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp
  \brief Plans for Maxima-components regarding clause-sets


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo Notions for clauses, clause-sets etc.
  <ul>
   <li> See "Better general naming conventions" in
   ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp
   for the general naming-discussion. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/plans/Literals.hpp
   for general discussions on "literals". </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/plans/Clauses.hpp for
   general discussions on "clauses". </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/plans/PartialAssignments.hpp
   for general discussions on "partial assignments". </li>
   <li> All variations on clause-sets should still be "syntactical objects",
   so the discussion in
   ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp
   is not relevant yet. </li>
   <li> At the base level we have
    <ol>
     <li> "clause-sets" as now; </li>
     <li> "ordered clause-sets", repetition-free lists of clauses; </li>
     <li> "clause-lists" (lists of clauses). </li>
    </ol>
   </li>
   <li> "Formal" versions (just adding variables):
    <ol>
     <li> "formal clause-sets" as now; </li>
     <li> "ordered formal clause-sets"; </li>
     <li> "formal clause-lists". </li>
    </ol>
    Additionally, the formal clause-sets also exist as "formal
    multi-clause-sets" and "formal labelled clause-sets".
   </li>
   <li> Should we also allow "multi-clause-sets" and "labelled clause-sets"
   without the variables?
    <ol>
     <li> Otherwise the "formal" in this context is superfluous. </li>
     <li> On the other hand, the multi- and labelled versions are introduced
     in the graph-theoretical context, and there we always have the set
     of vertices given (except of set-systems). </li>
    </ol>
   </li>
   <li> "Multi-clause-sets":
    <ol>
     <li> A triple [V,F,c] s.t. [V,F] is a formal clause-set and c: F -> NN.
     </li>
     <li> Accordingly "ordered multi-clause-sets". </li>
     <li> However, perhaps the clause-function is better defined on all
     possible clauses over V (and returns 0 for clauses which are not
     contained. </li>
    </ol>
   </li>
   <li> Labelled clause-sets (not "general clause-sets" to avoid confusion)
    <ol>
     <li> A "labelled clause-set" is a triple [V,F,f], where V is a
     set of variables, F a set of clause-labels, and f assigns to each
     element of F a clause over V. </li>
     <li> Accordingly, "ordered labelled clause-sets are triples
     [V,F,f], where now V,F are repetition-free lists. </li>
     <li> DONE
     Labelled clause-sets should always have sets of variables, so
     we can drop the "formal". </li>
    </ol>
   </li>
   <li> Relations to hypergraphs (see
   ComputerAlgebra/Hypergraphs/Lisp/plans/general.hpp):
    <ol>
     <li> fcs <-> hg, ofcs <-> ohg </li>
     <li> mucs <-> muhg, omucs <-> omuhg </li>
     <li> lcs <-> ghg, olcs <-> oghg. </li>
    </ol>
    Additionally we have
    <ol>
     <li> "cs" corresponds to "set-systems" </li>
     <li> "ocs" corresponds to "ordered set-systems" </li>
     <li> "cl" corresponds to "lists of sets". </li>
    </ol>
    A somewhat subtle point: clause-sets here are the only type of
    objects which are deeply copied (as sets!), while the rest is, since we
    have lists, copied in the shallow way.
   </li>
   <li> Formal clause-lists (fcl) have no correspondence at the hypergraph
   side:
    <ol>
     <li> If we have a list V of vertices and a list E of hyperedges, then
     we use the ghg [V,E,identity]. </li>
     <li> Seems alright. </li>
     <li> So perhaps also for clause-sets we should abandon "fcl" (but keep
     "cl"), since we have already "lcs". </li>
     <li> On the other hand, a formal clause-list seems to be a very natural
     concept, perhaps the most natural one from the point of view of SAT
     solving, so we should keep it. </li>
    </ol>
   </li>
   <li> Generalised literals:
    <ol>
     <li> Then we have the "non-boolean" versions, where literals are pairs
     [v,e], with e a value. </li>
     <li> And there a "power" versions, where then e is a set of values. </li>
     <li> The "monosigned literals" resp. "signed literals" are corresponding
     triples, with the third component in {-1,+1}. </li>
    </ol>
   </li>
   <li> A problem with non-boolean variables is, where to put the information
   about the domain of the variables:
    <ol>
     <li> See the discussions in
     ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp.
     </li>
     <li> Perhaps it's not part of clause-sets etc., but only part of
     "problems" given for example to SAT-solvers. </li>
     <li> So a signed clause-set etc. would always need to be accompanied
     by either a uniform domain, or by a domain function. </li>
     <li> But perhaps we should use pairs [FF, D], where D is a set,
     list or map. Perhaps the default for all types of clause-sets is
     a uniform domain. </li>
     <li> But since we can also use sensibly non-boolean clause-sets without
     the domain information (for example it is not needed in order to apply
     a partial assignment), we should use the (additional) suffix "ud" for
     uniform domain and "fd" for function domain. </li>
     <li> There is also the idea that a "domain association" is basically
     a partial assignment; actually it should be a "total partial assignment".
     </li>
     <li> One could allow then suffixes like "tpa_mp". </li>
    </ol>
   </li>
   <li> Conversions:
    <ol>
     <li> "cs2cl": from clause-sets to clause-lists </li>
     <li> Likely "cs_to_fcs" should become "cs2fcs". </li>
    </ol>
   </li>
  </ul>


  \todo Organisation
  <ul>
   <li> Inclusion
    <ol>
     <li> Currently, the outsourced files are "inclusions-wise equivalent"
     to ClauseSets/BasicOperations.mac, i.e., they include it and are
     automatically included. </li>
     <li> This is because it is too much work to sort out the real dependencies
     for all the Maxima-files in the library. </li>
     <li> It would be good to have a refactoring tool, which would find out
     about the dependencies:
      <ul>
       <li> If it is too hard to find out about the functions (and names in
       general) defined in a file, that it needs to explicitly given as
       input. </li>
       <li> And then we can just search for files using some of these
       functions, whether they correctly include them. </li>
       <li> And we search for files which use some of them but don't include
       it. </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> DONE
   We should have "Substitutions.mac":
    <ol>
     <li> See "Applying substitutions" in
     ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp. </li>
     <li> Everything currently under "Substitution and renaming" goes there.
     </li>
    </ol>
   </li>
   <li> DONE
   We should create "Statistics.mac".
    <ol>
     <li> DONE : move. </li>
     <li> A problem is now how to handle inclusion. </li>
     <li> For the moment, "Statistics.mac" and "BasicOperations.mac" are
     inclusions-wise equivalent. </li>
    </ol>
   </li>
   <li> See "Input and output" below. </li>
   <li> DONE We should create "PartialAssignments.mac". </li>
  </ul>


  \todo Input and output
  <ul>
   <li> See Interfaces/InputOutput/plans/Dimacs.hpp. </li>
   <li> Functions currently in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac
   should move to their own file "InputOutput.mac". </li>
   <li> The usage of "print" likely should be replaced by "printf" from
   package "stringproc". </li>
   <li> We should have options for output:
    <ol>
     <li> Instead of for example "php(3,2)" print out "php_3_2". </li>
     <li> DONE And also print it out in strict Dimacs format, optionally
     with the mapping from natural numbers to original variables given
     in the comments. </li>
    </ol>
   </li>
   <li> We need also reading from Dimacs-files.
    <ul>
     <li> A simple function for reading from a standard "cnf" DIMACS file might
     be:
     \verbatim
oklib_plain_include("stringproc")$

read_fcs_f(n) := block([fh, line, ll, cs : [], l,b,c],
  fh : openr(n),
  while stringp(line : readline(fh)) do (
    ll : tokens(line),
    if length(ll) >= 1 then
      if not(first(ll) = "c" or first(ll) = "p") then
        cs : cons(setify(rest(map(parse_string,ll),-1)), cs)
  ),
  cs : setify(cs),
  return(cs_to_fcs(cs))
)$
     \endverbatim
     </li>
     <li> The above function should better return a clause-list. </li>
     <li> Perhaps even more appropriate a formal clause-list, where the
     set of variables reflects the n-value from the p-line. </li>
     <li> Maxima likely has no parsing capabilities, but there should be
     Lisp-libraries for parsing?
      <ol>
       <li> It would be interesting to explore them (they should be useful
       for many occassions). </li>
      </ol>
     </li>
     <li> What to do with syntax checking?
      <ol>
       <li> Typically, at the Maxima level we do not check inputs. </li>
       <li> However we do provide (dedicated) tests. </li>
       <li> So we should write a syntax checking functions. </li>
      </ol>
     </li>
     <li> How to test this function?
      <ol>
       <li> Most appropriate seems to use an application test. </li>
      </ol>
     </li>
     <li> Additionally, there is the possibility of reading Extended DIMACS 
     files, where the variables might be non-integer names. In such a case
     there seem to be several possibilities
      <ul>
       <li> The file is read into a normal formal clause set, and the variables
       are treated directly as maxima nouns, where they are also declared 
       "posfun" etc, so that operations such as "abs" and so on work correctly.
       </li>
       <li> The file is read into a formal clause set, but the variables are
       translated to integer variables with the mapping from the variable name
       to the integer variable being returned in a hash or setmap (so a pair of
       the fcs and setmap is returned). </li>
       <li> Most appropriate seems to me to use the (formal) function "edim"
       (for "extended Dimacs"), which is used with one string-parameter (
       the variable-name). </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>

*/

