// Matthew Gwynne, 26.3.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp
  \brief Plans for the translation of Rijndael into active clauses ("SAT constraints") etc in Maxima


  \bug Test failure with okltest_ncl_ss
  <ul>
   <li>
   \verbatim
okltest_ncl_ss(ncl_ss)
ASSERT: Expression " 96448 = 153048 " does not evaluate to true.
   \endverbatim
   </li>
  </ul>


  \todo Document and test output_fast_ss_fcl_std
  <ul>
   <li> output_fast_ss_fcl and output_fast_ss_fcl_std use
   Maxima arrays to cache (very large) variable names
   during the AES translation, and use integer variable
   names in the intermediate steps instead. </li>
   <li> This results in a considerable speed up, yet produces
   precisely the same CNF as the original translation. </li>
   <li> Note also that output_fast_ss_fcl (internally, using
   local) redefines rename_fcl to use a Maxima array for hashing
   as well. We should consider whether to use this new improved
   version of rename_fcl at the global level. </li>
   <li> We need exact measures of how much of a speed up
   this function and the functions it redefines gives. </li>
   <li> We need documentation on exactly what it does. </li>
   <li> We need applications tests that can check for certain
   AES instances that it continues to yield the same translations.
   </li>
  </ul>


  \todo Simplify AES translation
  <ul>
   <li> The current AES translation tries to provide a "general" translation
   framework, and then translate the AES using that. </li>
   <li> If we were translating many ciphers and didn't care about the details,
   this would be suited, but we do care about the details. </li>
   <li> The disadvantages of the current system are:
    <ul>
     <li> %Variables are hard to identify and have very long names (nested
     namespaces etc). </li>
     <li> Due to large variable names, the size of clause-sets in memory
     is MUCH larger than necessary; see "output_ss_fcl_std runs out of memory
     generating one round AES". </li>
     <li> Code is hard to read due to accomodating the "framework". </li>
     <li> Statistics functions are hard to write because many different
     translations are bundled together, leading to many case distinctions.
     </li>
     <li> The translation is hard to reason about due to the added
     complexities of a general translation framework. </li>
    </ul>
   </li>
   <li> The current system should be replaced with a system like with the
   DES, see Cryptanalysis/DataEncryptionStandard/ConstraintTranslation.mac.
   </li>
   <li> %Variables should always be specially and specifically declared; no
   namespaces or anything similar. </li>
   <li> If there are two different translations, then these should be written
   separately, possibly in separate files. </li>
   <li> There should be no "equality" constraints, we just use variables where
   they occur. </li>
   <li> The disadvantage of a more "custom" translation is that it requires
   far more thought and understanding. However, this is what we want! </li>
  </ul>


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
     <li> Passing F to standardise_fcl seems to balloon the memory use from
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
   <li> The 4 round AES translation has been running for 3 days and is still
   running. </li>
   <li> The only real sustainable solution is to rewrite the AES translation
   using short specially named and thought out variables, like in the DES;
   see "Simplify AES translation" and
   Cryptanalysis/DataEncryptionStandard/ConstraintTranslation.mac. </li>
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
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp .
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
   <li> This todo should be split across the plans files. </li>
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
     Investigations/Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp
     into the docus. </li>
     <li> Add example AES translation with statistics. </li>
    </ul>
   </li>
   <li> The following information on the different possible translations
   should also be included and linked to
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp:
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


  \bug DONE Bug not in the milestones
  <ul>
   <li> A bug must never be hidden in a testobject-file! </li>
   <li> Then likely no further actions are taken to remove the source of
   the failure. </li>
  </ul>


  \bug DONE (corrected statistics after updating minimum representations)
  Test failure
  <ul>
   <li>
   \verbatim
okltest_ncl_ss(ncl_ss)
ASSERT: Expression " 2652 = 6236 " does not evaluate to true.
ERROR: /home/csoliver/OKplatform/system_directories/aux/tests/maxima/OKlib/ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/testobjects/Translations
   \endverbatim
   </li>
  </ul>

*/

