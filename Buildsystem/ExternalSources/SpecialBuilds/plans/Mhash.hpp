// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Mhash.hpp
  \brief Plans regarding installation of the Mhash library


  \bug MHash segmentation fault
  <ul>
   <li> Building with gcc version 4.4.1 on a 64-bit machine we get
   \verbatim
/bin/sh: line 4:  9858 Segmentation fault      ${dir}$tst
FAIL: keygen_test
   \endverbatim
   </li>
   <li> This is a known bug in Mhash; see for example
   https://bugzilla.redhat.com/show_bug.cgi?id=513254 . </li>
   <li> Unfortunately, according to
   http://sourceforge.net/projects/mhash/files/ , the project is from
   2007-08-18 0:00:00 GMT not under active development. </li>
   <li> So, either we fix the bug ourselves, or we stop using mhash. </li>
   <li> In ExternalSources/sources/Mhash/mhash-0.9.9.9-1.fc11.src.rpm
   apparently we have the mhash-package plus patches. </li>
   <li> The question is, how do we extract the package and the patches, and
   apply then the patches? </li>
   <li> For now we stop installing and using it. </li>
  </ul>


  \todo Complete MHash documentation
   
*/

