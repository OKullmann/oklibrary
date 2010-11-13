// Oliver Kullmann, 8.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ReleaseProcess/plans/general.hpp
  \brief Plans regarding the release process in general

  The different aspects of the release process (including the "continuous
  releases" via source-control access) are considered in:
  <ul>
   <li> Buildsystem/ReleaseProcess/plans/Release.hpp looks at the overall
   process for preparing a (special) release. </li>
   <li> Buildsystem/ReleaseProcess/plans/MailingLists.hpp treats all different
   aspects regarding e-mail communication. </li>
   <li> Buildsystem/ReleaseProcess/plans/PackageBuilding.hpp considers building
   the packages for the releases. </li>
   <li> Buildsystem/ReleaseProcess/plans/README.hpp looks at the README-file
   in the package. </li>
  </ul>


  \todo Packages history : DONE
  <ul>
   <li> DONE With every new package a summary about the progress is added to a
   subpage of Local/History.html. See "Package history" in
   Buildsystem/Html/Local/plans/History.hpp. </li>
   <li> DONE
   Compare "Backups and archives" in OKlib/plans/general.hpp
   for the "official history". </li>
   <li> DONE (no packages are kept --- they can be recreated via the Git
   history)
   A policy is needed which packages to be kept.
    <ol>
     <li> Somewhere there should be already some discussion on
     that? </li>
     <li> In any case only the minimal packages are kept. </li>
     <li> Perhaps the packages created with every advance in the
     version number (disregarding the fourth digit). </li>
     <li> There could be months between advances in the version number,
     but otherwise we have too much to store, and there should be enough
     clones floating around in case of a dispute regarding the recent
     history would show up (which seems unlikely). </li>
     <li> However the policy about archiving packages should ask for
     additional archiving of packages in case for a long time no advance
     happened while there is activity (this should be prevented by
     reasonable milestones). </li>
    </ol>
   </li>
   <li> DONE (the "official history" is handled by "Backups and archives"
   in OKlib/plans/general.hpp)
   Can we make the OKlibrary "officially" available? Or "more
   official"?? Perhaps we need a Sourceforge-account??? The Git-history should
   be well-secured. </li>
  </ul>


  \todo DONE (there is only a permanent release process via packages
  created every few days or so; to the outside
  world it matters which functionality is provided, which is regulated
  by the milestones, however this does not affect the release process;
  yet no experience with access to git-repositories by users)
  Plans-structure review:
  The plans-structure in Buildsystem/ReleaseProcess/plans/ needs review.
  <ul>
   <li> Should we make a distinction between "special releases" and
   "continuous releases" ? </li>
  </ul>


  \todo "Prerequisites" for further development
  <ul>
   <li> The README-file contains only tools which are needed for building
   and for the basic tasks. </li>
   <li> But there is a lot of other software, which users/developers of the
   OKlibrary will encounter at some point (for example "automake"). </li>
   <li> Somewhere these tools need also to be discussed. </li>
   <li> Perhaps located somewhere in the docus-system. </li>
   <li> Or perhaps in the FAQ-page. </li>
  </ul>


*/

