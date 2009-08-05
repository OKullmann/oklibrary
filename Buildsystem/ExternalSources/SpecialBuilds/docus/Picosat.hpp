// Matthew Gwynne, 31.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Picosat.hpp
  \brief Documentation on how to build and use Picosat


  <h1> Installing Picosat </h1>


  <h2> On the purpose of Picosat </h2>

  Picosat is an conflict-driven SAT solver. It also provides a
  variety of specialised functionality including the production of UNSAT proof
  traces.


  <h2> What the installation yields </h2>

  <ul>
   <li> The SAT solver, accessible via <code>$(picosat_exec_okl)</code>. </li>
   <li> Some documentation in directory <code>$(picosat_doc_dir_okl)</code>.
   </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(picosat_recommended_version_number_okl)</code> </li>
   <li> Expected path to the executable:
    <ol>
     <li> picosat program call = <code>$(picosat_call_okl)</code> </li>
     <li> A symbolic link is provided in the public bin-directory
     (<code>$(public_bin_dir_okl)</code>). </li>
    </ol>
   </li>
   <li> Checking the installation: 
    <ul>
     <li> Ready: $(picosat_call_ready_okl) </li>
     <li> location = $(location_picosat_call_okl) </li>
     <li> version = <code>$(version_picosat_call_okl)</code>
     </li>
    </ul>
   </li>
  </ul>


  <h2> How to install </h2>

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> picosat </code> </td>
    <td> Build the recommended version of picosat, using the local gcc. </td>
   </tr>
   <tr>
    <td> <code> cleanpicosat </code> </td>
    <td> Remove the build directory. </td>
   </tr>
   <tr>
    <td> <code> cleanallpicosat </code> </td>
    <td> Remove build, installation and documentation directory. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version, set variable
   <code>picosat_recommended_version_number_okl</code> accordingly. </li>
  </ul>


  <h2> Usage </h2>

  <ul>
   <li> Use <code>$(picosat_exec_okl) filename</code> (where, as usual,
   "filename" is a SAT-instance in DIMACS format). </li>
  </ul>

*/

