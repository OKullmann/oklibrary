// Oliver Kullmann, 26.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/plans/CheckBuildSystem.hpp
  \brief Plans regarding testing the build system


  \todo %Test script:
  We need some test system for the build system. Optimally, it would run
  like our normal test system; perhaps this is hard to achieve, but at
  least we need a list of manual checks, well specified, which cover
  all functions of the build system, and which is performed from time to
  time (manually). Then we can partially automate it.
  <ul>
   <li> make prebuild </li>
   <li> make all </li>
   <li> make check </li>
   <li> make new_check </li>
   <li> make clean (in all variations) </li>
   <li> make html </li>
   <li> make %boost etc. (all external sources) </li>
   <li> make package (the future package building) </li>
  </ul>


  \todo Different environments
  <ul>
   <li> We should try to check different Linux distributions:
    <ol>
     <li> Suse 10.0 or greater </li>
     <li> RedHat ??? </li>
     <li> Debian </li>
    </ol>
  </ul>

*/

