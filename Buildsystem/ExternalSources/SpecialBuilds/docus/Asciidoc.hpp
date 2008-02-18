// Oliver Kullmann, 18.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Asciidoc.hpp
  \brief Documentation on how to build Asciidoc


  <h1> Installing Asciidoc </h1>


  <h2> On the purpose of Asciidoc </h2>

  <ul>
   <li> Asciidoc is a prerequisite for installing Git (see
   Buildsystem/ExternalSources/SpecialBuilds/docus/Git.hpp). </li>
   <li> Often it is already installed; if not it should be supplied
   by your Linux distribution. </li>
   <li> Otherwise we supply a system-installation (no local installation,
   since this program belongs (only) to the system-environment). </li>
   <li> The purpose of Asciidoc is to translate text files in AsciiDoc format
   to DocBook, HTML or LinuxDoc format; we do not use this tool in the
   OKlibrary. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(asciidoc_recommended_version_number_okl)</code> </li>
   <li> Asciidoc program call = <code>$(asciidoc_call_okl)</code> </li>
   <li> Ready: $(asciidoc_call_ready_okl)
    <ul>
     <li> location = $(location_asciidoc_call_okl) </li>
     <li> version = <code>$(version_asciidoc_call_okl)</code>
     </li>
    </ul>
   </li>
  </ul>


  <h2> How to install </h2>

  Only system-wide installation.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> asciidoc </code> </td>
    <td> Install the recommended version of asciidoc. </td>
   </tr>
   <tr>
    <td> <code> cleanallasciidoc </code> </td>
    <td> Remove build directory. </td>
   </tr>
  </table>

*/

