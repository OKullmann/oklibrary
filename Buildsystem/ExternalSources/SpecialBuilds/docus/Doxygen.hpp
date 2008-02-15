// Oliver Kullmann, 23.7.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
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
     <li> The <code>doxygen</code> program:
      <ul>
       <li> Main usage: Takes as input a file with configuration settings,
       and produces a directory with html-pages. </li>
       <li> Can also produce a configuration-file template. </li>
       <li> And can update a configuration file. </li>
      </ul>
     <li> The <code>doxytag</code> program (for index-creation). </li>
    </ol>
   </li>
   <li> Documentation </li>
  </ul>
  The OKlibrary build system provides a link to the doxyen-documentation, and
  administrates a doxygen-configuration file.


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(doxygen_recommended_version_number_okl)</code> </li>
   <li> Paths to the two executables:
    <ol>
     <li> doxygen-program call = <code>$(doxygen_call_okl)</code> </li>
     <li> doxytag-program call = <code>$(doxytag_call_okl)</code> </li>
    </ol>
   </li>
   <li> ready: $(doxygen_call_ready_okl)
    <ul>
     <li> location = $(location_doxygen_call_okl) </li>
     <li> version = <code>$(version_doxygen_call_okl)</code>
     </li>
    </ul>
   </li>
   <li> Documentation: $(doxygen_html_documentation_index_location_tag_okl) </li>
   <li> Doxygen-configuration file:
    <ol>
     <li> Template: <code>$(doxy_file_template)</code> </li>
     <li> Preprocessed configuration file: <code>$(doxy_file)</code> </li>
    </ol>
   </li>
   <li> Output log-file (error messages only): <code>$(doxygen_error_messages)</code>
   </li>
  </ul>


  <h2> How to install </h2>

  <h3> Prerequisites </h3>

  <ul>
   <li> <code>gcc</code> is needed (as for many packages). </li>
   <li> <code>Qt library</code> (often already installed). </li>
  </ul>

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> doxygen </code> </td>
    <td> Calls sub-targets <code>doxygen_base</code> and
    <code>doxygen_links</code> </td>
   </tr>
   <tr>
    <td> <code> doxygen_base </code> </td>
    <td> Build the recommended version of doxygen, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> doxygen_links </code> </td>
    <td> For local installation, creates links in the public bin-directory.
    </td>
   </tr>
   <tr>
    <td> <code> cleanalldoxygen </code> </td>
    <td> Remove build, installation and documentation directory. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> Local versus system-wide installation:
    <ol>
     <li> The default is local installation. </li>
     <li> Set variable <code>doxygen_default_install_okl</code> to value
     "system" to switch. </li>
    </ol>
   </li>
   <li> To install a different version, set variable
   <code>doxygen_recommended_version_number_okl</code> accordingly. </li>
  </ul>


*/

