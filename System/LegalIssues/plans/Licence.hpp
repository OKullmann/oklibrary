// Oliver Kullmann, 8.7.2007 (Swansea)
/* Copyright 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file LegalIssues/plans/Licence.hpp
  \brief Plans regarding licences

  The basic decision is to use GPLv3 (http://www.fsf.org/licensing/licenses/gpl.html)


  \todo Licence maintenance
  <ul>
   <li> We must make sure, that every new non-data file gets the
   licence statement. </li>
   <li> When external developers check in new files, then those must have the
   OK-copyright etc. clause (otherwise the submission is rejected). </li>
   <li> We should also add the creation-information and licence to
   html-files. </li>
   <li> For every release, the year of the release must be added to every
   file's copyright notice (if not already present). This boils down to
   adding the new year to every copyright statement at the beginning
   of a new year. </li>
   <li> "AddLicence2" is more advanced than "AddLicence1", so the latter
   should be updated. </li>
   <li> Also a third type of script is needed for make-files. </li>
   <li> Important that these scripts tell us about "bad" files. </li>
   <li> Part of the licence must be that the full history is handed down. Is
   this guaranteed by the original text, or do we need to amend it?
    <ol>
     <li> E-mail was sent to FSL. </li>
     <li> No answer (6.10.2007); one should try it again. </li>
    </ol>
   </li>

  </ul>

*/

