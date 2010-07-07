// Matthew Gwynne, 27.2.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/milestones.hpp

  \module_version Lisp/Cryptanalysis/Rijndael 0.0.6 (4.6.2009)


  \par Version 0.0.7
  
  \par
  In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the following topics are handled:
    - Fix translation system
    - Parameterise inclusion of inverse operation in Mixcolumn translation 
    - Standardise block ordering: DONE
    - Truncate AES for reduced rounds: DONE


  \par
  In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp the following topics are handled:
    - Representations of the Sbox using additional variables
    - Move notion of AES constraints to separate module/unit
    - Organisation
    - How to represent elements of arbitrary fields as boolean variables?

  \par
  In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp the following topics are handled:  
    - Provide tests for all functions: DONE

  
  \par
  In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp the following topics are handled:
    - Randomly choosing "unknown" block bits: DONE

    
  \par Version 0.0.8

  \par
  In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp the following topics are handled:
    - Generate good CNF hitting clause-sets for the AES Sbox
    - Extracting prime implicate representations from the hitting-cls-representations
    - Compute *all* prime implicates (for the purpose of analysis)
  

  \par Version 0.0.9

  \par
  In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp the following topics are handled:
    - Compute *all* prime implicates (for the purpose of analysis)


  \par Version 0.1
  
  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the following topics are handled:
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
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp the following topics are handled:
    - Discussion on Generalisation/Parameterisation

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp the following topics are handled:
    - Find the symmetries of the AES Sbox DNF
    - Related 8-bit bijections

  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp the following topics are handled:
    - Find the symmetries of the AES Field Operations


  \par Version 0.1.2
  
  \par
   In ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp the following topics are handled:
    - Active clauses for field operations
    - Partitioning into active clauses
    - Algebraic aspects DONE (Moved to CryptoSystems/Rijndael)
  
  \par
   Update the following milestones.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history
   - 0.0.4 : 7.3.2008; initial version number (quite a few plans for translations to SAT).
   - 0.0.5 : 3.6.2009; added tests for all most Sbox analysis functions and tidied the implementation.
   - 0.0.6 : 4.6.2009; Added tests and finalised the initial translation system ready for rewriting using new notions.

*/
