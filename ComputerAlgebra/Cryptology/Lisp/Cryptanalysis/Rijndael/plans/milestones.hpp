// Matthew Gwynne, 27.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/milestones.hpp

  \module_version Lisp/Cryptanalysis/Rijndael 0.0.8 (28.7.2011)


  \par Version 0.0.9

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   the following topics are handled:
    - Simplify AES translation
    - Rewrite translation functions using ss_field_op_fulldnf_gen_fcl etc
    - Functions should not cache return values

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp
   the following topics are handled:
    - Test failure with okltest_ncl_ss
    - Tidy AES translation
    - output_ss_fcl_std runs out of memory generating one round AES
    - Order of small scale matrix dimensions
    - Bug not in the milestones : DONE
    - Test failure : DONE

  \par
   In Cryptanalysis/Rijndael/plans/ConstraintTemplateSmallScaleRewriteRules.hpp
   the following topics are handled:
    - Move AES box translations into separate file
    - Update specifications
    - Test failure with okltest_ss_sbox_pi_cst_cl : DONE

  \par
   In Cryptanalysis/Rijndael/plans/ConstraintTemplateRewriteRules.hpp
   the following topics are handled:
    - Test failure with okltest_aes_mul3_pi_cst_cl
    - Test failure with okltest_aes_sbox_pi_cst_cl : DONE


  \par Version 0.0.10

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the
   following topics are handled:
    - Remove AES-specific translation
    - Complete small scale helper functions
    - Standardise output files names

  \par
   In Cryptanalysis/Rijndael/plans/ConstraintTemplateRewriteSystem.hpp
   the following topics are handled:
    - Notion of "constraint"

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/ConstraintTemplateSmallScaleRewriteRules.hpp
   the following topics are handled:
    - Remove hard-coding of multiplication by 01 in small scale MixColumn

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp
   the following topics are handled:
    - Move experimental todos to investigations



  \par Version 0.0.11

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the
   following topics are handled:
    - Handling external data
    - Rewrite ncl_list_ss correctly

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   the following topics are handled:
    - S-box and multiplication boolean 6xm functions
    - Evaluating AES "constraints"

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp
   the following topics are handled:
    - Add XOR translations for field multiplications

  \par Version 0.1

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the
   following topics are handled:
    - Rewrite tests based on rewritten translation system
    - Write Docus

  \par
   In Cryptanalysis/Rijndael/plans/ConstraintTemplateSmallScaleRewriteRules.hpp
   the following topics are handled:
    - Rearranging linear components of Sbox and MixColumns

  \par
   The aim is to have a first translation, so that amongst others SAT-solvers
   can be utilised to attack AES.

  \par
   Update the following milestones.


  \par Version 0.1.1

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   the following topics are handled:
    - Generating defaults constant for small scale
    - Generating polynomial representations of field operations

  \par
  In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp the
  following topics are handled:
    - Other software systems offering Sbox translations
    - Representations of the Sbox using additional variables
    - Add decomposed Sbox operations

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp
   the following topics are handled:
    - Find the symmetries of the AES Field Operations
    - Determine *all* prime implicates (for the purpose of analysis)


  \par Version 0.1.2

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp
   the following topics are handled:
    - Generate good CNF hitting clause-sets for the AES Field Operations
    - Prime implicate representations from hitting-cls-representations

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the
   following topics are handled:
    - How to represent elements of arbitrary fields as boolean variables?
    - Provide additional translation into CSP-solver format
    - Generate translation that allows multiple plaintext/ciphertext pairs

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/ActiveClauses.hpp the
   following topics are handled:
    - Active clauses for field operations
    - Partitioning into active clauses

  \par
   Create milestones.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history
   - 0.0.4 : 7.3.2008; initial version number (quite a few plans for translations to SAT).
   - 0.0.5 : 3.6.2009; added tests for all most Sbox analysis functions and tidied the implementation.
   - 0.0.6 : 4.6.2009; added tests and finalised the initial translation system ready for rewriting using new notions.
   - 0.0.7 : 19.3.2011; improved the translation system by introducing meaningful variable-names and separate handling of clauses, and various extensions.
   - 0.0.8 : 28.7.2011; started adding documentation for AES translation, addressed old and stale todos and investigated prime implicates for AES boxes.

*/
