// Oliver Kullmann, 23.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Gcc.hpp
  \brief Documentation on how to build Gcc


  <h1> Installing Gcc </h1>


  <h2> On the purpose of Gcc </h2>

  Gcc is a compiler collection; for the OKlibrary it provides the main
  C++ (and C) compiler (called "g++" and "gcc", and some external sources
  also use the Fortran compiler (called "gfortran").

  An existing system-wide installation of <code>gcc</code> is a prerequisite
  for using the OKlibrary.


  <h2> What the installation yields </h2>

  <ul>
   <li> Executables
    <ol>
     <li> the C++ compiler <code>g++</code> </li>
     <li> the C compiler <code>gcc</code> </li>
    </ol>
   </li>
   <li> Source libraries </li>
   <li> Link libraries </li>
   <li> Documentation </li>
  </ul>
  The OKlibrary build system needs access to the gcc- and the g++-program,
  and provides a link to the gcc-documentation.
  Since different compiler versions might have different trade-offs, the
  OKlibrary supports the installation and usage of different versions of gcc.
  Furthermore usage of a system gcc and of local gcc's is supported.

  <h2> Current state of installation </h2>

  <ul>
   <li> System gcc
    <ul>
     <li> gcc-program call = <code>$(gcc_system_call)</code> </li>
     <li> ready: $(gcc_system_call_ready)
      <ul>
       <li> location = $(location_gcc_system_call) </li>
       <li> version = <code>$(version_gcc_system_call)</code> </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> System g++
    <ul>
     <li> g++-program call = <code>$(gpp_system_call)</code> </li>
     <li> ready: $(gpp_system_call_ready)
      <ul>
       <li> location = $(location_gpp_system_call) </li>
       <li> version = <code>$(version_gpp_system_call)</code> </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Local gcc (provided by the OKlibrary) in recommend version $(gcc_recommended_version_number)
    <ul>
     <li> documentation: $(gcc_html_documentation_index_location_tag) </li>
     <li> gcc-program call = <code>$(gcc_local_call)</code> </li>
     <li> ready: $(gcc_local_call_ready) </li>
     <li> g++-program call = <code>$(gpp_local_call)</code> </li>
     <li> ready: $(gpp_local_call_ready) </li>
    </ul>
   </li>
  </ul>


  <h2> How to install </h2>

  Only local installation currently. For building gcc always the system gcc is used.

  <h3> Make targets </h3>

  <table>
   <tr>
    <td> <code> gcc </code> </td>
    <td> Build the recommended gcc-version. </td>
   </tr>
   <tr>
    <td> <code> gcc_x.y.z </code> </td>
    <td> Build the gcc version x.y.z (use for example <code>gcc_4.1.2</code>). </td>
   </tr>
   <tr>
    <td> <code> gcc_all </code> </td>
    <td> Build all supported gcc-versions. </td>
   </tr>
  </table>


  <h2> Usage </h2>

  XXX setting the link-path XXX

*/

