// Matthew Gwynne, 27.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/milestones.hpp

  \module_version Lisp/Cryptanalysis/Rijndael 0.0.7 (19.3.2011)


  \par Version 0.0.8

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp the following topics are handled:
    - Update
    - Determine *all* prime implicates (for the purpose of analysis)

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp 
   the following topics are handled:
    - output_ss_fcl_std runs out of memory generating one round AES
    - Fix translation system

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp
   the following topics are handled:
    - Determine *all* prime implicates (for the purpose of analysis)


  \par Version 0.0.9

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   the following topics are handled:
    - Functions should not cache return values

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp 
   the following topics are handled:
    - Notion of "constraint"
    - Order of small scale matrix dimensions
    - Handling external data
    - Update specifications
    - Move AES box translations into separate file

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp
   the following topics are handled:
    - Add decomposed Sbox operations

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp
   the following topics are handled:
    - Add decomposed Sbox operations


  \par Version 0.0.10

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   the following topics are handled:
    - Generating defaults constant for small scale
    - Generating polynomial representations of field operations

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the
   following topics are handled:
    - Rewrite ncl_list_ss correctly
    - Remove AES-specific translation
    - Complete small scale helper functions
    - Remove hard-coding of multiplication by 01 in small scale MixColumn
    - Standardise output files names
    - Rearranging linear components of Sbox and MixColumns

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp
   the following topics are handled:
    - Move experimental todos to investigations
    - Add XOR translations for field multiplications
  

  \par Version 0.0.11

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the
   following topics are handled:
    - How to represent elements of arbitrary fields as boolean variables?

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   the following topics are handled:
    - Rewrite translation functions using ss_field_op_fulldnf_gen_fcl etc


  \par Version 0.1
  
  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the
   following topics are handled:
    - Rewrite tests based on rewritten translation system
    - Write Docus
    - Generate translation that allows multiple plaintext/ciphertext pairs
    - Constraint Rewrite System : DONE
    - Encoding AES (top down) : DONE

  \par
   The aim is to have a first translation, so that amongst others SAT-solvers
   can be utilised to attack AES.

  \par
   Update the following milestones.


  \par Version 0.1.1

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   the following topics are handled:
    - Discussion on Generalisation/Parameterisation

  \par
  In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp the
  following topics are handled:
    - Representations of the Sbox using additional variables

  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp
   the following topics are handled:
    - Find the symmetries of the AES Field Operations


  \par Version 0.1.2
  
  \par
   In Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the
   following topics are handled:
    - Provide additional translation into CSP-solver format 
    - Active clauses for field operations
    - Partitioning into active clauses
    - Algebraic aspects : DONE (Moved to CryptoSystems/Rijndael)
  
  \par
   Create milestones.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history
   - 0.0.4 : 7.3.2008; initial version number (quite a few plans for translations to SAT).
   - 0.0.5 : 3.6.2009; added tests for all most Sbox analysis functions and tidied the implementation.
   - 0.0.6 : 4.6.2009; added tests and finalised the initial translation system ready for rewriting using new notions.
   - 0.0.7 : 19.3.2011; improved the translation system by introducing meaningful variable-names and separate handling of clauses, and various extensions.

*/
