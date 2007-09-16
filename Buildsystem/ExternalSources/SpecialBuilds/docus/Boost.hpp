// Oliver Kullmann, 21.8.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Boost.hpp
  \brief Documentation on how to build and use Boost


  <h1> Installing and using Boost </h1>


  <h2> On the purpose of Boost </h2>

  Boost is the main C++ library besides the standard library, for all purposes,
  and in general of the highest standard.


  <h2> What the installation yields </h2>

  <ul>
   <li> The %boost header files and the corresponding link libraries XXX </li>
   <li> Documentation XXX </li>
  </ul>


  <h2> Usage </h2>

  <ul>
   <li> To include a boost-header use for example
   \code
#include <boost/iostreams/categories.hpp>
   \endcode
   together with "${boost_include_option}" XXX (currently "$(Boost)" XXX). </li>
   <li> To link with %Boost, use "${boost_link_option}" XXX for each sub-library separately XXX. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version = <code>$(boost_recommended_version_number)</code> </li>
   <li> %boost include option = <code>$(boost_include_option)</code> XXX </li>
   <li> %boost link option = <code>$(boost_link_option)</code> XXX </li>
   <li> ready: $(boost_ready)
    <ul>
     <li> mode = <code>$(boost_default_install)</code> </li>
     <li> version = <code>$(version_boost)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(boost_html_documentation_index_location_tag) </li>
   <li> list of supported versions XXX </li>
  </ul>


  <h2> How to install </h2>

  <ul>
   <li> Local and system-wide installation possible; the default is local
   installation if not configure otherwise. XXX yet only local installation XXX </li>
   <li> Set variable <code>boost_default_install</code> to value "system" to switch. XXX </li>
   <li> If some required local installation of gcc is not available, then it is build first. XXX </li>
  </ul>

  <h3> Make targets </h3>

  The targets are <code>%boost</code>, <code>boost_all</code> and <code>boost_gcc_all</code>, with modifiers <code>boost_recommended=</code> and <code>gcc-version=</code>.
  
  <table>
   <tr>
    <td> <code> %boost </code> </td>
    <td> Build the recommended Boost-version, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> %boost-V </code> </td>
    <td> Build the Boost-version V, using the system-gcc (using for example <code>boost-1_33_1</code>). </td>
   </tr>
   <tr>
    <td> <code> %boost %boost_recommended=boost-V </code> </td>
    <td> Same as <code>%boost-V</code>. </td>
   </tr>
   <tr>
    <td> <code> boost_all </code> </td>
    <td> Build all supported Boost-versions, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> %boost gcc-version=x.y.z </code> </td>
    <td> Build the recommended Boost-version, using the local gcc-version x.y.z (using for example <code>gcc-version=4.1.2</code>). </td>
   </tr>
   <tr>
    <td> <code> %boost gcc-version=all </code> </td>
    <td> Build the recommended Boost-version for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> %boost %boost_recommended=boost-V gcc-version=x.y.z </code> </td>
    <td> Build the Boost-version V, using the local gcc-version x.y.z. </td>
   </tr>
   <tr>
    <td> <code> %boost-V+x.y.z </code> </td>
    <td> Build the Boost-version V, using the local gcc-version x.y.z. </td>
   </tr>
   <tr>
    <td> <code> %boost %boost_recommended=boost-V gcc-version=all </code> </td>
    <td> Build the Boost-version V, for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> boost_gcc_all </code> </td>
    <td> Build all supported Boost-versions for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
  </table>

*/

