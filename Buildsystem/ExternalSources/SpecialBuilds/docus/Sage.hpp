// Oliver Kullmann, 9.8.2007 (Swansea)
/* Copyright 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Sage.hpp
  \brief Documentation on how to build Sage


  <h1> Installing Sage </h1>


  <h2> On the purpose of Sage </h2>

  Sage is a computer-algebra platform with uniform access to several platform.
  <ul>
   <li> If necessary, we use it besides Maxima (and our other systems). </li>
   <li> Though we strongly prefer to directly use the various underlying
   systems. </li>
   <li> Thus we only use original Sage-functionality, not the interfaces to
   other systems (except for trying things out). </li>
  </ul>


  <h2> What the installation yields </h2>

  <ul>
   <li> The main sage-executable (an interactive console), available in
   <code>$(public_bin_dir_okl)</code>. So by entering <code>sage</code> on
   the command-line this system is entered. </li>
   <li> Documentation. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(sage_recommended_version_number_okl)</code> </li>
   <li> Sage-program call = <code>$(sage_call_okl)</code> </li>
   <li> Ready: $(sage_call_ready_okl)
    <ul>
     <li> location = $(location_sage_call_okl) </li>
     <li> version = <code>$(version_sage_call_okl)</code>
     </li>
    </ul>
   </li>
   <li> Documentation: $(sage_html_documentation_index_location_tag_okl) </li>
  </ul>


  <h2> How to install </h2>

  <ul>
   <li> Only local installation currently. </li>
   <li> There is no build-directory (which could be removed),
   but all is put into the installation-directory. </li>
  </ul>


  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> sage </code> </td>
    <td> Build the recommended version of sage. </td>
   </tr>
   <tr>
    <td> <code> cleanallsage </code> </td>
    <td> Removes the installation- and documentation-directories. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version of Sage, set variable
   <code>sage_recommended_version_number_okl</code> accordingly. </li>
  </ul>


*/

