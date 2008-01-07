// Oliver Kullmann, 13.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/plans/SetUp.hpp
  \brief Plans for the makefile "SetUp.mak", which does the very first set-up.


  \todo Further actions by the setup-process:
  <ul>
   <li> The log-link is currently only set by SetUp.mak, but when moving
   system_directories, then this link becomes stale (while SetUp.mak is
   not equipped to update the link): We should document this; the easy
   solution seems to be just to manually redefine the log-link, but it
   should also be possible via oklib. </li>
   <li> Create a copy in ~/bin of the oklib-script, and make it executable. </li>
  </ul>

*/

