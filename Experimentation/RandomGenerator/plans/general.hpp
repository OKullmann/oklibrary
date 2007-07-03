// Oliver Kullmann, 13.9.2005 (Swansea)

/*!
  \file RandomGenerator/plans/general.hpp
  \brief Plans for the module RandomGenerator.


  \todo Update namespaces.


  \todo Schedule:
  The first thing is to transfer (and upgrade) the
  implementation of OKsolver/Generator. This should proceed in the
  following stages:
  <ol>
   <li> Creating the concept of BigIntegers in Concepts/plans/BigIntegers.hpp,
   together with the wrapper for GMP. </li>
   <li> Replacing the current big-integer implementation in OKgenerator
   (see Transitional/OKsolver/Generator). </li>
   <li> Creating the concept for AES in the (new) module Cryptology (see
   Kryptologie/plans/Kryptologie.hpp). </li>
   <li> Replacing the current AES implementation by the one provided by
   Cryptology (see Kryptologie/plans/Kryptologie.hpp). </li>
   <li> Replacing General/Kommandozeile (see General/Kommandozeile.hpp)
   by module Transitional/ProgramOptions. </li>
  </ol>

*/

/*!
  \namespace OKlib::RandomGenerator
  \brief Components for the generation of random problems
*/

namespace OKlib {
  namespace RandomGenerator {
  }
}

