// Oliver Kullmann, 15.9.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/CMake.hpp
  \brief Documentation on how to build CMake


  <h1> Installing CMake </h1>


  <h2> On the purpose of CMake </h2>

  CMake is a build tools, used by some external sources.


  <h2> What the installation yields </h2>

  <ul>
   <li> The main executables:
    <ol>
     <li> the <code>cmake</code> program. </li>
    </ol>
   </li>
   <li> Documentation </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(cmake_recommended_version_number_okl)</code> </li>
   <li> Paths to the executables:
    <ol>
     <li> cmake program call = <code>$(cmake_call_okl)</code> </li>
    </ol>
    Note that symbolic links are provided in $(public_bin_dir_okl).
   </li>
   <li> Ready: $(cmake_call_ready_okl)
    <ul>
     <li> location = $(location_cmake_call_okl) </li>
     <li> version = <code>$(version_cmake_call_okl)</code>
     </li>
    </ul>
   </li>
   <li> Documentation: $(cmake_html_documentation_index_location_tag_okl) </li>
  </ul>


  <h2> How to install </h2>

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> cmake </code> </td>
    <td> Build the recommended version of cmake, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> cleancmake </code> </td>
    <td> Remove build directory. </td>
   </tr>
   <tr>
    <td> <code> cleanallcmake </code> </td>
    <td> Remove build, installation and documentation directory. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> Local versus system-wide installation:
    <ol>
     <li> The default is local installation. </li>
     <li> Set variable <code>cmake_default_install_okl</code> to value
     "system" to switch. </li>
    </ol>
   </li>
   <li> To install a different version, set variable
   <code>cmake_recommended_version_number_okl</code> accordingly. </li>
  </ul>

*/

