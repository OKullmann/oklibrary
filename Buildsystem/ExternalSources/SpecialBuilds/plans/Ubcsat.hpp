// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp
  \brief Plans regarding installation of the Ubcsat package


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
   <li> First we try what happens on 32-bit and 64-bit machines. </li>
   <li> We should download the now available documentation. </li>
   <li> OK must get in contact with the Ubcsat-group. </li>
  </ul>


  \todo Complete docu-file : DONE


  \todo Check the new Ubcsat version. ?? There doesn't seem to exist one??

*/

