// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp
  \brief Plans regarding installation of the Ubcsat package


  \bug Ubcsat source files are read-only
  <ul>
   <li> The permissions of the sources files in "src/" for the Ubcsat source
   tarball are "444", which causes issues when they are copied during any
   rebuild (therefore keeping the same permissions) to the 
   "Installations/SAT/Ubcsat" directory, as the user then doesn't have 
   permission to overwrite a read-only file. </li> 
   <li> It seems two solutions here are to either 
    <ol>
     <li> Use "cp -f" rather than "cp" to force removal of the files first 
     rather than simply overwriting them. </li>
     <li> "chmod" the files during the build to 644. </li>
    </ol>
   </li>
  </ul>


  \bug Ubcsat does not flush the output-buffer
  <ul>
   <li> Flushing the output buffer therefore is left to the operating system. 
   </li>
   <li> Problems then occur when redirecting output to a file (i.e the output
   buffer is not flushed at all until the end). </li>
   <li> Ubcsat seems to have a "-rflush" option, which flushes the output 
   buffer after each run, but this was introduced in version 1.1.0 (See
   http://www.satlib.org/ubcsat/revisions.txt ). </li>
  </ul>


  \bug Cutoff value etc. should be 64 bits on a 64-bit machine
  <ul>
   <li> Yet "unsigned int" is used for example for the cutoff-value,
   not allowing big experimentation. </li>
   <li> Tell the Ubcsat people. </li>
  </ul>


  \todo Investigating the weak performance of Ubcsat on OK's 32-bit machine
  <ul>
   <li> See "Bounds on NR([q,q],2)" in
   Experimentation/Investigations/plans/RamseyProblems.hpp on some data
   on the problem. </li>
   <li> We should send the Ramsey-instance to the Ubcsat-people, to see
   whether they think the performance on csltok is pretty bad (or perhaps
   the performance on cs-wsok is pretty good). </li>
  </ul>


  \todo Building Ubcsat (OK, ML)
  <ul>
   <li> The Ubcsat installation seems pretty up-to-date, but we should
   check the details (only concerning 1.0.0). </li>
   <li> See "Installation of UBCSAT completed" in
   Satisfiability/Algorithms/LocalSearch/plans/general.hpp. </li>
   <li> DONE (updated to the new system, where only the "recommended"
   version number is provided)
   Yet the installation of a different version than the "recommended"
   version wouldn't work, since the directory-names contain the recommended
   version number (and not the one dictated by the target).
    <ol>
     <li> One could leave it like that --- if a version different from
     the recommended shall be used then the recommended version needs
     to be overridden. </li>
     <li> But then we don't need to provide different targets. </li>
    </ol>
   </li>
   <li> DONE Remove the hardcoded version-numbers. </li>
   <li> DONE (removed)
   What is the meaning of "tag" and "tag_" ??? The names should tell. </li>
   <li> No file modification in the external sources. DONE </li>
  </ul>


  \todo Update to version 1.1.0
  <ul>
   <li> First we try what happens on 32-bit and 64-bit machines.
    <ol>
     <li> On cs-wsok (64bit): The provided executable seems to work, but
     the compiled executable yields an error:
     \verbatim
builds/SAT/Ubcsat> ./ubcsat -alg rnovelty+ -i ~/SAT-Algorithmen/OKplatform/GreenTao_2_5_33500.cnf
#
# UBCSAT version 1.1.0 (Sea to Sky Release)
#
# http://www.satlib.org/ubcsat
#
# ubcsat -h for help
#
Error: Invalid Literal [-31473] in clause [0]
     \endverbatim
     Likely this is due to incorrect assumptions on integer types. </li>
    </ol>
   </li>
   <li> We should download the now available documentation. </li>
   <li> OK and MG must get in contact with the Ubcsat-group. </li>
   <li> See "Contact the developers of Ubcsat" in
   Satisfiability/Algorithms/LocalSearch/plans/general.hpp. </li>
  </ul>


  \todo Complete docu-file : DONE


  \todo DONE (see above)
  Check the new Ubcsat version. ?? There doesn't seem to exist one??

*/

