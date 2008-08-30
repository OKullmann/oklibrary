// Oliver Kullmann, 30.8.2007 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Ecl.hpp
  \brief Documentation on how to build Ecl


  <h1> Installing Ecl </h1>


  <h2> On the purpose of Ecl </h2>

  The computer algebra system Maxima is implemented in "Common Lisp", and
  Ecl is an implementation of this form of Lisp.

  Yet we do not use Ecl directly, only indirectly through Maxima.


  <h2> What the installation yields </h2>

  <ul>
   <li> The ecl-executable, used for compiling lisp-files as well as an
   interactive interpreter. </li>
   <li> Documentation on Common Lisp in general, and on the usage of
   the ecl-executable. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version: $(ecl_recommended_version_number_okl) </li>
   <li> Checking the installation:
    <ol>
     <li> ecl-program call = <code>$(ecl_call_okl)</code> </li>
     <li> ready: $(ecl_call_ready_okl)
      <ul>
       <li> location = $(location_ecl_call_okl) </li>
       <li> version = <code>$(version_ecl_call_okl)</code> </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> Documentation: $(ecl_html_documentation_index_location_tag_okl) </li>
   <li> Helper program "rlwrap" for the frontendL
    <ol>
     <li> Recommended version: $(rlwrap_recommended_version_number_okl) </li>
     <li> rlwrap-program call = <code>$(rlwrap_call_okl)</code> </li>
     <li> ready: $(rlwrap_call_ready_okl)
      <ul>
       <li> location = $(location_rlwrap_call_okl) </li>
       <li> version = <code>$(version_rlwrap_call_okl)</code> </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>
  As usual, if a different version of Ecl is installed than "officially"
  recommended, then by overwriting "ecl_recommended_version_number_okl"
  (for example in <code>.oklib/override.mak</code>) this can be
  corrected.


  <h2> How to install </h2>


  <h3> Prerequisites </h3>

  <ul>
   <li> Since Ecl does not use "readline", the front-end does not
   handle cursor-keys; we automatically install "rlwrap" to provide these
   facilities, which is (automatically) used by Maxima. </li>
   <li> A symbolic link to "rlwrap" is placed in the public bin-directory
   <code>$(public_bin_dir_okl)</code>. </li>
  </ul>


  <h3> Make targets </h3>

  Only local installation currently.
  <table>
   <tr>
    <td> <code> ecl </code> </td>
    <td> Build the recommended versions of ecl and rlwrap. </td>
   </tr>
   <tr>
    <td> <code> cleanecl </code> </td>
    <td> Removes the ecl build directory. </td>
   </tr>
   <tr>
    <td> <code> cleanallecl </code> </td>
    <td> Removes all ecl build/installation/documentation directories. </td>
   </tr>
   <tr>
    <td> <code> rlwrap </code> </td>
    <td> Build the recommended version of rlwrap. </td>
   </tr>
   <tr>
    <td> <code> cleanallrlwrap </code> </td>
    <td> Removes all rlwrap build/installation/documentation directories. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version of Ecl, set variable
   <code>ecl_recommended_version_number_okl</code> accordingly. </li>
   <li> To install a different version of Rlwrap, set variable
   <code>rlwrap_recommended_version_number_okl</code> accordingly. </li>
 </ul>


*/

