// Matthew Gwynne, 5.10.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/ConstraintTemplateRewriteSystem.hpp
  \brief User documentation for the Maxima computer algebra system implementation of a constraint template rewrite system.

  
  \todo General idea
  <ul>
   <li> The notion of a constraint template rewrite system is that
   one has a list of constraint templates (see "Data types") modelling
   the conjunction of a set of constraints, and then using the arguments
   and variables provided within each constraint template, the system
   recursively applies a series of "constraint template rewrite rules"
   which translate a constraint template into a list of constraint
   templates. </li>
   <li> Such a system allows a single large constraint template to be
   rewritten into a system of much smaller component constraint template
   and then further translation systems used to translate this
   list into something like a CNF clause-set. </li>
  </ul>


  \todo Data types
  <ul> 
   <li> There are four main data-types within the constraint template
   rewrite system, namely
   <ul>
    <li> namespaces </li>
    <li> constraint templates </li>
    <li> rewrite maps </li>
    <li> constraint template rewrite rules </li>
   </ul>
   where 
   <ul>
    <li> A <strong>namespace</strong> is simply a function which takes a 
    variable and returns a
    variable. This function need not actually evaluate, and in fact, this
    is precisely the point in the majority of cases. The key idea with the
    namespace is that when introducing new variables in during any translation
    process, one wishes to ensure variables do not clash, therefore at all
    levels, namespaces are introduced, which are then applied to variables,
    such that a variable is uniquely identified by the named given to it at
    a particular point in the translation, as well as the nested application
    of namespace functions. For the standard namespaces constructed in this
    system see cstt_namespace_new. </li>
    <li> A <strong>constraint template</strong> is a list of at least 3 
    elements, where the first
    element is the name of the constraint template, the second element is the
    list of variables associated with the constraint template and the last
    (note, not the third) is the namespace of the function. A constraint 
    template may optionally have any number of arguments situated in between 
    it's 2nd and last elements which will relate to the working of each 
    constraint template. </li>
    <li> A <strong>constraint template rewrite rule</strong> is a function 
    which takes a constraint template and returns a list of constraint templates
    which model the input constraint template. Note here that new variables can 
    be introduced using the namespace of the original constraint template 
    composed with a new namespace constructed for the new constraint templates. 
    </li>
    <li> A rewrite bundle is a list with two elements, where the first element
    is a constraint template rewrite rule f, and the second element is a function 
    which, given a constraint template, returns a list of variables introduced 
    by f. </li>
    <li> A <strong>constraint rewrite map</strong> is a an ordered (i.e. list) 
    set map which maps a given constraint template name to a rewrite bundle. 
    This allows rewrite functions to be passed rewrite maps, which then 
    determine how each constraint is rewritten, and allow the user to tailor 
    the rewrite process to produce multiple, different, translations. </li>
   </ul>
   </li>
  </ul>


  \todo Example
  <ul>
   <li> As an example, consider that one has the EVEN-PARITY constraint over 4
   variables:
   \verbatim
declare(even_par_ns, posfun);
declare(even_par_ns, noun);
even_par_cstt : ["even_parity",['v1,'v2,'v3,'v4], even_par_ns];
   \endverbatim
   Then even_par_ns would be the namespace and even_par_cst would be the
   constraint template. Note that one could translate this constraint
   to two constraints each computing the parity of two variables and
   introducing new variables v1 and v2 to store the intermediate results. 
   </li>
  </ul>

*/
