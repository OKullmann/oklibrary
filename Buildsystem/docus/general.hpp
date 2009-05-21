// Oliver Kullmann, 25.8.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/docus/general.hpp
  \brief The central docus-file for the buildsystem


  <h1> Overview on the buildsystem </h1>

  The build-system is called via <code>oklib</code> (Buildsystem/MasterScript/docus/general.hpp), which
  delegates the targets to the makefiles documented here:
  <ul>
   <li> External sources building : Buildsystem/ExternalSources/docus/general.hpp </li>
   <li> The master-script "oklib" : Buildsystem/MasterScript/docus/general.hpp </li>
   <li> %OKlib building : Buildsystem/OKlibBuilding/docus/general.hpp </li>
  </ul>

  For the very initial set-up:
  <ol>
   <li> Call first <code>oklib --setup</code>, </li>
   <li> then call <code>oklib --prebuild</code>. </li>
  </ol>
  If the path to <code>oklib</code> has not been set yet, then use
  <code>$(oklib_call)</code>.


  For operating problems arising at the level of the operating system
  and its tools, see Buildsystem/docus/Linux.hpp.

*/


