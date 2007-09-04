// Oliver Kullmann, 26.6.2007 (Swansea)

/*!
  \file Buildsystem/ReleaseProcess/plans/PackageBuilding.hpp
  \brief Plans regarding package building


  \todo General
  <ul>
   <li> Checking is separate, has to be done (fully) in advance
   see Buildsystem/plans/CheckBuildSystem.hpp. </li>
   <li> The appropriate "user clone" has to be created in advance
   (see Buildsystem/ReleaseProcess/plans/Release.hpp) --- and also this clone has
   been tested. </li>
   <li> Packages must be stored in an accessible depot. At least the
   core-packages need to be backed-up on cs-oksvr. See
   "ExternalSources repository" in Buildsystem/ReleaseProcess/plans/Release.hpp. </li>
   <li> Package data must be on the Internet-page
   (as automatical as possible).
    <ol>
     <li> md5-checksums </li>
     <li> construction dates and times </li>
    </ol>
   </li>
   <li> Also a ((semi-)automatical) e-mail to the notification list. </li>
   <li> DONE (basic ideas are clear now; text needs to be transferred to full
   documentation)
   Packages are called like
   \verbatim
OKlibrary-0.2.0.8
   \endverbatim
   where "0.2.0" is the main version number and ".8" the current "stage"
   (the whole string "0.2.0.8" is given by make-variable "transitional_version").
    <ol>
     <li> So a new package is only created when at least the current stage
     is advanced; users who wish to by fully update use Git. </li>
     <li> Parts (without the external libraries etc.) should be suffixed
     like "-minimal". </li>
     <li> Regarding the date formatting, we should stick to this
     European form. DONE (no dates in the name; otherwise we use for
     example "04.09.2007") </li>
     <li> Should the name be "OKplatform" ? DONE (no, yet no use
     for OKplatform) </li>
     <li> DONE (no time included; this is available via the history)
     Should also the time be included? Likely not (it must not
     matter). </li>
    </ol>
   </li>
   <li> DONE (explained in more details in the next item)
   A package exists for every new version number of Transitional. </li>
   <li> DONE (question answered; to be entered into the full documentation)
   Shall we make a distinction between "major" and "minor" versions? Yes,
   in the documentation and announcements we make a differences between the
   four types of advances (related to the four digits): Advancing to
   "1.0.0.0" should be a "big thing", advancing to "0.3.0.0" still gets
   some publicity, advancing to "0.2.1.0" is announced on internal mailing
   lists, still with explanations what did change, while finally advancing
   to "0.2.0.4" only creates automated messages. </li>
   <li> DONE (no, we use standard package names, not such home-cooked)
   Packages need precise names; proposal (for the next release):
   \verbatim
OKlib_0.1.6_31072007
   \endverbatim
   this is, after the name comes the version number, and then the date
   (day) of packaging. </li>
   </ul>


  \todo The script
  <ul>
   <li> Synchronise the following with "Distributing the library" in
   Buildsystem/ReleaseProcess/plans/Release.hpp. </li>
   <li> Name of this Bash script: "CreatePackage". </li>
   <li> Main steps for the full package:
    <ol>
     <li> mkdir OKplatform etc. </li>
     <li> Copy the user-clone. </li>
     <li> Copy external sources. </li>
     <li> Copy documents. </li>
     <li> Run Buildsystem/Makefile (for setup). </li>
     <li> oklib --prebuild </li>
     <li> oklib html </li>
     <li> Compress. </li>
    </ol>
   </li>
   <li> For this to work we need to make the internal url's relative.
   See "Install configuration system" in Buildsystem/Html/plans/general.hpp. </li>
   <li> So a complete package comes with documentation, and for the rest
   the standard build system can be used (no need for additional makefiles).
   However, there should be additional targets for the oklib-masterscript
   to "compile and test" everything. </li>
   <li> Variations:
    <ol>
     <li> "minimal" without external sources and html. </li>
     <li> "extern" only contains the external sources. </li>
     <li> "doc" only with the documentation. </li>
    </ol>
   </li>
  </ul>

*/
