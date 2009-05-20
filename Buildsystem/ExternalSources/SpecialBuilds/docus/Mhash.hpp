// Oliver Kullmann, 6.9.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Mhash.hpp
  \brief Documentation on how to build and use Mhash


  <h1> Installing and using Mhash </h1>


  <h2> On the purpose of Mhash </h2>

  Mhash is a C library with cryptographic tools XXX


  <h2> What the installation yields </h2>

  <ul>
   <li> The mhash header files and the corresponding link libraries XXX </li>
   <li> Documentation XXX </li>
  </ul>


  <h2> Usage </h2>

  <ul>
   <li> To include the mhash-header use
   \code
#include <mhash.h>
   \endcode
   together with "${mhash_include_option_okl}" as option for gcc. </li>
   <li> To link with mhash, use "${mhash_link_option_okl}" as option for gcc. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(mhash_recommended_version_number_okl)</code> </li>
   <li> %mhash include option = <code>$(mhash_include_option_okl)</code> </li>
   <li> %mhash link option = <code>$(mhash_link_option_okl)</code> </li>
   <li> ready: $(mhash_ready_okl)
    <ul>
     <li> version = <code>$(version_mhash_okl)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(mhash_html_documentation_index_location_tag_okl) </li>
  </ul>


  <h2> How to install </h2>

  Some general remarks:
  <ul>
   <li> Currently only local installation is provided (using the local gcc).
   </li>
   <li> If the required local installation of gcc is not available, then it
   needs to be built first (currently this is not handled by the build
   system). </li>
  </ul>


  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> mhash </code> </td>
    <td> Build the recommended version of mhash, using the local gcc. </td>
   </tr>
   <tr>
    <td> <code> cleanallmhash </code> </td>
    <td> Remove build, installation and documentation directory. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version, set variable
   <code>mhash_recommended_version_number_okl</code> accordingly. </li>
   <li> And to use a different version of gcc set variable
   <code>gcc_recommended_version_number_okl</code> accordingly. </li>
  </ul>

*/

