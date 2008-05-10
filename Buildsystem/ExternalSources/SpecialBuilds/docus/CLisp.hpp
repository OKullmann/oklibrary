// Oliver Kullmann, 26.12.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/CLisp.hpp
  \brief Documentation on how to build CLisp


  <h1> Installing CLisp </h1>


  <h2> On the purpose of CLisp </h2>

  The computer algebra system is implemented in "Common Lisp", and
  CLisp is an implementation of this form of Lisp, suitable for the
  computer algebra system Maxima.


  <h2> What the installation yields </h2>

  <ul>
   <li> The clisp-executable, compiling lisp-files as well as an
   interactive interpreter. </li>
   <li> Documentation on Common Lisp in general, and on the usage of
   the clisp-executable. </li>
   <li> Library-files, needed to execute lisp-files. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version: $(clisp_recommended_version_number_okl) </li>
   <li> Checking the installation:
    <ol>
     <li> clisp-program call = <code>$(clisp_call_okl)</code> </li>
     <li> ready: $(clisp_call_ready_okl)
      <ul>
       <li> location = $(location_clisp_call_okl) </li>
       <li> version = <code>$(version_clisp_call_okl)</code> </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  <h2> How to install </h2>

  <ul>
   <li> CLisp comes with Linux distributions, so you could install it
   with your distribution tool. See below for configuration. </li>
   <li> We recommend local installation. </li>
  </ul>

  <h3> Prerequisites </h3>

  <ul>
   <li> "libsigsegv" is needed (the clisp installation process will
   tell you about it); it can be installed as shown below. </li>
  </ul>


  <h3> Make targets </h3>

  Only local installation currently.
  <table>
   <tr>
    <td> <code> clisp </code> </td>
    <td> Build the recommended version of clisp. </td>
   </tr>
   <tr>
    <td> <code> clisp-x.y </code> </td>
    <td> Build the version x.y of clisp (for example
    <code>clisp-2.43</code>). </td>
   </tr>
   <tr>
    <td> <code> cleanallclisp </code> </td>
    <td> Removes all clisp build/installation/documentation directories. </td>
   </tr>
   <tr>
    <td> <code> libsigsegv </code> </td>
    <td> Build the recommended version of libsigsegv. </td>
   </tr>
   <tr>
    <td> <code> libsigsegv-x.y </code> </td>
    <td> Build the version x.y of libsigsegv (for example
    <code>libsigsegv-2.5</code>). </td>
   </tr>
   <tr>
    <td> <code> cleanalllibsigsegv </code> </td>
    <td> Removes all libsigsegv build/installation/documentation directories. </td>
   </tr>
  </table>

*/

