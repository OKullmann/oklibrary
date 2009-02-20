// Oliver Kullmann, 6.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Backup/plans/Backup.hpp
  \brief Plans regarding the backup-program written by Tony Bao
  \deprecated Likely to be removed.


  \todo What to do with this module?
  <ul>
   <li> In principle it should be useful to have a C++ program for the
   backup. </li>
   <li> On the other hand, there are quite a few tools out there for
   this purpose, so we need to consider what this module offers.
    <ol>
     <li> The main idea was that just a directory with symbolic links
     is created, and the backup-file would just contain the corresponding
     files resp. directories. </li>
     <li> It appeared that this would be convenient, and that existing
     tools didn't support such a use of links. </li>
     <li> But likely other tools allow to create a configuration file,
     which contains in some (more powerful syntax) files and directories
     to be backed up. This would be more explicit. </li>
     <li> So it appears this module is no longer needed; perhaps it contains
     some tools? No, doesn't look like that. </li>
    </ol>
   </li>
   <li> So it seems, that this module should be removed. </li>
   <li> DONE (see "Backups and archives" in OKlib/plans/general.hpp)
   Due to the character of a "holistic library" however, there
   should exist at least some discussion of backing up. </li>
  </ul>

*/

/*!
  \namespace OKlib::Backup
  \brief Components for backups
*/

namespace OKlib {
  namespace Backup {
  }
}

