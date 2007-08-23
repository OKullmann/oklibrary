// Oliver Kullmann, 19.10.2005 (Swansea)

/*!
  \file BigIntegers/plans/GMPWrapper.hpp
  \brief Plans for the sub-module for wrapping the GMP-C-approach into a class according to the basic concept.


  \todo Design the concept of a wrapper for a big-int-class
  <ul>
   <li> The main first "client" is the OKgenerator (see
   RandomGenerator/plans/general.hpp); for the generator
   itself only VLiteralGeneratorAES.cpp and OKgenerator.cpp itself
   use big integers. </li>
   <li> See BigIntegers/concepts/plans/general.hpp. </li>
  </ul>


  \todo Write tests for the concepts


  \todo %Implementation of the basic wrappers:
  I guess, best based on the C-interface.
  <ul>
   <li> constructor uses mpz_init </li>
   <li> destructor uses mpz_clear </li>
  </ul>

*/

