// Oliver Kullmann, 6.9.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
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
   <li> The <code>R</code> program XXX </li>
   <li> Documentation </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> R interface call = <code>$(R_call)</code> </li>
   <li> ready: $(R_call_ready)
    <ul>
     <li> location = $(location_R_call) </li>
     <li> version = <code>$(version_R_call)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(R_html_documentation_index_location_tag) </li>
  </ul>


  <h2> How to install </h2>

  Only system installation currently.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> R </code> </td>
    <td> Build the recommended R-version, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> R-V </code> </td>
    <td> Build the R-version V, using the system-gcc (for example <code>R-2.5.0</code>). </td>
   </tr>
  </table>

*/

