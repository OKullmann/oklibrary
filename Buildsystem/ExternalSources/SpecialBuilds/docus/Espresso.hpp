// Oliver Kullmann, 3.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Espresso.hpp
  \brief Documentation on how to build and use Espresso


  <h1> Installing Espresso </h1>


  <h2> On the purpose of Espresso </h2>

  Espresso is a program for the computation of small CNF/DNF representations
  of boolean functions.


  <h2> Package history </h2>

  <ul>
   <li> The original Espresso home page is
   http://embedded.eecs.berkeley.edu/pubs/downloads/espresso/index.htm . </li>
   <li> We use a slightly updated version from
   http://code.google.com/p/eqntott/downloads/detail?name=espresso-ab-1.0.tar.gz .
   </li>
  </ul>


  <h2> What the installation yields </h2>

  <ul>
   <li> The executable <code>$(espresso_public_call_okl)</code>. </li>
   <li> Some documentation in directory <code>$(espresso_doc_dir_okl)</code>.
   </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(espresso_recommended_version_number_okl)</code> </li>
   <li> Path to the executable:
    <ol>
     <li> espresso program call = <code>$(espresso_call_okl)</code> </li>
     <li> A symbolic link <code>$(espresso_public_call_okl)</code> is provided
     in the public bin-directory (<code>$(public_bin_dir_okl)</code>). </li>
    </ol>
   </li>
   <li> Checking the installation:
    <ul>
     <li> Ready: $(espresso_call_ready_okl) </li>
     <li> location = $(location_espresso_call_okl) </li>
     <li> version = <code>$(version_espresso_call_okl)</code>
     </li>
    </ul>
   </li>
  </ul>


  <h2> How to install </h2>

  <h3> Make targets </h3>

  <table>
   <tr>
    <td> <code> espresso </code> </td>
    <td> Build the recommended version of espresso, using the local gcc. </td>
   </tr>
   <tr>
    <td> <code> cleanespresso </code> </td>
    <td> Remove the build directory. </td>
   </tr>
   <tr>
    <td> <code> cleanallespresso </code> </td>
    <td> Remove build, installation and documentation directory. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version, set variable
   <code>espresso_recommended_version_number_okl</code> accordingly. </li>
  </ul>


  <h2> Usage </h2>

  <ul>
   <li> Use <code>$(espresso_exec_okl) filename</code>, where the file contains
   a boolean function in the (extended) PLA-format. </li>
   <li> An example is to use the following "test.pla" file (see below)
   with espresso
   \verbatim
.i 3
.o 1
110 1
111 1
010 1
001 0
000 0
100 1
011 0
101 0
   \endverbatim
   by calling
   \verbatim
> espresso2.3 test.pla
.i 3
.o 1
.p 3
1-0 1
-10 1
11- 1
.e
  \endverbatim
   </li>
   <li> The PLA file is a format for describing boolean functions. </li>
   <li> For simple usage one can assume that the ".i" line specifies the
   number of input variables, the ".o" line the number of output variables,
   and then each line specifies the entry in a truth table describing the
   associated boolean function. </li>
   <li> If an entry is marked with a "-", this is simply shorthand for the
   expansion into multiple truth table entries where the "-" values take every
   possibility but the rest of the values remain unchanged. </li>
   <li> For more information on the PLA format, see
   ExternalSources/sources/Boolean/Espresso/espresso.5.html . </li>
  </ul>

*/

