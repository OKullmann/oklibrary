// Oliver Kullmann, 6.9.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/R.hpp
  \brief Documentation on how to build R


  <h1> Installing R </h1>


  <h2> On the purpose of R </h2>

  R is a powerful system for statistical computations, used by the OKlibrary
  for all statistical computations.


  <h2> What the installation yields </h2>

  <ul>
   <li> The <code>R</code> program (usable in interactive- and in
   batch-mode). </li>
   <li> Documentation </li>
  </ul>
  Via <code>oklib --R</code> (see Buildsystem/MasterScript/docus/general.hpp)
  the user can call the installed R-program.


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(R_recommended_version_number_okl)</code> </li>
   <li> R interface call = <code>$(R_call_okl)</code> </li>
   <li> ready: $(R_call_ready_okl)
    <ul>
     <li> location = $(location_R_call_okl) </li>
     <li> version = <code>$(version_R_call_okl)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(R_html_documentation_index_location_tag_okl) </li>
  </ul>


  <h2> How to install </h2>


  <h3> Prerequisites </h3>

  <ul>
   <li> <code>gcc</code> is needed (as for many packages). </li>
   <li> <code>mpfr</code> (often already installed). </li>
  </ul>


  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> R </code> </td>
    <td> Build the recommended R-version, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> cleanallR </code> </td>
    <td> Remove build, installation and documentation directory. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> Local versus system-wide installation:
    <ol>
     <li> The default is local installation. </li>
     <li> Set variable <code>R_default_install_okl</code> to value
     "system" to switch. </li>
    </ol>
   </li>
   <li> To install a different version, set variable
   <code>R_recommended_version_number_okl</code> accordingly. </li>
  </ul>

*/

