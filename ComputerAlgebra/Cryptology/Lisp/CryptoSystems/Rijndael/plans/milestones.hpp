// Matthew Gwynne, 26.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/milestones.hpp

  \module_version Lisp/CryptoSystems/Rijndael 0.0.9 (3.6.2009)


  \par Version 0.0.10

 \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
   the following topics are handled:
    - Notions and notations
    - Update the following todos
    - Alter internal AES functions to use polynomials and matrices : DONE
    - Truncate AES for reduced rounds: DONE

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/AdvancedEncryptionStandard.hpp
   the following topics are handled:
    - Sbox polynomial implementation : DONE
    - Convert aes_key_expansion to return a list of matrices : DONE
    - Remove addition from AES round : DONE

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/SmallScaleAdvancedEncryptionStandard.hpp
   the following topics are handled:
    - Tests for implementations
    - Use matrix of GF(b) matrices for MixColumn
    - Generating test vectors
    - Datatypes and naming conventions
    - Small scale AES implementation : DONE


  \par Version 0.0.11

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
   the following topics are handled:
    - The Rijndael cipher as an iterated condition system
    - Design of round functions
    - Key schedule : DONE
    - Algebraic aspects DONE


  \par Version 0.1

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
   the following topics are handled:
    - Docus and Demos
    - Requirements
    - Coding Standards

  \par
   By Version 0.1, the original basic implementation of AES should be completed and documented.

  \par
   Update the following milestones.


  \par Version 0.1.1

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
   the following topics are handled:
    - Modularising the Rijndael-implementation
    - Generalisations

  \par
   By Version 0.2, a more general AES implementation (including field generalisations etc) should have been produced.

  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history
   - 0.0.6 : 7.3.2008; initial version number (basic implementation of AES completed with tests, and settling of module structure).
   - 0.0.7 : 15.3.2008; some bugs in tests fixed, as well as sbox lookup data added and integrated with AES implementation.
   - 0.0.8 : 16.6.2008; restructuring and redesign.#
   - 0.0.9 : 3.6.2009; tidied implementation by using good naming conventions and removing unnecessary functions.

*/
