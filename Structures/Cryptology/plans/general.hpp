// Oliver Kullmann, 7.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Cryptology/plans/general.hpp
  \brief Plans for the module on cryptological tools


  \todo Concept for aes-usage
  <ul>
   <li> Create a concept for AES-usage (see client
   Experimentation/RandomGenerator/BlockChiffAES.hpp). </li>
   <li> Replace the C-header in rijndael.hpp by the corresponding C++ headers.
   </li>
   <li> Create doxygen-documentation for rijndael.hpp/.cpp. </li>
   <li> Implement the AES-concept by rijndael. </li>
   <li> Create a generic test function for AES (see the tests in
   Experimentation/RandomGenerator) </li>
   <li> %Test the AES-wrapper of rijndael. </li>
   <li> We also should have application tests, which can be used for the
   computer-algebra programs as well (see
   ComputerAlgebra/Cryptology/plans/Rijndael.hpp). </li>
  </ul>


  \todo MHash
  <ul>
   <li> See "MHash segmentation fault" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Mhash.hpp. </li>
   <li> For now, the related applications have been disabled. </li>
   <li> Update the usage of the MHash-library. </li>
   <li> DONE (no releases anymore)
   Are the newer version of the MHash-library? </li>
   <li> HashMD5.cpp shall become an application. </li>
   <li> What about "newHash" ? </li>
  </ul>


  \todo Crypto++
  <ul>
   <li> See "Cryptology" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp. </li>
   <li> Looks reasonable; but US-specific? </li>
   <li> For AES we have an implementation, but perhaps for other uses. </li>
  </ul>


  \todo OKgenerator
  <ul>
   <li> Either transfer or remove the file with link-library-information for
   the OKgenerator. </li>
  </ul>

*/


