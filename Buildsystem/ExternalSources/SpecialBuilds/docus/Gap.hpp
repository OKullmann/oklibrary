// Oliver Kullmann, 12.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Gap.hpp
  \brief Documentation on how to build Gap


  <h1> Installing Gap </h1>


  <h2> On the purpose of Gap </h2>

  Gap is a computer-algebra system with special focus on group theory
  and discrete mathematics.


  <h2> What the installation yields </h2>

  <ul>
   <li> The gap-executable (an interactive console), available in
   <code>$(public_bin_dir_okl)</code>. So by entering <code>gap</code> on
   the command-line this system is entered. </li>
   <li> Documentation. </li>
  </ul>
  We install all (standard) packages.


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(gap_extracted_version_number_okl)</code> </li>
   <li> Gap-program call = <code>$(gap_call_okl)</code> </li>
   <li> Ready: $(gap_call_ready_okl)
    <ul>
     <li> location = $(location_gap_call_okl) </li>
     <li> version = <code>$(version_gap_call_okl)</code>
     </li>
    </ul>
   </li>
   <li> Documentation: $(gap_html_documentation_index_location_tag_okl) </li>
  </ul>


  <h2> How to install </h2>

  <ul>
   <li> Only local installation currently. </li>
   <li> There is no build-directory (which could be removed),
   but all is put into the installation-directory. </li>
  </ul>


  <h3> Prerequisites </h3>



  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> gap </code> </td>
    <td> Build the recommended version of Gap. </td>
   </tr>
   <tr>
    <td> <code> cleanallgap </code> </td>
    <td> Removes the installation- and documentation-directories. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version of Gap, set variable
   <code>gap_recommended_version_number_okl</code> accordingly. </li>
  </ul>

*/

