// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/AESKeyDiscovery.hpp
  \brief On investigations on finding AES keys given plaintext and ciphertext pairs.


  \todo Breaking AES
  <ul>
   <li> What do we mean by "Breaking AES"?
   <ul>
    <li> We take the standard notion of "breaking" a symmetric block cipher,
    namely that if given a set of plaintext/ciphertext pairs, encrypted using
    the same key, can we then deduce the key faster than a brute force search?
    </li>
    <li> The size of the set of plaintext/ciphertext pairs needed, and how
    much faster than brute force one can find the key are then matters of
    practicality. </li>
    <li> Given a single plaintext/ciphertext pair, there may be multiple keys
    for which the AES algorithm maps that plaintext block to that ciphertext
    block given that key. Therefore, for the purposes of "breaking" AES in
    earnest, one may require providing multiple AES translations, each
    considering different plaintext/ciphertext pairs, but using the same key
    variables (see "Generate translation that allows multiple
    plaintext/ciphertext pairs" in
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp).
    </li>
    <li> Therefore, experiments detailed within this plans file consider taking
    various AES SAT translations, setting all plaintext variables, all
    ciphertext variables (to the appropriate values given the plaintext and the
    AES algorithm) and then setting "n" key bits where "n" ranges from 1 to the
    total number of key bits (128 in most cases). </li>
    <li> To "break" AES, the idea is obviously to leave all key variables
    unset. </li>
  </ul>

*/
