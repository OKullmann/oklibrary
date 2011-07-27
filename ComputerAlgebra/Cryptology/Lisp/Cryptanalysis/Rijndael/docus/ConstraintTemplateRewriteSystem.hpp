// Matthew Gwynne, 5.10.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/ConstraintTemplateRewriteSystem.hpp
  \brief User documentation for the Maxima computer algebra system implementation of a constraint template rewrite system.

  
  \todo General idea
  <ul>
   <li> We represent a constraint system as a list (conjunction) of
   <strong>constraints</strong> which are iteratively rewritten to smaller
   and smaller constraints; finally the constraints are then translated
   individually to some problem domain (SAT, CP etc). </li>
   <li> There are three main stages in this translation:
    <ol>
     <li> <strong>Local constraint rewriting</strong>: iteratively translating
     single constraints to lists of "smaller" constraints. </li>
     <li> <strong>Global constraint propagation</strong>: propagating the
     effect of global constraints (for example variable renamings) across
     the entire constraint list. </li>
     <li> <strong>Constraint translation</strong>: translating single
     constraints directly to SAT etc. </li>
    </ol>
   </li>
   <li> See "Data types" for a list of the data-types involved in each
   stage. </li>
   <li> The main functions involved in each case are:
    <ul>
     <li> local constraint rewrite system:
      <ul>
       <li> rewrite_all_csttl: from a list of constraints, and a rewrite map
       as input, returns the results of applying the rewrite rules, given by
       the rewrite map, to the input constraints. </li>
      </ul>
      See ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateRewriteSystem.mac.
     </li>
     <li> global constraint rewrite system:
      <ul>
       <li> prop_all_csttl: from a list of constraints, a list of variables,
       and a list of propagation functions, returns a pair [csttl,var_l]
       where csttl and var_l are the constraint and variable lists after
       applying the propagations functions. </li>
       <li> prop_eq_csttl: propagation function which propagates equivalence
       constraints "eq_cst". </li>
      </ul>
      See ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateGlobalPropagation.mac.
     </li>
     <li> constraint translation:
      <ul>
       <li> csttl2cl: from a list of constraints, and a rewrite map, returns
       a clause-list representing the list of constraints; each constraint
       is translated by the rewrite rule given by the rewrite map for
       that constraint. </li>
       <li> csttl2fcl: fcl version of csttl2cl; returns a formal clause-list.
       </li>
      </ul>
      See ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateTranslation.mac.
     </li>
    </ul>
   </li>
   <li> In the case of the <em>constraint translation</em>, translation
   functions are specific to the domain being translated to; there are
   different translation functions for each different translation "target"
   (SAT,CSP etc), at both the problem and general level. </li>
   <li> In the all 3 stages, new variables can be introduced. </li>
   <li> We translate the AES using this framework; see
   Cryptanalysis/Rijndael/docus/Translations.hpp. </li>
  </ul>


  \todo Data types and concepts
  <ul>
   <li> There are 6 main data-types and concepts in the <em>local
   constraint rewrite system</em>:
    <ul>
     <li> A <strong>constraint template</strong> is a concept which defines
     a related set of constraints.
      <ul>
       <li> Constraint templates are not made concrete in code. </li>
       <li> However, we do consider constraint templates; a constraint
       has a "constraint template name". </li>
       <li> The constraint template name allows us to know what "type" of
       constraint it is. </li>
       <li> For example, "ss_sbox_cst" is the name of the small scale S-box
       constraint template. Specific S-box constraints have "ss_sbox_cst"
       as their constraint template name, and then other parameters defining
       the specific S-box, the variables and so on. </li>
      </ul>
     </li>
     <li> A <strong>namespace</strong> is a function which is applied to a
     variable, creating a new <em>unique</em> variable, which doesn't share
     it's name with any other variable in the constraint template list. </li>
     <li> A <strong>constraint</strong> is a list of at least 3
     elements:
      <ul>
       <li> the first element is the name of the constraint template as a
       string; </li>
       <li> the second element is the list of variables associated with the
       constraint; </li>
       <li> the last element is the "namespace" of the constraint. </li>
      </ul>
     A constraint L may optionally have any number of additional
     arguments in list positions 3,...,length(L)-1.
     </li>
     <li> A <strong>constraint template rewrite rule</strong> is a function
     which takes a constraint and returns a list of constraints which
     represent the input constraint. </li>
     <li> A <strong>constraint template variable rewrite rule</strong> is a
     function which takes a constraint and returns a list of new variables in
     the constraint list after applying the corresponding rewrite rule to
     the input constraint. </li>
     <li> A <strong>rewrite bundle</strong> is a triple [fr,nsp,fv] where
     fr is a rewrite rule, nsp is a namespace and fv is variable rewrite rule
     for fv. </li>
     <li> A <strong>rewrite map</strong> is a an ordered
     (i.e. list) set map which maps a given constraint template name (string)
     to a rewrite bundle. </li>
    </ul>
   </li>
   <li> There is 1 data-type in the <em>global constraint propagation
   system</em>:
    <ul>
     <li> A <strong>propagation function</strong> takes a constraint list, and
     a list of the variables in the constraint list and returns the pair of
     the new constraint and variable lists after propagating the constraint(s)
     it handles. </li>
    </ul>
   </li>
   <li> There are 4 data-types in the <em>SAT translation mapping</em>:
    <ul>
     <li> A <strong>translation function</strong> takes a constraint and
     returns a clause-list representing the constraint. </li>
     <li> A <strong> variable translation function</strong> takes a constraint
     and returns a list of the new variables in the clause-list returned by
     the associated translation function given the input constraint. </li>
     <li> A <strong>rewrite bundle</strong> is a pair [ft,fv] where
     fr a translation function and fv is variable rewrite rule
     for fv. </li>
     <li> A <strong>rewrite map</strong> is a list of rewrite bundles
     specifying the map from constraint template names to translation
     functions. </li>
    </ul>
   </li>
  </ul>


  \todo Naming conventions
  <ul>
   <li> Constraint - denoted by postfix "cst". </li>
   <li> Constraint rewrite function - denoted by "cstr" in
   function name. </li>
   <li> Constraint list - denoted by "cstl". </li>
  </ul>


  \todo Example
  <ul>
   <li> As an example, consider that one has the EVEN-PARITY constraint over
   n=4 variables:
   \verbatim
declare(even_par_ns, posfun);
declare(even_par_ns, noun);
even_par_cstt : ["even_parity",['v1,'v2,'v3,'v4], even_par_ns];
   \endverbatim
   Then "even_par_ns" would be the namespace and "even_par_cst" would be the
   constraint. </li>
   <li> We can an EVEN-PARITY constraint to three even parity constraints and
   an odd parity constraint, using new variables:
   \verbatim
EVEN-PARITY(v1,v2,v3,v4) = EVEN-PARITY(v5,v6) and EVEN-PARITY(v1,v2,v5) and EVEN-PARITY(v3,v4,v6)
   \endverbatim
   </li>
   <li> As EVEN-PARITY(v,v') is true iff v == v', we can use equality
   constraints to enforce this. </li>
   <li> Implementing this as rewrite rules:
   \verbatim
kill(even_par_ns)$
declare(even_par_ns,noun)$
declare(even_par_ns,posfun)$
even_par_split_namespace([arg_l]) := apply(nounify(even_par_ns),arg_l)$

even_par_split_cstrb :
  [even_par_split_cstr_cstl,even_par_split_namespace,even_par_split_ns_var_l]$

even_par_split_ns_var_l(cst) :=
 if length(cst[2]) <= 3 then []
 else block(
  [ns : cstt_namespace_new(even_par_split_namespace,cst)],
  [ns(1),ns(2)])$
even_par_split_cstr_cstl(cst) := block([ns, split_index],
 ns : cstt_namespace_new(even_par_split_namespace,cst),
 new_vars : even_par_split_ns_var_l(cst),
 split_index : floor(length(cst[2])/2),
 if length(cst[2]) = 2 then [["eq_cst",cst[2], ns]]
 else if length(cst[2]) <= 3 then
   [["ternary_even_parity",cst[2], ns]]
 else
   [["even_parity",
     endcons(new_vars[1],
             take_elements(split_index,cst[2])),1, ns],
    ["even_parity",
     endcons(new_vars[2], rest(cst[2],split_index)), ns],
    ["even_parity", even_par_split_ns_var_l(cst),2,ns]])$
   \endverbatim
   </li>
   <li> Using the constraint rewrite rule:
   \verbatim
cstl : rewrite_all_csttl([even_par_cstt],[["even_parity",even_par_split_cstrb]]);
[["eq_cst",
         [even_par_ns(even_par_ns(1,["even_parity",cstt_id_ns])),
          even_par_ns(even_par_ns(2,["even_parity",cstt_id_ns]))],
         lambda([a],
                lambda([a],even_par_ns(even_par_split_namespace(a,["even_parity",cstt_id_ns])))(
                 even_par_split_namespace(a,["even_parity",cstt_id_ns])))],
        ["ternary_even_parity",[v3,v4,even_par_ns(even_par_ns(2,["even_parity",cstt_id_ns]))],
         lambda([a],
                lambda([a],even_par_ns(even_par_split_namespace(a,["even_parity",cstt_id_ns])))(
                 even_par_split_namespace(a,["even_parity",cstt_id_ns])))],
        ["ternary_even_parity",[v1,v2,even_par_ns(even_par_ns(1,["even_parity",cstt_id_ns]))],
         lambda([a],
                lambda([a],even_par_ns(even_par_split_namespace(a,["even_parity",cstt_id_ns])))(
                 even_par_split_namespace(a,["even_parity",cstt_id_ns])))]]
cstl_vars : rewrite_all_cstt_vars_l([even_par_cstt],[["even_parity",even_par_split_cstrb]]);
  [even_par_ns(even_par_ns(1,["even_parity",cstt_id_ns])),
   even_par_ns(even_par_ns(2,["even_parity",cstt_id_ns]))]
   \endverbatim
   </li>
   <li> We can then remove the equivalence constraint "eq_cst", by propagating
   a renaming of even_par_ns(even_par_ns(1,["even_parity",cstt_id_ns])) to
   even_par_ns(even_par_ns(2,["even_parity",cstt_id_ns])):
   \verbatim
csttl: prop_all_csttl(cstl,append(even_par_cstt[2],cstl_vars),[prop_eq_csttl]);
  [[["ternary_even_parity",
          [v3,v4,
           veq(even_par_ns(even_par_ns(1,["even_parity",cstt_id_ns])),
               even_par_ns(even_par_ns(2,["even_parity",cstt_id_ns])))],
          lambda([a],
                 lambda([a],even_par_ns(even_par_split_namespace(a,["even_parity",cstt_id_ns])))(
                  even_par_split_namespace(a,["even_parity",cstt_id_ns])))],
    ["ternary_even_parity",
          [v1,v2,
           veq(even_par_ns(even_par_ns(1,["even_parity",cstt_id_ns])),
               even_par_ns(even_par_ns(2,["even_parity",cstt_id_ns])))],
          lambda([a],
                 lambda([a],even_par_ns(even_par_split_namespace(a,["even_parity",cstt_id_ns])))(
                  even_par_split_namespace(a,
                                           ["even_parity",1,
                                            lambda([a],
                                                   even_par_ns(
                                                    even_par_split_namespace(
                                                     a,["even_parity",cstt_id_ns])))])))]],
        [v1,v2,v3,v4,
         veq(even_par_ns(even_par_ns(1,["even_parity",cstt_id_ns])),
             even_par_ns(even_par_ns(2,["even_parity",cstt_id_ns])))]]
   \endverbatim
   </li>
   <li> The variables:
   \verbatim
even_par_ns(even_par_ns(1,["even_parity",cstt_id_ns]))
even_par_ns(even_par_ns(2,["even_parity",cstt_id_ns]))
   \endverbatim
   have been replaced with the single variable:
   \verbatim
veq(even_par_ns(even_par_ns(1,["even_parity",cstt_id_ns])),
    even_par_ns(even_par_ns(2,["even_parity",cstt_id_ns])))
   \endverbatim
   </li>
  </ul>

*/
