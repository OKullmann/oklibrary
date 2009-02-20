// Oliver Kullmann, 7.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ReleaseProcess/plans/Release.hpp
  \brief Plans regarding releasing the software

  The special aspect of package building is treated in Buildsystem/ReleaseProcess/plans/PackageBuilding.hpp


  \todo Developers
  <ul>
   <li> Different types of developers:
    <ol>
     <li> main developer (OK) </li>
     <li> core developer (full access to main repository) </li>
     <li> local developer (only access to a controlled clone; local to Swansea) </li>
     <li> external developer (only access to a controlled clone) </li>
    </ol>
   </li>
   <li> See "Developers information" in Buildsystem/Configuration/plans/Persons.hpp. </li>
   <li> Different defaults for mailing lists. </li>
   <li> Everybody can submit patches. </li>
   <li> Perhaps everybody except core developers should (always) work with branches ?!
   Submission then to the master-branch. </li>
   <li> Local and external developers each have their own dedicated push+pull clones. Core developers
   connects it with the main repository. </li>
  </ul>


  \todo Release plan
  <ul>
   <li> In Annotations/Release-1_0 we have a release plan for version 1.0
   --- what to do with this plan?
    <ol>
     <li> Move all plans there to the appropriate plans in OKlib. </li>
     <li> Part I on components delived should be integrated into the
     respective milestones. </li>
     <li> Some with Part VII. </li>
     <li> "Supported platforms": We must do something about Linux/Unix,
     while the rest is not supported. </li>
     <li> The relative order in the schedule can be considered when
     integrating the plans. </li>
     <li> Part IV on "Usage" should go to here. </li>
     <li> Compare V, VI with "MailingLists.hpp". </li>
    </ol>
   </li>
   <li> DONE (all plans more here)
   Release plans perhaps should move to here --- or stay in Annotations? </li>
  </ul>


  \todo ExternalSources repository
  <ul>
   <li> In cs-oksvr:/work/Repositories/ExternalSources_recommended we store the (currently) recommended
   versions. We need then (likely under Configuration) a "database" with md5sum-results. See Buildsystem/plans/Configuration.hpp. </li>
  </ul>


  \todo Special tag
  <ul>
   <li> A special Git-tag for a release should be created? </li>
   <li> Or is it identical with a new version of the library? Looks
   better. </li>
   <li> The special tag should be a "full tag", with signature etc. </li>
   <li> But perhaps we postpone the signature for now. </li>
   <li> Name of the tag
   \verbatim
OKlibrary-0.1.6
   \endverbatim
   (compare package-names in Buildsystem/plans/PackageBuilding.hpp). </li>
   <li> Message just for example 'New version ("THEME")'. </li>
   <li> Compare with "Tagging" in Buildsystem/SourceControl/plans/general.hpp. </li>
  </ul>


  \todo Improved releases
  <ul>
   <li> What kind of clones does the user get?
    <ol>
     <li> One master-user-clone is created on cs-oksvr, and the users get clones of it as
     described in "Copied clones which know how to connect" in Buildsystem/plans/VersionControl.hpp. </li>
    </ol>
   </li>
   <li> How user can update:
    <ol>
     <li> The simplest category of user only uses the releases (the packages; see
     "Package construction script" below). </li>
     <li> An "active user" has pull-access to the dedicated user-clone. </li>
     <li> Finally a user can become a "developer" (see "Developers" below). </li>
    </ol>
    Perhaps the dedicated user-clone is only updated "every few days".
   </li>
   <li> How can we create special "views" for the users? They should be able to register
   for modules or subjects, and then get commit-notifications related to those modules.
   Sending them also the patches? Or the new files??
   </li>
  </ul>

*/

