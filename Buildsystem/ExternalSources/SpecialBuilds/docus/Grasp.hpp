// Oliver Kullmann, 18.7.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Grasp.hpp
  \brief Documentation on how to build and use Grasp


  <h1> Installing Grasp </h1>


  <h2> On the purpose of Grasp </h2>

  Grasp was the first "conflict-driven" SAT solver.


  <h2> What the installation yields </h2>

  <ul>
   <li> The SAT solver, accessible via <code>$(grasp_exec_okl)</code>. </li>
   <li> Full documentation in directory <code>$(grasp_doc_dir_okl)</code>.
   </li>
   <li> We compile it with gcc version 4.1.2, since compilation fails for
   newer versions (due to the use of non-standard headers like "iostream.h").
   </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(grasp_recommended_version_number_okl)</code> </li>
   <li> Expected path to the executable:
    <ol>
     <li> grasp program call = <code>$(grasp_call_okl)</code> </li>
     <li> A symbolic link is provided in the public bin-directory
     (<code>$(public_bin_dir_okl)</code>). </li>
    </ol>
   </li>
   <li> Checking the installation: 
    <ul>
     <li> Ready: $(grasp_call_ready_okl) </li>
     <li> location = $(location_grasp_call_okl) </li>
     <li> version = <code>$(version_grasp_call_okl)</code>
     </li>
    </ul>
   </li>
   <li> The main documentation page: $(grasp_html_documentation_index_location_tag_okl) </li>
  </ul>


  <h2> How to install </h2>

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> grasp </code> </td>
    <td> Build the recommended version of grasp, using the local gcc. </td>
   </tr>
   <tr>
    <td> <code> cleangrasp </code> </td>
    <td> Remove the build directory. </td>
   </tr>
   <tr>
    <td> <code> cleanallgrasp </code> </td>
    <td> Remove build, installation and documentation directory. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version, set variable
   <code>grasp_recommended_version_number_okl</code> accordingly. </li>
  </ul>


  <h2> Usage </h2>

  <ul>
   <li> Use <code>$(grasp_exec_okl) filename</code> (where, as usual,
   "filename" is a SAT-instance in DIMACS format). </li>
   <li> All resources are restricted; the default values are as follows:
    <ol>
     <li> <code>+T 10000</code> (maximal runtime in seconds; default is
     thus nearly 3 hours). </li>
    </ol>
   </li>
  </ul>

*/

