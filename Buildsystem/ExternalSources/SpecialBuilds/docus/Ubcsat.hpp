// Oliver Kullmann, 23.8.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Ubcsat.hpp
  \brief Documentation on how to build and use Ubcsat


  <h1> Installing and using Ubcsat </h1>


  <h2> On the purpose of Ubcsat </h2>

  Ubcsat is a collection of procedures for configuring local search SAT solvers. We have tweaked it somewhat to get it closer to a library, and we corrected
  the source code so that it runs also on 64 bit machines.


  <h2> What the installation yields </h2>

   Into $(ExternalSources)/Ubcsat/$(ubcsat_recommended_version_number) the
   archive ubcsat-$(ubcsat_recommended_version_number).tar.gz is extracted,
   which contains pre-compiled binaries for Unix/Linux systems and Windows systems.
   There are no changes, except that the source-files in sub-directory src are converted
   to Unix/Linux line-endings. XXX

  The build adds sub-directories bin and lib:
  <ol>
   <li> In bin the ubcsat-executable is compiled with corrected sources. </li>
   <li> In lib one finds the corresponding link-library, also compiled with the
   corrected source, and now defining the macro ALTERNATEMAIN, so that no main
   function is created. </li>
  </ol>
  The corrected sources come from Transitional/LocalSearch/Ubcsat/corrected. XXX


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version = <code>$(ubcsat_recommended_version_number)</code> </li>
   <li> ubcsat include option = <code>$(ubcsat_include_option)</code> XXX </li>
   <li> %ubcsat link option = <code>$(ubcsat_link_option)</code> XXX </li>
   <li> ready library: $(ubcsat_ready) XXX
   </li>
   <li> ready executable XXX
    <ul>
     <li> location = $(location_ubcsat_call) </li>
     <li> version = <code>$(version_ubcsat_call)</code>
     </li>
    </ul>
   </li>
  </ul>


  <h2> Usage </h2>

  XXX

  Calling the binary with "ubcsat -h" will show the list of options.

  To use Ubcsat-components from inside the OKlibrary, in the ".link_libraries"-file the specification
  \code echo -L${Ubcsat_dir}/1-0-0/lib -lubcsat \endcode
  is needed XXX, which enables linking with all functions and variables provided by the ubcsat-library. For the source-libraries add
  \code -I- -I$(OKsystem)/Transitional/LocalSearch/Ubcsat/local -I$(OKsystem)/Transitional/LocalSearch/Ubcsat/corrected $(Ubcsat) \endcode
  to the definition of <code>source_libraries</code> in <code>definitions.mak</code>:
  This activates the localised version of header files from the Ubcsat-library, and,
  if not already overwritten by a localised version, the corrected files (while
  the other files are taken from ExternalSources/Ubcsat/1-0-0/src).

  The main header file for the Ubcsat-library is
  Transitional/LocalSearch/Ubcsat/local/ubcsat.h.


  <h2> How to install </h2>

  Only local installation.

  <h3> Make targets </h3>

  <table>
   <tr>
    <td> <code> ubcsat </code> </td>
    <td> Builds the source-library, the link-library and the ubcsat-program in
    ExternalSources/Ubcsat/1-0-0, using the system-gcc. </td>
   </tr>
  </table>

  XXX

*/

