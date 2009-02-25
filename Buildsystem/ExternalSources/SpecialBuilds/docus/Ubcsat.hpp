// Oliver Kullmann, 23.8.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Ubcsat.hpp
  \brief Documentation on how to build and use Ubcsat


  <h1> Installing and using Ubcsat </h1>


  <h2> On the purpose of Ubcsat </h2>

  Ubcsat is a collection of procedures for configuring local search SAT solvers.
  <ul>
   <li> We have tweaked it somewhat to get it closer to a library. </li>
   <li> We corrected the source code so that it runs also on 64 bit machines.
   </li>
   <li> And we made it now flushing it's output. </li>
  </ul>


  <h2> What the installation yields </h2>

  Via <code>oklib ubcsat</code> we obtain the installation directory
  <code>$(ubcsat_installation_dir_okl)</code> which contains the following
  subdirectories:
  <ul>
   <li> <code>bin</code> contains the executable <code>ubscat</code>, a
   solver platform which can execute the main local-search algorithms. A link
   to this program is planted in the public bin-directory
   (which is <code>$(public_bin_dir_okl)</code>). </li>
   <li> <code>lib</code> contains the link-libraries which are linked together
   in the above <code>ubcsat</code> executable, and which are here made available
   for linking. Compiled with the macro ALTERNATEMAIN, so that no main
   function is created. </li>
   <li> The above executable program as well as the link-libraries are compiled
   using the corrected sources from <code>$(ubcsat_corrected_dir_okl)</code>.
   </li>
   <li> The original sources are copied to the <code>src</code> sub-directory
   (converted to Unix/Linux line-endings). </li>
  </ul>


  <h2> Current state of installation </h2>

  We consider here only the executable:
  <ul>
   <li> Recommended version =
   <code>$(ubcsat_recommended_version_number_okl)</code> </li>
   </li>
   <li> ready: $(ubcsat_call_ready_okl)
    <ul>
     <li> location = $(location_ubcsat_call_okl) </li>
     <li> version = <code>$(version_ubcsat_call_okl)</code>
     </li>
    </ul>
   </li>
  </ul>


  <h2> Usage </h2>

  Using the ubcsat-executable:
  <ul>
   <li> A link is provided in the public bin-directory ($(public_bin_dir_okl)).
   </li>
   <li> Using <code>ubcsat -h</code> will show the list of options.
    <ol>
     <li> Most useful seems to start with %e.g.,
     \verbatim
ubcsat -alg rsaps -runs 100 -cutoff 1000 -i file
     \endverbatim
     where for the parameter "-alg" one has to chose one of the available
     algorithms (here "rsaps"). </li>
     <li> In this way one gets a feeling for the quality of the algorithm,
     and how cutoff needs to be adapted to the problem at hand. </li>
     <li> Additionally the usage of the parameter <code>-noimprove value</code>
     is of interest, which aborts a run after value-many steps without
     improvement of the current minimum number of falsified clauses. </li>
    </ol>
   </li>
   <li> <code>ubcsat -ha</code> shows the list of available algorithms.
   Typically one needs to try them all out. </li>
  </ul>

  We provide a convenience-wrapper <code>ubcsat-okl</code> (also in the
  public bin-directory):
  <ul>
   <li> This calls <code>ubcsat</code> with the parameters
   <code>$(ubcsat_defaults_okl)</code>, followed by the parameters given to
   <code>ubcsat-okl</code>. </li>
   <li> So we leave out the banner etc., for each run additionally output the
   seed-value (so that the run can be repeated), and especially provide more
   statistics about the best assignments found. </li>
   <li> The wrapper can be used as ubcsat itself, for example
   \verbatim
ubcsat-okl -alg samd -runs 100 -cutoff 10000 -noimprove 1000 -i file
   \endverbatim
   </li>
  </ul>
  
  To use Ubcsat-components from inside the OKlibrary:
  <ul>
   <li> In the ".link_libraries"-file the specification
   \code 
echo ${ubcsat_link_option_okl} 
   \endcode
   is needed, which enables linking with all functions and variables provided by the
   ubcsat-library. </li>
   <li> For the source-libraries add
   \code
$(ubcsat_include_option_okl)
   \endcode
   to the definition of <code>source_libraries</code> in <code>definitions.mak</code>:
   This activates the localised version of header files from the Ubcsat-library, and,
   if not already overwritten by a localised version, the corrected files (while
   the other files are taken from the original sources). </li>
   <li> And the main header file for the Ubcsat-library is
   OKlib/LocalSearch/Ubcsat/local/ubcsat.h. </li>
  </ul>


  <h2> How to install </h2>

  Only local installation.

  <h3> Make targets </h3>

  <table>
   <tr>
    <td> <code> ubcsat </code> </td>
    <td> Builds the source-library, the link-library and the ubcsat-program,
     using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> cleanubcsat </code> </td>
    <td> Removes the build-directory. </td>
   </tr>
   <tr>
    <td> <code> cleanallubcsat </code> </td>
    <td> Removes build- and installation-directories. </td>
   </tr>
  </table>

*/

