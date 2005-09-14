// Oliver Kullmann, 13.9.2005 (Swansea)

/*!
  \file RandomGenerator_Plans.hpp
  \brief Plans for the module RandomGenerator.
  \todo The first thing is to transfer (and upgrade) the
  implementation of OKgenerator. This should proceed in the
  following stages:
   1. Creating the concept of BigIntegers in the module NumberTheory
   together with the wrapper for GMP.
   2. Replacing the current big-integer implementation in OKgenerator.
   3. Creating the concept for AES in the (new) module Cryptology.
   4. Writing a wrapper for some existing implementations for AES.
   5. Replacing the current AES implementation in OKgenerator.
   6. Then one needs to decide what about the input parameter handling
      (writing our own module, or using Boost).
*/

