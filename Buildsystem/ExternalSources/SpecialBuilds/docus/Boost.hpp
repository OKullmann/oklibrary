// Oliver Kullmann, 21.8.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
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
   together with "${boost_include_option_okl}" XXX XXX). </li>
   <li> To link with %Boost, use "${boost_link_option_okl}" XXX for each sub-library separately XXX. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version = <code>$(boost_recommended_version_number)</code> </li>
   <li> %boost include option = <code>$(boost_include_option_okl)</code> XXX </li>
   <li> %boost link option = <code>$(boost_link_option_okl)</code> XXX </li>
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

  Some general remarks:
  <ul>
   <li> Currently only local installation is provided. </li>
   <li> If the required local installation of gcc is not available, then it
   needs to be built first (currently this is not handled by the build
   system). </li>
  </ul>


  <h3> Make targets </h3>

  <table>
   <tr>
    <td> <code> %boost </code> </td>
    <td> Build the recommended Boost-version, using the local gcc. </td>
   </tr>
   </table>


  <h3> Configuration </h3>

  <ul>
   </ul>

*/

