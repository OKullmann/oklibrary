// Matthew Gwynne, 26.3.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp
  \brief Plans for the translation of Rijndael into active clauses ("SAT constraints") etc in Maxima


  \bug output_ss_fcl_std runs out of memory generating one round AES
  <ul>
   <li> Running the following:
   \verbatim
oklib_load_all()$
num_rounds : 1$
num_columns : 4$
num_rows : 4$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$
   \endverbatim
   results in the following error:
   \verbatim
Maxima encountered a Lisp error:

 Memory limit reached. Please jump to an outer pointer, quit program and enlarge the
memory limits before executing the program again.
   \endverbatim
   </li>
   <li> The Maxima memory limit is 2GB. </li>
   <li> Using "oklib_monitor : true$", to watch the progress of the function,
   shows us that Maxima runs out of memory when it runs standardise_fcl
   in output_ss_fcl_std. </li>
   <li> Before standardise_fcl is called, Maxima uses around 300MB. This
   is as seen from the "top" command. </li>
   <li> There are two issues here:
    <ol>
     <li> The translated AES clause-set F is far larger than it needs to be:
      <ul>
       <li> F has 31400 variables. </li>
       <li> F has 1510056 literal occurrences. </li>
       <li> If each literal was stored as a 32-bit integer, then F should
       take up 1510056 * 4 / (1024 * 1024) = ~5.8MB in memory. </li>
       <li> However, each literal in F is an unevaluated composition
       of nounified Maxima functions terms. The string representation
       of this function could be > 200 characters. </li>
       <li> How does Maxima store functions terms? </li>
      </ul>
     </li>
     <li> Passing F standardise_fcl seems to balloon the memory use from
     ~300MB to >2GB:
      <ul>
       <li> Redefining rename_fcl to:
       \verbatim
rename_fcl(FF,VL) := block(local(h),
  for V in map("[", FF[1], VL) do h[V[1]] : V[2],
  return([create_list(i,i,1,length(FF[1])),
   map(
     lambda([C], (
       map(lambda([L], if L > 0 then h[L] else -h[-L]), C))), FF[2])]))$
       \endverbatim
       before running output_ss_fcl_std results in Maxima using only
       ~500MB and running output_fcl_v. </li>
       <li> Does the use of Maxima's associative arrays reduce memory usage?
       Perhaps the conversion of hash keys to strings causes a blow-up in
       memory? </li>
       <li> Does the inlining of functions such as substitutetotal_cl
       and substitutetotal_c result in less copying and therefore less
       memory usage?
        <ul>
         <li> This shouldn't be the case as Maxima lists are passed
         by reference. </li>
         <li> However, various calls to map might create new lists. </li>
        </ul>
       </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> Setting the heap size to 4GB allows the computation to go through:
   \verbatim
set_heap_size_ecl(2**32);
   \endverbatim
   </li>
   <li> The full 10 round AES translations (<code>num_rounds : 1$</code>)
   runs out of memory when the heap memory limit is 4GB. </li>
   <li> DONE For now, the Maxima heap size, heap_size_ecl_okl in
   Buildsystem/Configuration/ExternalSources/maxima.mak should be increased to
   4GB. This would bring it in line with the amount of main memory required in
   Buildsystem/ReleaseProcess/README. </li>
  </ul>


  \todo Handling external data
  <ul>
   <li> We need to offer translations of the AES which use
   larger (thousands of clauses) representations of the Sbox,
   field multiplications etc. </li>
   <li> However, including these in the OKlibrary git repository drastically
   increases the size of the repository, and realistically, the repository
   is not the place for data. </li>
   <li> Therefore we should create a directory ExternalSources/data and
   have a subdirectory "AdvancedEncryptionStandard" in which we can store
   files like those currently stored in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/data/ and with much
   larger representations. </li>
   <li> We can then provide a function "ss_load_external_data()" which
   looks in this directory and loads all of the files found within. </li>
   <li> Questions:
    <ul>
     <li> Should we be storing Maxima files as the data or CNFs which are
     then somehow read in? </li>
     <li> How does this external data show up in the git repository?
     There should, at least, be md5sums for each of the files. </li>
     <li> What directory structure to use? Is
     "ExternalSources/data/AdvancedEncryptionStandard" sufficient or
     should we maintain a more fine grained structure mirroring the
     modules in the git repository?
     </li>
    </ul>
   </li>
  </ul>


  \todo Order of small scale matrix dimensions
  <ul>
   <li> Functions such as "ss_fcl" take first the number of columns and
   then the number of rows. </li>
   <li> Taking the arguments in this order is inconsistent with the standard
   order for specifying matrix dimensions (first the number of rows, then
   the number of columns). </li>
   <li> The order should be corrected and all references to the altered
   functions updated. </li>
  </ul>


  \todo Rewrite ncl_list_ss correctly
  <ul>
   <li> Due to the large number of cases, ncl_list_ss is currently written
   programmatically, using list functions to combine the results of various
   other statistics functions. </li>
   <li> Ideally this should be rewritten to enumerate all the cases and list
   all of the formulas for the statistics in full. </li>
  </ul>


  \todo Remove AES-specific translation
  <ul>
   <li> The small scale AES translation handles the full AES, and therefore
   there is no need for a separate AES translation now, and the duplication of
   code means we constantly have to update two different versions. </li>
   <li> Therefore, the full AES translation should be removed and the helper
   functions made to use the small scale translation. </li>
  </ul>


  \todo Complete small scale helper functions
  <ul>
   <li> We need generation and output functions for the small
   scale translation. </li>
   <li> Some exist (see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translation.mac),
   but these need to be checked and some thought given to whether they cover
   all experimental instances we wish to generate. </li>
   <li> For example, can we integrate "Rearranging linear components of Sbox
   and MixColumns". </li>
   <li> See also "Dimensions" in
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp .
   </li>
  </ul>


  \todo Standardise output files names
  <ul>
   <li> Currently the filenames output by functions such as
   "output_ssmult_fullcnf_stdname" and so on in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac.
   have not been well thought out. </li>
   <li> Therefore some thought needs to be taken and a simple scheme developed.
   </li>
  </ul>


  \todo How to represent elements of arbitrary fields as boolean variables?
  <ul>
   <li> In
   CryptoSystems/Lisp/Rijndael/SmallScaleAdvancedEncryptionStandard.mac
   an arbitrary field can be used for the small scale AES word field. </li>
   <li> However, a GF(3^2) field would have 9 elements, and there is no
   longer a simple match between polynomial coefficients of the element
   and variables in the translation. </li>
   <li> One solution is to translate the problem into a non-boolean
   problem, and then translate from there to a boolean problem using
   the standard translation. </li>
   <li> What is the right approach? </li>
   <li> For now, this is rather a moot point as we do not use this
   generalisation, but it seems wrong to have different levels of
   generality within the implementation and translation. </li>
  </ul>


  \todo Provide additional translation into CSP-solver format
  <ul>
   <li> A translation of the AES into a format usable by popular CSP solvers
   should be undertaken. This should allow comparison to the SAT translation.
   </li>
   <li> Is there a generic input format (like DIMACS) for CSP problems? </li>
   <li> Translating to something Minion can read seems reasonably simple, using
   constraints such as:
   <ul>
    <li> difference (for xor) </li>
    <li> table or negativetable (for sbox, GF multiplication etc) </li>
   </ul>
   one should be able to generate a simple AES CSP problem.
   </li>
   <li> The specification of the sbox and multiplication operations are then
   separate from their instances within the constraint set. This should allow
   for a variety of different translations (including using large prime
   implicate representations). </li>
  </ul>


  \todo Write docus
  <ul>
   <li> Documentation should be written explaining the way the translation
   system works, as well as pointing out the standard functions to use. </li>
   <li> Much of this information can be moved from "Fix translation system".
   </li>
   <li> Documentation should be split into the general constraint rewriting
   tools, and the AES specific documentation. </li>
   <li> For docus on the general constraint rewriting, see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/ConstraintTemplateRewriteSystem.hpp.
   </li>
   <li> For docus on the AES specific translation functions, see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/Translations.hpp.
   </li>
   <li> For the general constraint rewriting, the following needs to be done:
    <ul>
     <li> Extend the example with a translation to SAT. </li>
     <li> Explain the naming conventions. </li>
     <li> Explain the need for constraints to contain enough information
     to be unique. </li>
     <li> Discuss or link to discussions on the design criteria. </li>
    </ul>
   </li>
   <li> For the AES translation, the following needs to be done:
    <ul>
     <li> List key functions with explanations:
      <ul>
       <li> What are the convenience functions? </li>
       <li> What are the output functions? </li>
       <li> What are the constraints? </li>
       <li> What are the variables? </li>
       <li> What are the translation functions? </li>
      </ul>
     </li>
     <li> List naming conventions. </li>
     <li> Explain namespace creation conventions:
      <ul>
       <li> For new constraints new namespaces are constructed by using an
       unevaluated lambda expression to include the "parent" constraint
       as an argument. This means the namespace encodes the history of
       how the variable was created. </li>
       <li> However, for efficiency (space) reasons, the variables are
       removed from the constraint. Otherwise, nesting namespaces results
       in a rapid growth in the size of variable "tokens". </li>
       <li> Additional parameters need to be added to constraints, to ensure
       similar constraints created by the same rewrite rule are still uniquely
       identifiable after the removal of variables from the constraint. </li>
      </ul>
     </li>
     <li> What are the AES parameters? </li>
     <li> What are the key papers? </li>
     <li> Where is the AES implementation at the Maxima level? </li>
     <li> How to handle different parameter settings? </li>
     <li> What is the parameter space? Move discussion from
     "Investigating dimensions" in
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp
     into the docus. </li>
     <li> Add example AES translation with statistics. </li>
    </ul>
   </li>
   <li> The following information on the different possible translations
   should also be included and linked to
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/general.hpp:
   <ul>
    <li> We model a generalised AES system (see
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac)
    which supports the following different translations:
    <ul>
     <li> Mix columns variants with translations using:
     <ul>
      <li> the boxes in the encryption direction </li>
      <li> both encryption and decryption directions. </li>
     </ul>
     </li>
     <li> Box translations for the Sbox and field multiplications using:
     <ul>
      <li> Small CNF representations (see
      Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp
      and
      Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp).
      </li>
      <li> The canonical DNF translation (see
      ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Constructions.mac). </li>
      <li> r_k-reduced representation (see
      OKlib/ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac). </li>
      <li> Small hitting clause-set representations (see
      Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp).
      </li>
     </ul>
     </li>
     <li> Combining affine components
     <ul>
      <li> We have the standard AES which has the Sbox made up of the
      word (byte) level inversion of block elements, following by the
      application of an affine transformation. </li>
      <li> On the other hand, the affine portion of the Sbox can be moved
      through the Shiftrows operation such that it can be combined with the
      MixColumns component, yielding a component which is entirely linear at
      the byte level, leaving only the non-linear inversion and addition of
      the affine constant as part of the Sbox operation. </li>
      <li> Splitting the two types of operation like this will hopefully
      yield Sbox translations which are more compact and more amenable to
      SAT solvers as the affine transformation is specifically designed to
      increase equation size, and make the Sbox harder to model. </li>
     </ul>
     </li>
     <li> Generalised small scale parameters (see
     Cryptology/Lisp/CryptoSystems/Rijndael/SmallScaleAdvancedEncryptionStandard.mac)
     <ul>
      <li> n_R: number of rows in the AES block (default 4, can be
      1, 2 or 4). </li>
      <li> n_C: number of columns in the AES block (default 4, can be
      1, 2 or 4). </li>
      <li> e: size of word field in bits in the AES (default 8, can be
      1, 2 or 4 or 8) - in general we might consider arbitrary fields. </li>
      <li> r: number of rounds (default 10, can be any positive integer).
      </li>
     </ul>
     </li>
    </ul>
    </li>
   </ul>
   </li>
   <li> DONE This has been started but needs to be extended significantly. </li>
  </ul>


  \todo Generate translation that allows multiple plaintext/ciphertext pairs
  <ul>
   <li> Given that a single plaintext/ciphertext pair (P,C), encrypted with
   AES  using a key K, might not be enough, on it's own, to deduce K (as there
   may be some K' which performs the same mapping for this specific (P,C)), for
   genuine experiments and understanding of AES, translations allowing
   multiple plaintext/ciphertext pairs but sharing the key variables are
   needed. </li>
   <li> A simple method here is to perform the translation multiple times,
   introducing distinct variables for each pair of plaintext/ciphertext, but
   using the same key variables in each translation. The union of all such
   translations is then the required result. </li>
  </ul>


  \todo DONE (new system implemented; remaining issues moved to other todos)
  Fix translation system
  <ul>
   <li> DONE Each of the following todos must be completed on its own, with
   explanations how it was achieved. </li>
   <li> DONE (moved to 'Evaluating AES "constraints"')
   Special care must be devoted to the notions of "active clauses" and
   "constraint", where some progress must be achieved in more general modules,
   learning from the experiences here. </li>
   <li> DONE (moved to 'Evaluating AES "constraints"')
   The notion of "active clause" implies that for example AES encryption/
   decryption can be executed by just providing plain-text and key as a partial
   assignment and evaluating the whole active clause-set. Has this been
   achieved?? </li>
   <li> DONE (removed todos reference the old translation; issues
   have been addressed by this todo)
   The discussions here, and in the already removed todos (see ticket
   MG-22), need to be fully checked for being completely transferred to
   specifications and/or docus. </li>
   <li> DONE Further questions are:
    <ul>
     <li> DONE (each constraint is now fully specified by the parameters
     within it)
     What is the best way to control the rewrite process? </li>
     <li> DONE (each constraint is now fully specified by the parameters
     within it)
     The current system works but a more precise, systematic way of
     controlling how many rounds, or which rewrite rules are used etc is
     needed. </li>
     <li> DONE What is the best way to handle parameters controlling the translation?
     <ul>
      <li> There are already many parameters, many of which are controlled via
      global variables, which is potentially bad practice and underdocumented.
      </li>
      <li> However taking these parameters as explicit parameters (which must
      always be provided) is incredibly cumbersome. </li>
      <li> These parameters should become arguments to the constraint templates,
      as this is precisely what these additionals arguments are for, then such
      information (%e.g. that the 2 round AES variant should be considered) is
      not hidden away. </li>
      <li> These can be considered simply by their position in the argument list,
      and if an argument is optional, then you must still provide it if one of
      the arguments appearing later in the list must be specified. </li>
      <li> For this reason, for such parameters, defaults should be provided,
      however, they should not be changed in the global scope. </li>
     </ul>
     </li>
    </ul>
   </li>
   <li> DONE (covered by docus and other todos) Specification:
    <ul>
     <li> Concepts:
      <ul>
       <li> DONE (covered in docus; see
       ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/ConstraintTemplateRewriteSystem.hpp)
       Constraint - A list with at least 2 arguments, the first being
       the name of the constraint, the second a list of variables in a
       predefined order, and then any additional arguments which may
       be required for the computation of the constraints. For example:
       \verbatim
       ["aes_ct",[aes_ns(p,1),...,aes_ns(p,128),...,aes_ns(c,128)],rounds,namespace]
       \endverbatim
       Within the constraint rewrite system, every such constraint should
       have a namespace as it's last argument.
       See
       Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
       for an example of similar such constraints.
       </li>
       <li> DONE (moved to "Write docus")
       Namespace - An unevaluated function, where
        <ol>
         <li> The first (required) argument is a variable. </li>
	 <li> The remaining arguments are specific to the constraint
	 and are used as parameters. For instance, an AES constraint will
	 include the number of rounds as an additional parameter. </li>
	 <li> The last argument to a namespace is then simply
	 a constraint, providing context for the namespace. In all currrently
	 considered cases, this constraint will be a constraint being
	 rewritten by the constraint rewrite rule associated with the given
	 namespace. </li>
	 <li> The result of the function (if it were to be evaluated) is
	 assumed to be a variable. </li>
	 <li> As the return type of the namespace
  	 function is assumed to be a variable, we may nest namespaces
	 within each other. </li>
	 <li> As the namespace is an unevaluated function, it may be
	 an unevaluated lambda expression, and so arguments may be
	 included in a namespace in this way, that is:
	 \verbatim
lambda([a],some_namespace_x(a,1,2,3))
	 \endverbatim
	 </li>
	 <li> The problem with having the whole constraint as the final
	 argument for the namespace here, is that as more and more rewrites
	 occur, there is a significant overhead, as each variable is then a
	 rather large object. This grows exponentially, as at each level,
	 the namespace used for the level above occurs for every variable
	 instance. </li>
	 <li> Such an overhead is quite significant, taking more than 30
	 seconds to generate just 8 variables 3 levels down. </li>
	 <li> This makes generating the AES translation for any number of
	 rounds rather difficult. </li>
	 <li> One solution would be to have every rewrite function take
	 the namespace to use, rather than using a namespace specific
	 to that rewrite function. Then the rewrite_all_csttl function
	 would use the namespace associated given in the constraint
	 rewrite bundle. </li>
	 <li> If rewrite_all_csttl used the namespace in the constraint
	 rewrite bundle, then for efficient generation of the AES
	 translation, one could simply use a different constraint
	 rewrite bundle which would specify namespaces which drop, hash, or
	 canonically map the constraint variables to a much smaller
	 object. </li>
	 <li> Another solution is to simply include additional wrapper
	 functions for rewrite_all_csttl, rewrite_all_cstt_vars_l etc, which
	 avoid this explosion by keeping track of constraints individually
	 and only introducing an identifier when a new constraint appears.
	 </li>
	 <li> For example:
	 \verbatim
rewrite_all_csttl_fast(cstl,rewrite_map) := block(
  [cst_hash : sm2hm({}), cst_count : 0],
  local(construct_constraint_namespace),
  construct_constraint_namespace(namespace_p,namespace,cst) := block(
    if ev_hm(cst_hash, [namespace_p,namespace,cst]) = false then
      set_hm(cst_hash, [namespace_p,namespace,cst], cst_count : cst_count + 1),
    lambda([a], namespace_p(a,cst[1],ev_hm(cst_hash, [namespace_p,namespace,cst])))),
  rewrite_all_csttl(cstl,rewrite_map))$
	 \endverbatim
	 </li>
	 <li> Such a system would only affect the actual variable
	 representations, but not the order of the variables, or anything
	 else about the translation, and so there would be an exact
	 one to one mapping between the variables returned by
         rewrite_all_cstt_vars_l and rewrite_all_vars_l_fast. Therefore, one
         could use the fast translation, and simply map back to the full
         variable representations if needed. </li>
	 <li> The namespace here should not include the entire constraint,
	 but just additional information which uniquely identifies the
	 namespace, specific to an individual constraint. </li>
        </ol>
       </li>
       <li> DONE (see
       ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/ConstraintTemplateRewriteSystem.hpp
       and "Write docus")
       Variable - A positive noun, defined in the usual way
       (see ComputerAlgebra/Satisfiability/Lisp/Generators/Generators.mac) for
       each constraint template, where the arguments are used to indicate
       which instance of a particular type of variable are being used (that
       is, using nouns in the same way as is usual, see
       ComputerAlgebra/Satisfiability/Lisp/Generators/Generators.mac). </li>
      </ul>
     </li>
     <li> DONE (see rewrite_all_csttl; overview in 
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/ConstraintTemplateRewriteSystem.hpp;
     also see "Write docus")
     Overview of system:
     <ul>
      <li> DONE (see rewrite_all_csttl) 
      To translate AES one would call a rewrite all constraint
      function called "rewrite_all_csttl", which would take as an argument a
      list containing only a constraint called "aes_cst", where
      arguments for the constraint are the variables of AES
      (plaintext, key and ciphertext), along with a list of arguments, which
      would include the identity as the namespace (i.e., "lambda([a],a)"), and
      then additionally arguments specifying which translation is used for the
      Sbox, field multiplications, and whether one should include the
      mixcolumn inverse operation etc. </li>
      <li> DONE (see rewrite_all_csttl)
      So for example:
      \verbatim
rewrite_all_csttl([["aes_cst",[p1,...,p128,k1,...,k128,c1,...,c128],lambda([a],a)]]);
      \endverbatim
      </li>
      <li> DONE (see rewrite_all_csttl)
      "rewrite_all_csttl" would then call constraint rewrite
      function, called for instance, "aes_cstr_cstl", which would take as an
      argument, the constraint, along with a rewrite mapping. </li>
      <li> DONE (see rewrite_all_csttl)
      aes_cstr_cstl would then translate this into a list of constraints,
      such as "aes_subbytes_cst", for which all newly introduced
      variables have the namespace "aes_ns", where additional arguments to
      the namespace are additional (non-namespace) arguments to "aes_cst",
      and the namespace argument of (for instance) "aes_subbytes_cst" is
      "lambda([a],aes_ns(a,arg1,arg2,...))", and additional arguments
      to sub-constraint-templates are simply those arguments relevant
      to that sub-constraint. </li>
      <li> DONE (see rewrite_all_csttl)
      Such a rewrite procedure should continue until on newly
      produced constraints, and existing constraints
      until all constraint rewrite functions have been applied
      in the order specified by the given rewrite mapping. </li>
      <li> DONE (see rewrite_all_csttl)
      The result of "rewrite_all_csttl" is then a set of constraints
      which can no longer be rewritten into smaller
      constraints. </li>
      <li> DONE (see
      ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateTranslation.mac)
      At this point one can then call a "translate to CNF" function
      on this set of constraints to rewrite it to CNF. </li>
      <li> DONE (see
      ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateGlobalPropagation.mac)
      Within the translate to CNF function, constraints
      such as those representing equivalence of variables can be translated
      by replacement of variables etc, rather than adding additional clauses
      etc. </li>
      <li> DONE (moved to 'Notion of "constraint"')
      MG needs to alter the currently implemented parts of the new system
      to take account of the altered specification (change from notions of
      constraint templates to simply constraints). </li>
      <li> (DONE) MG should see
      Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
      and replace the functional notation used in the new system with
      lists and clarify the distinction between constraint templates
      and constraints. </li>
     </ul>
     </li>
     <li> DONE (moved to
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/ConstraintTemplateRewriteSystem.hpp)
     Datatypes (in an abstract sense):
      <ul>
       <li> Constraint - denoted by postfix "cst". </li>
       <li> Constraint rewrite function - denoted by "cstr" in
       function name. </li>
       <li> Constraint list - denoted by "cstl". </li>
      </ul>
     </li>
     <li> DONE (functions now exist; todo in "Write docus" to
     list and document (some of) them)
     Necessary functions and structures:
      <ul>
       <li> Constraint rewrite bundle:
        <ul>
	 <li> Three element list containing:
	  <ol>
           <li> A constraint rewrite function. </li>
	   <li> A namespace used for any new variables introduced in the
	   constraint template rule. </li>
	   <li> A function which computes a list of variables
	   introduced by the constraint template rewrite
	   function. </li>
	  </ol>
	 </li>
	 <li> Such a structure groups together all elements relating to
	 the particular rewrite of a given constraint template. </li>
	</ul>
       </li>
       <li> Constraint rewrite function:
        <ul>
	 <li> Take as argument, a constraint. </li>
	 <li> Returns a list of constraint templates representing the original
	 constraint. </li>
	 <li> If the given constraint is of the type which the function
	 rewrites then the returned list will be a new list of constraints,
	 otherwise the function just returns the singleton list with
	 the input constraint as the only element. </li>
	 <li> Each of the constraints in the returned list should
	 have a namespace n as their final
	 argument, where n is the functional composition of
	 the namespace for this rewrite function, and the namespace given
	 as the "parent namespace" for the constraint being
	 rewritten. </li>
	 <li> If a constraint is given which is the correct constraint
	 but lacking a namespace argument, then the identity function is
	 assumed as the namespace. </li>
	 <li> For example, given the constraint:
	 \verbatim
["example_ct",[ct_arg_1,ct_arg_2,ct_arg_3],ex_param,ex_namespace_parent]
	 \endverbatim
	 a constraint rewrite function "example_cstr_cstl" with this as an argument
	 would return:
	 \verbatim
[["example_sub_ct",[ct_arg_1,ct_arg_2,example_namespace(ex_namespace_parent(ct_arg_new,ex_param))],example_namespace],
["example_sub_ct2",[ct_arg_3],example_namespace]]
	 \endverbatim
	 </li>
	</ul>
       </li>
       <li> Namespace variables function:
        <ul>
	 <li> Takes as first argument a constraint. </li>
	 <li> Takes as the second argument a rewrite mapping. </li>
	 <li> Return a list of variables introduced by the associated
	 constraint rewrite function, when given the above constraint. </li>
	 <li> Note here, that through such functions, one has immediately
	 a method of translating variables to integer values, as such
	 values are simply the index of the variable in the list. </li>
	 <li> The ordering introduced here should be a reasonable
	 well-defined ordering, not simply random, as it comes. </li>
	</ul>
       </li>
       <li> Rewrite mapping:
        <ul>
	 <li> A list of mappings, where a mapping is simply a two element
	 list, with the first element being the name of a constraint,
	 for example, "aes_ct", and the second argument being a constraint
	 template rewrite bundle, with which to rewrite this template. </li>
	 <li> That is, a rewrite mapping is simply an ordered set map, from
	 constraint names to a constraint rewrite bundles to rewrite with.
	 </li>
	 <li> Such rewrite mappings then determine which rewrite rules are to
	 be applied by global rewrite functions (see "Rewrite all constraint
	 templates function"), and also the order in which these
	 rewrites will be done. </li>
	 <li> Several default rewrite mappings can then be provided in the
	 library for convenience. </li>
	 </li>
	</ul>
       </li>
       <li> Rewrite all constraints function:
        <ul>
         <li> Takes as first argument a set of constraints. </li>
	 <li> Takes as second argument a rewrite mapping. </li>
	 <li> Returns a list of constraint templates after applying all
	 rewrite rules. </li>
        </ul>
       </li>
       <li> Translate to CNF function:
        <ul>
	 <li> Takes as argument a list of constraint templates. </li>
	 <li> Returns a list containing:
	  <ol>
	   <li> A list of variables present in the rewritten list. </li>
	   <li> A list of clauses and constraint templates after
	   applying all translations from constraint templates to CNF.
	   </li>
	  </ol>
	 </li>
	 <li> Call this result list a formal pseudo-constraint list.
	 </li>
	 <li> The point here is that if there are only constraints
	 in the input, which are rewritable to CNFs, then the result is simply
	 a formal clause-list. </li>
	 <li> If it is passed constraints which do not have
	 a translation to CNF then it will simply return them as they are,
	 within the list of clauses/templates. </li>
	 <li> Each specific rewrite situation, such as for instance handling
	 of equality constraints should be represented by separate functions
	 which rewrite the entire pseudo-constraint set, and are
	 then called by this function. </li>
	</ul>
       </li>
       <li> Constraint list to CNF clause list mapping function:
        <ul>
	 <li> This function should take as an argument a list of constraint
	 templates. </li>
	 <li> The result should be a formal CNF clause-list representing the
         constraint system. </li>
	 <li> Auxilliary functions which translate individual constraint
	 templates into clause-lists are also necessary (and exist for the
	 most part). </li>
	 <li> Additionally, constraints such as equality constraints should
	 be rewritten, to simple replace literals with their equivalent
	 literals, although there are different options which
	 should be represented by different rewrite functions. </li>
	</ul>
       </li>
       <li> (DONE Replaced with variable generator)
       Variable count function:
        <ul>
	 <li> Takes as argument namespace arguments. </li>
	 <li> Returns the number of variables used in this namespace. </li>
	 <li> Such a function should be defined for each namespace
	 and can be recursively calculated based on sub-namespaces. </li>
	</ul>
       </li>
       <li> (DONE Replaced with variable generator)
       Variable to integer mapping function:
        <ul>
	 <li> Take as an argument a variable. </li>
	 <li> Return the integer representing that variable. </li>
	 <li> Note that the argument may have unevaluated namespaces
	 surrounding it, and these can then be used by the mapping
	 function to determine the correct mapping. </li>
	 <li> Such a mapping function should exist for each namespace,
	 where the mapping from the set of all variables V(N) in a namespace
	 N to the set of integers {1,...,size(V(N))} is bijective. </li>
	 <li> Mapping functions can then be defined recursively using
	 the mapping functions for sub-namespaces and variable count
	 functions. </li>
	</ul>
       </li>
       <li> (DONE Replaced with variable generator)
       Integer to variable mapping function - simply the inverse of the
       "Variable to integer mapping function". </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> This todo should be split up, as it is getting rather large. </li>
   <li> DONE (old system replaced by constraint template system)
   This translation works and has the following advantages:
    <ul>
     <li> It is a simple rewrite procedure. </li>
     <li> Rewrite rules are easy to replace. </li>
    </ul>
    however, it also has several disadvantages:
    <ul>
     <li> DONE (translation process split into stages;
     see ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/docus/ConstraintTemplateRewriteSystem.hpp)
     %Clauses are injected directly into the set of constraint templates,
     requiring explicit detection of "sets" within the rewrite system, as they
     are not constraint templates to be rewritten and therefore must be treated
     differently. </li>
     <li> DONE (constraints now have additional parameters;
     see ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateSmallScaleRewriteRules.mac)
     As there is no context for various constraints, only variable
     arguments, rewrite rules such as "aes_round" etc can't use this additional
     information, and:
      <ul>
       <li> Must resort to using "aes_make_vars" which resorts to
       use of global variables in the process. </li>
       <li> Trying to instantiate the system of constraint templates into a
       system of true constraints may result in some information lost which
       could have been used. </li>
      </ul>
     </li>
     <li> DONE (added global propagation/renaming of variables)
     Operations such as "shiftrows" must be represented using equality
     constraints, which is a waste. </li>
    </ul>
    The main systematic disadvantage of using some "random" variables
    is that we are losing the meaning of variables.
   </li>
   <li> DONE (old system replaced by constraint template system)
   To improve the system, the following changes are suggested:
    <ul>
     <li> Split the overall clause set generation process into two steps:
     <ol>
      <li> A pure constraint template rewrite system. </li>
      <li> Translation of the constraint template system into a clause set.
      </li>
     </ol>
     In this way:
     <ul>
      <li> There is no need for explicit detection of "sets" within the
      rewrite system and everything is much cleaner. </li>
      <li> The second step can be used to remove equality constraints using
      variable replacement. </li>
      <li> The second step can be replaced with other translations into
      constraint languages, or replacement of constraint templates with "true
      constraints". See
      Satisfiability/Lisp/ConstraintProblems/plans/Conditions.hpp. </li>
     </ul>
     </li>
     <li> Alter the constraint template format to the form
     "aes_cp([p_1,...,p_n],[namespace,...])":
     <ul>
      <li> The first argument to the template is a list of variables. </li>
      <li> The second is a list of additional information about the
      constraint, such as a namespace for auxillary variables, although other
      information
      could be added based on the type of constraint template. </li>
      <li> This allows one to provide all information associated with the
      constraint template. </li>
      <li> Also, given a namespace for auxillary variables, variables may be
      generated in the rewrite rules without the need for any global function
      like "aes_make_vars". </li>
      <li> The form of the namespace seems simplest to be a positive function,
      as then namespaces can be composed, and when auxillary variables are
      needed within a rewrite rule, the namespace can simply be "applied" to
      a localised variable name. </li>
     </ul>
     </li>
    </ul>
   </li>
   <li> DONE (Old system has been fully replaced by the new;
   see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateRewriteSystem.mac)
   The current translation system works in the following way:
   <ul>
    <li> The common datatype is a set of "constraint templates" of the form
    "aes_c(p_1,p_2,p_3,...p_128,k_1,...)" where:
    <ul>
     <li> "aes_cp" is an unevaluated function. </li>
     <li> "p_1" etc represent variables within the constraint system. </li>
     <li> %Variables are associated with the underlying variables inherent in
     the constraint by their position in the function arguments. </li>
    </ul>
    </li>
    <li> The process starts off with the set with just the "aes_c" constraint
    template with the plaintext, key and ciphertext input variables as
    arguments. </li>
    <li> For each constraint template, there are rewrite rules, for instance
    "aes_cp" which take as arguments the variables given to the constraint
    template and produce a set of new constraint templates to replace it. </li>
    <li> When a rewrite rule needs to introduce auxillary variables, to ensure
    no clashing of variables occurs over multiple uses of the rule, a function
    "aes_make_vars" is used to generate unique variables (either named or
    integer depending on how aes_make_vars is assigned). </li>
    <li> Such rewrite rules are applied across the set of constraint templates
    using "rewrite_condition". This is done by simply giving the rewrite rule
    the arguments for the template, and then replacing it in the set of
    constraint templates with the new set of templates returned by the rule.
    </li>
    <li> Some rewrite rules, such as "aes_sbox_cp" produce sets of clauses,
    instead of sets of constraint templates. </li>
    <li> Rewrite rules are applied in a set order using "rewrite_all" to
    produce a final clause set. </li>
   </ul>
   </li>
  </ul>


  \todo DONE Move notion of AES constraints to separate module/unit
  <ul>
   <li> The representation of components of the AES as constraints
   and the evaluation of these constraints is separate from the
   rewriting of these constraints (constraints, plus additional information).
   </li>
   <li> The AES implementation at
   Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac
   should be updated to take the same arguments as each of the constraints
   used within this system (no global variables like aes_num_rounds), so one
   can evaluate the constraints generated by the rewrite functions. </li>
   <li> Doing this also allows various todos to be moved into a separate
   todo file. </li>
  </ul>

*/

