// Oliver Kullmann, 23.7.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Gcc.hpp
  \brief Documentation on how to build Gcc


  <h1> Installing Gcc </h1>


  <h2> On the purpose of Gcc </h2>

  Gcc is a compiler collection:
  <ul>
   <li> For the OKlibrary it provides the main
   C++ and the main C compiler (called "g++" and "gcc"). </li>
   <li> Some external sources also use the Fortran compiler
   (called "gfortran"). </li>
  </ul>
  An existing system-wide installation of <code>gcc</code> is a prerequisite
  for using the OKlibrary (mainly for building external sources).
  <ul>
   <li> However, for compiling the sources of the OKlibrary we need
   full control over the version used. </li>
   <li> And so by default here the local installation (provided by the
   OKlibrary) is used. </li>
   <li> For normal usage of the OKlibrary, the user does not need to care
   about this local installation, but the masterscript "oklib" handles
   everything. </li>
   <li> 
  </ul>


  <h2> What the installation yields </h2>

  <ul>
   <li> Executables
    <ol>
     <li> the C++ compiler <code>g++</code> </li>
     <li> the C compiler <code>gcc</code> </li>
    </ol>
   </li>
   <li> Link libraries (mostly not explicitly used) </li>
   <li> Source libraries (mostly not explicitly used) </li>
   <li> Documentation. </li>
  </ul>
  Since different compiler versions might have different trade-offs, the
  OKlibrary supports the installation and usage of different versions of gcc.


  <h2> Usage </h2>

  Calling <code>g++</code> or <code>gcc</code> is straightforward,
  however the executables need also to know how to link to the
  gcc-libraries:
  <ul>
   <li> On Linux systems, dynamic linking is standard, and so by default
   the link-paths are not hard-coded into the executables. </li>
   <li> Since the OKlibrary tries to avoid making system-wide changes,
   just using the provided <code>g++</code> or <code>gcc</code> will
   compile the code, but the code in general will not run. </li>
   <li> So the executables <code>g++</code> and <code>gcc</code>
   are not provided in the public OKlibrary/bin directory. </li>
   <li> The usage within the OKlibrary is completely handled by the
   masterscript <code>oklib</code>, and so the user normally does not
   need to be concerned with the usage of gcc. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> System gcc
    <ul>
     <li> gcc-program call = <code>$(gcc_system_call_okl)</code> </li>
     <li> ready: $(gcc_system_call_ready_okl)
      <ul>
       <li> location = $(location_gcc_system_call_okl) </li>
       <li> version = <code>$(version_gcc_system_call_okl)</code> </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> System g++
    <ul>
     <li> g++-program call = <code>$(gpp_system_call_okl)</code> </li>
     <li> ready: $(gpp_system_call_ready_okl)
      <ul>
       <li> location = $(location_gpp_system_call_okl) </li>
       <li> version = <code>$(version_gpp_system_call_okl)</code> </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Local gcc (provided by the OKlibrary) in recommend version $(gcc_recommended_version_number_okl)
    <ul>
     <li> documentation: $(gcc_html_documentation_index_location_tag_okl) </li>
     <li> gcc-program call = <code>$(gcc_local_call_okl)</code> </li>
     <li> ready: $(gcc_local_call_ready_okl) </li>
     <li> g++-program call = <code>$(gpp_local_call_okl)</code> </li>
     <li> ready: $(gpp_local_call_ready_okl) </li>
    </ul>
   </li>
  </ul>


  <h2> How to install </h2>

  Some general remarks:
  <ul>
   <li> Only local installation currently (through the OKlibrary). </li>
   <li> For building gcc always the system gcc is used. </li>
  </ul>


  <h3> Make targets </h3>

  <table>
   <tr>
    <td> <code> gcc </code> </td>
    <td> Build the recommended gcc-version. </td>
   </tr>
   <tr>
    <td> <code> cleangcc </code> </td>
    <td> Remove the build-directory regarding the recommended gcc-version. </td>
   </tr>
   <tr>
    <td> <code> cleanallgcc </code> </td>
    <td> Remove all build-, installation- and documentation-directories for all gcc-versions. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version of Gcc, set variable
   <code>gcc_recommended_version_number_okl</code> accordingly. </li>
   <li> By default, languages <code>$(gcc_enable_languages_okl)</code>
   are compiled; for a different choice set variable
   <code>gcc_enable_languages_okl</code>. </li>
   <li> By default, the thread-model is <code>$(gcc_threads_okl)</code>;
   for a different choice set variable <code>gcc_threads_okl</code>. </li>
   <li> Other default configuration options are
   <code>$(gcc_other_options_okl)</code>; for a different choice set variable
   <code>gcc_other_options_okl</code>. </li>
  </ul>


*/

