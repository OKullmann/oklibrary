// Oliver Kullmann, 7.11.2006 (Swansea)

/*!
  \file Structures/Cryptology/plans/Kryptologie.hpp
  \brief Plans for the module on cryptological tools


  \todo Update: DONE
  First an overview on the content of the module, and what to do with it, is needed.
  <ul>
   <li> DONE (new todos created)
   Most important is to create a concept for
   AES-encoding/decoding, choosing some reasonable implementation
   (Kryptologie/rijndael.hpp and Kryptologie/rijndael.cpp seem to be fine,
   at least for the start), and writing a wrapper for it (of course,
   testing it). </li>
   <li> A new module "Cryptology" should be created, with all these tools
   (perhaps best in part "Structures"). DONE </li>
  </ul>


  \todo Concept for aes-usage
  <ul>
   <li> Create a concept for AES-usage (see client Experimentation/RandomGenerator/BlockChiffAES.hpp). </li>
   <li> Replace the C-header in rijndael.hpp by the corresponding C++ headers. </li>
   <li> Create doxygen-documentation for rijndael.hpp/.cpp. </li>
   <li> Implement the AES-concept by rijndael. </li>
   <li> Create a generic test function for AES (see the tests in
   Experimentation/RandomGenerator) </li>
   <li> Test the AES-wrapper of rijndael. </li>
  </ul>


  \todo MHash
  <ul>
   <li> Update the usage of the MHash-library. </li>
   <li> Are the newer version of the MHash-library? </li>
   <li> HashMD5.cpp shall become an application. </li>
   <li> What about "newHash" ? </li>
  </ul>


  \todo OKgenerator
  <ul>
   <li> Either transfer or remove the file with link-library-information for the
   OKgenerator. </li>
  </ul>

*/


