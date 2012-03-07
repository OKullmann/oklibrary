// Oliver Kullmann, 1.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/milestones.hpp

  \module_version Investigations/Cryptography/AdvancedEncryptionStandard 0.1.5.2 (3.11.2011)


  \par Version 0.1.6 :

  \par Cryptography/AdvancedEncryptionStandard/KeyDiscovery version 0.1.3
  (see AdvancedEncryptionStandard/plans/KeyDiscovery/milestones.hpp)

  \par Cryptography/AdvancedEncryptionStandard/Representations version 0.1.6
  (see AdvancedEncryptionStandard/plans/Representations/milestones.hpp)

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/general.hpp the
   following topics are handled:
    - Summary of previous experimental results
    - Merge SAT2011 plans with module one level higher : DONE
    - MiniSAT2 based solvers return incorrect times using experiment script : DONE
    - Add milestones : DONE
    - Replace "merge_cnf.sh" with "AppendDimacs" : DONE
    - Update scripts : DONE

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/Benchmarks.hpp the
   following topics are handled:
    - Prepare benchmarks for SAT 2011

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/EncryptionDecryption.hpp the
   following topics are handled:
    - Update instructions

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp
   the following topics are handled:
    - Solvers to be used for experimentation
    - Prepare experiments for the SAT 2012 paper : DONE


  \par Version 0.1.7 :

  \par Cryptography/AdvancedEncryptionStandard/KeyDiscovery version 0.2
  (see AdvancedEncryptionStandard/plans/KeyDiscovery/milestones.hpp)

  \par Cryptography/AdvancedEncryptionStandard/Representations version 0.1.7
  (see AdvancedEncryptionStandard/plans/Representations/milestones.hpp)

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/general.hpp the
   following topics are handled:
    - Separate key-schedule and block-cipher
    - Notions for AES operation
    - Investigating conditions and their representations
    - Structure
    - Explain how to replace various AES boxes with identity or random boxes
    - Determining a "good" local search algorithm to use on AES instances

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp
   the following topics are handled:
    - Move experiment data to investigation-reports
    - Update experiment script

  \par
   Create further milestones.

  \par
   Create an outline of the experiments.


  \par Version 0.1.8 :

  \par Cryptography/AdvancedEncryptionStandard/Representations version 0.1.8
  (see AdvancedEncryptionStandard/plans/Representations/milestones.hpp)

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/general.hpp the
   following topics are handled:
    - Using SBSAT
    - Merge SAT2011 plans with module one level higher

  \par
   In AdvancedEncryptionStandard/plans/EncryptionDecryption.hpp the
   following topics are handled:
    - Encryption
    - Decryption

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp
   the following topics are handled:
    - Investigating dimensions

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/AESKeyDiscovery.hpp
   the following topics are handled:
    - Breaking AES

  \par
   In AdvancedEncryptionStandard/plans/CryptographicProperties.hpp
   the following topics are handled:
    - Cryptographic properties of AES
    - Keys for which AES encrypts P to P
    - Create sub-module

  \par
   In Experimentation/Investigations/plans/CurrentExperiments.hpp the section
   on AES is updated.


  \par Version 0.1.9 :

  \par Cryptography/AdvancedEncryptionStandard/Representations version 0.1.9
  (see AdvancedEncryptionStandard/plans/Representations/milestones.hpp)

  \par
   In Cryptography/AdvancedEncryptionStandard/plans/general.hpp the
   following topics are handled:
    - Open problems


  \par Version 0.2 : PLANNING SAT2012 COMPLETED



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.1   : 1.1.2011; initial version number (various plans and first results regarding AES "box" representations).
   - 0.1.1 : 1.1.2011; started serious planning for the SAT2011 contributions.
   - 0.1.2 : 1.1.2011; removed/transferred completed todos, updates links.
   - 0.1.3 : 2.1.2011; tidied general todos and updated SAT2011 sub-module.
   - 0.1.4 : 5.1.2011; created "Representations" sub-module with associated information.
   - 0.1.5 : 8.1.2011; tidied plans, introduced method descriptions and started experiment plans files.

*/

