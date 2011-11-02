// Oliver Kullmann, 1.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp
  \brief Investigations into AES related to the SAT 2011 paper

  The aim of these experiments is to investigate translations of the AES and 
  small scale AES cryptosystems using the translation scheme given in 
  ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac.

  The key aspects of the investigation are the representations for the 
  byte-function components of the AES (see 
  ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac) and
  the translations of AES using these component translations into CNF.




*/
