// Oliver Kullmann, 23.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Doxygen.hpp
  \brief Documentation on how to build Doxygen


  <h1> Installing Doxygen </h1>


  <h2> On the purpose of Doxygen </h2>

  Doxygen is a program to extract documentation from program sources. We use it
  for the documentation of C and C++ code as well as for the plans and docus.


  <h2> What the installation yields </h2>

  <ul>
   <li> Two executables
    <ol>
     <li> the <code>doxygen</code> program: Takes as input a file with configuration settings, and produces a directory with html-pages. </li>
     <li> the <code>doxytag</code> program </li>
    </ol>
   </li>
   <li> Documentation </li>
  </ul>
  The OKlibrary build system needs access to the doxygen-program, and provides a link
  to the doxyen-documentation. It administrates a doxygen-configuration file.

  <h2> Current state of installation </h2>

  <ul>
   <li> doxygen-program call = <code>$(doxygen_call)</code> </li>
   <li> ready: $(doxygen_call_ready)
    <ul>
     <li> location = $(location_doxygen_call) </li>
     <li> version = <code>$(version_doxygen_call)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(doxygen_html_documentation_index_location_tag) </li>
   <li> doxygen-configuration file XXX </li>
  </ul>


  <h2> How to install </h2>

  Only system-wide installation currently.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> doxygen </code> </td>
    <td> Build the recommended version of doxygen, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> doxygen-x.y.z </code> </td>
    <td> Build the version x.y.z of doxygen, using the system-gcc (for example
    <code>doxygen-1.5.2</code>). </td>
   </tr>
  </table>

*/

