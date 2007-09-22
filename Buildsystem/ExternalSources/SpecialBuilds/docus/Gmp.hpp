// Oliver Kullmann, 21.8.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Gmp.hpp
  \brief Documentation on how to build and use Gmp


  <h1> Installing and using Gmp </h1>


  <h2> On the purpose of Gmp </h2>

  Gmp is a C-library for computation with integers, rationals and floating point
  numbers of arbitrary size.


  <h2> What the installation yields </h2>

  <ul>
   <li> The gmp.h header file and the corresponding link library (libgmp.a
   and variations). </li>
   <li> Documentation (info-pages; we extract also the html and dvi
   documentation). </li>
   <li> The package default is to install under <code>/usr/local</code>
   (in sub-directories <code>include</code>, <code>lib</code> and
   <code>info</code>). Our default is local installation. </li>
  </ul>


  <h2> Usage </h2>

  <ul>
   <li> To include the gmp-header use
   \code
#include <gmp.h>
   \endcode
   together with "${gmp_include_option}". </li>
   <li> To link with gmp, use "${gmp_link_option}". </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> gmp include option = <code>$(gmp_include_option)</code> </li>
   <li> gmp link option = <code>$(gmp_link_option)</code> </li>
   <li> ready: $(gmp_ready)
    <ul>
     <li> mode = <code>$(gmp_default_install)</code> </li>
     <li> version = <code>$(version_gmp)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(gmp_html_documentation_index_location_tag) </li>
  </ul>


  <h2> How to install </h2>

  Local and system-wide installation possible, the default is local
  installation;
  set variable <code>gmp_default_install</code> to value "system" to switch.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> gmp </code> </td>
    <td> Build the recommended version of gmp, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> gmp-a.b.c </code> </td>
    <td> Build the version a.b.c of gmp, using the system-gcc (for example
    <code>gmp-4.2.1</code>). </td>
   </tr>
  </table>

*/

