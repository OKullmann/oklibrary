// Oliver Kullmann, 3.1.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Maxima.hpp
  \brief Documentation on how to build Maxima


  <h1> Installing Maxima </h1>


  <h2> On the purpose of Maxima </h2>

  Maxima is a computer-algebra system, written in Lisp. It is our entry-level
  language, used for procedural specifications and experimental (smaller)
  computations.


  <h2> What the installation yields </h2>

  <ul>
   <li> The main maxima-executable (an interactive console). </li>
   <li> Documentation. </li>
   <li> DISABLED
   Access to the the "Encyclopedia of Integer Sequences" as provided by
   Alexey Beshenov (http://beshenov.ru/maxima/eis/):
    <ol>
     <li> See the demo-file in the installation directory
     <code>$(maxima_eis_installation_dir_okl)</code> for the search
     interface. </li>
     <li> However note that no load is necessary (since automatically
     performed by <code>oklib</code>). </li>
     <li> The database is directory
     <code>$(maxima_eis_base_installation_dir_okl)/A</code> </li>
    </ol>
   </li>
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
   for available (Common-) Lisp-installations:
    <ol>
     <li> We support CLisp (see
     Buildsystem/ExternalSources/SpecialBuilds/docus/CLisp.hpp)
     and Ecl (see Buildsystem/ExternalSources/SpecialBuilds/docus/Ecl.hpp).
     </li>
     <li> At this time Ecl is considerably faster, while (due to some effort
     we spent on it, with the help of the Maxima and the Ecl community)
     no problems are known (anymore). So our default is now Ecl. </li>
    </ol>
   </li>
  </ul>


  <h3> Prerequisites </h3>

  <ul>
   <li> For building and running Maxima the default Lisp is Ecl. </li>
   <li> Moreover, the default is a locally installed Ecl, which needs
   to be installed before; see
   Buildsystem/ExternalSources/SpecialBuilds/docus/Ecl.hpp. </li>
   <li> The Ecl-version used for building Maxima is hard-wired into
   the Maxima installation:
    <ol>
     <li> So the specific Ecl-installation in directory 
     <code>$(ecl_base_installation_dir_okl)</code>
     used for installing Maxima may not be removed. </li>
     <li> With the current settings this Ecl-installation would be
     <code>$(ecl_installation_dir_okl)</code>. </li>
     <li> Installing different Ecl-versions later on the other
     hand does not affect the already built Maxima-installation. </li>
    </ol>
    Analogous statements hold for CLisp (which may be used alternatively).
   </li>
   <li> The Maxima <code>draw</code> command (as well as
   <code>graph_draw</code>) uses Gnuplot, and version 4.2 or later is required.
   It can be installed locally as below (creating a link to the binary in the
   OKplatform/bin directory). </li>
  </ul>


  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> maxima </code> </td>
    <td> Build the recommended version of Maxima. </td>
   </tr>
   <tr>
     <td> <code> allmaxima </code> </td>
     <td> Build first Ecl (locally), then Gnuplot (locally),
     and then Maxima. </td>
   </tr>
   <tr>
    <td> <code> cleanmaxima </code> </td>
    <td> Removes the build-directory. </td>
   </tr>
   <tr>
    <td> <code> cleanallmaxima </code> </td>
    <td> Removes the build-, installation- and documentation-directories. </td>
   </tr>
   <tr>
    <td> <code> cleanallallmaxima </code> </td>
    <td> Invokes <code>cleanallecl,cleanallgnuplot,cleanallmaxima</code>. </td>
   </tr>
   <tr>
    <td> <code> gnuplot </code> </td>
    <td> Build the recommended version of Gnuplot. </td>
   </tr>
   <tr>
    <td> <code> cleangnuplot </code> </td>
    <td> Removes the build-directory. </td>
   </tr>
   <tr>
    <td> <code> cleanallgnuplot </code> </td>
    <td> Removes the build and installation-directories. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version of Maxima, set variable
   <code>maxima_recommended_version_number_okl</code> accordingly. </li>
   <li> To install a different version of Gnuplot, set variable
   <code>gnuplot_recommended_version_number_okl</code> accordingly. </li>
   <li> To install CLisp, set <code>maxima_lisp_name_okl=clisp</code>
   (while the default value is "ecl"). </li>
  </ul>

*/

