// Oliver Kullmann, 3.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Maxima.hpp
  \brief Documentation on how to build Maxima


  <h1> Installing Maxima </h1>


  <h2> On the purpose of Maxima </h2>

  Maxima is a computer-algebra system written in Lisp. It is our entry-level
  language, used for procedural specifications and experimental (smaller)
  computations.


  <h2> What the installation yields </h2>

  <ul>
   <li> The main maxima-executable (an interactive console) </li>
   <li> Documentation </li>
  </ul>
  Normally, the user doesn't need to interact with Maxima directly, but can
  use <code>oklib --maxima</code> (see
  Buildsystem/MasterScript/docus/general.hpp).


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(maxima_recommended_version_number_okl)</code> </li>
   <li> Maxima-program call = <code>$(maxima_call_okl)</code> </li>
   <li> Ready: $(maxima_call_ready_okl)
    <ul>
     <li> location = $(location_maxima_call_okl) </li>
     <li> version = <code>$(version_maxima_call_okl)</code>
     </li>
    </ul>
   </li>
   <li> Documentation: $(maxima_html_documentation_index_location_tag_okl) </li>
  </ul>


  <h2> How to install </h2>

  <ul>
   <li> Only local installation currently. </li>
   <li> The Maxima-build process searches
   for available Lisp-installations; we recommend CLisp (see
   Buildsystem/ExternalSources/SpecialBuilds/docus/CLisp.hpp). </li>
   <li> 


  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> maxima </code> </td>
    <td> Build the recommended version of maxima. </td>
   </tr>
   <tr>
     <td> <code> allmaxima </code> </td>
     <td> Build first CLisp (locally). </td>
   </tr>
   <tr>
    <td> <code> cleanallmaxima </code> </td>
    <td> Removes the build-, installation- and documentation-directories. </td>
   </tr>
   <tr>
    <td> <code> cleanallallmaxima </code> </td>
    <td> Invokes <code>cleanalllibsigsegv,cleanallclisp,cleanallmaxima</code>. </td>
   </tr>
  </table>

  <h3> Configuration </h3>

  <ul>
   <li> To install a different version of Maxima, set variable
   <code>clisp_recommended_version_number_okl</code> accordingly. </li>
  </ul>

*/

