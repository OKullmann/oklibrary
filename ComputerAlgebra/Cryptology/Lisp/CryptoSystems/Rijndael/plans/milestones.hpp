// Matthew Gwynne, 26.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/milestones.hpp

  \module_version Lisp/CryptoSystems/Rijndael 0.0.10.2 (31.1.2011)


  \par Version 0.0.11

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/AdvancedEncryptionStandard.hpp
   the following topics are handled:
    - Sbox vector calculation yields wrong size : DONE

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/SmallScaleAdvancedEncryptionStandard.hpp
   the following topics are handled:
    - Removing parts of the AES
    - Generating test vectors
    - Test failure with level "full" : DONE
    - Add decomposed Sbox boxes : DONE
    - Datatypes and naming conventions : DONE


  \par Version 0.0.12

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/SmallScaleAdvancedEncryptionStandard.hpp
   the following topics are handled:
    - Docus

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
   the following topics are handled:
    - natl vs nat_l

  \par Version 0.0.13

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
   the following topics are handled:
    - Improving tests
    - Docus and Demos

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/AdvancedEncryptionStandard.hpp
   the following topics are handled:
    - Requirements


  \par Version 0.0.14

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/AdvancedEncryptionStandard.hpp
   the following topics are handled:
    - Provide function for generating AES term
    - Standardise data types and documentation


  \par Version 0.1

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
   the following topics are handled:
    - Notions and notations
    - Requirements
    - Coding Standards

  \par
   By Version 0.1, the original basic implementation of AES should be
   completed and documented.

  \par
   Update the following milestones.


  \par Version 0.1.1

  \par
   In ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
   the following topics are handled:
    - Modularising the Rijndael-implementation
    - Generalisations

  \par
   By Version 0.2, a more general AES implementation (including field
   generalisations etc) should have been produced.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history
   - 0.0.6 : 7.3.2008; initial version number (basic implementation of AES completed with tests, and settling of module structure).
   - 0.0.7 : 15.3.2008; some bugs in tests fixed, as well as sbox lookup data added and integrated with AES implementation.
   - 0.0.8 : 16.6.2008; restructuring and redesign.#
   - 0.0.9 : 3.6.2009; tidied implementation by using good naming conventions and removing unnecessary functions.
   - 0.0.10 : 3.1.2011; tidied up and switched to polynomial+matrix representation, as well as adding the small scale implementation.

*/
