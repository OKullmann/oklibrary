// Oliver Kullmann, 26.6.2007 (Swansea)

/*!
  \file Buildsystem/plans/PackageBuilding.hpp
  \brief Plans regarding package building


  \todo General
  <ul>
   <li> Checking is separate, has to be done (fully) in advance
   see Buildsystem/plans/CheckBuildSystem.hpp. </li>
   <li> The appropriate "user clone" has to be created in advance
   (see Buildsystem/plans/Release.hpp) --- and also this clone has
   been tested. </li>
   <li> A package exists for every new version number of Transitional. </li>
   <li> Packages need precise names; proposal (for the next release):
   \verbatim
OKlib_0.1.6_31072007
   \endverbatim
   this is, after the name comes the version number, and then the date
   (day) of packaging. But this is non-standard, so better we use
   \verbatim
OKlibrary-0.1.6
   \endverbatim
    <ol>
     <li> Should also the time be included? Likely not (it must not
     matter). </li>
     <li> Should the name be "OKplatform" ? </li>
     <li> Regarding the date formatting, we should stick to this
     European form. </li>
     <li> Parts (without the external libraries etc.) should be suffixed
     like "-minimal". </li>
    </ol
   </li>
   <li> Packages must be stored in an accesible depot. At least the
   core-packages need to be backed-up on cs-oksvr. See
   "ExternalSources repository" in Buildsystem/plans/Release.hpp. </li>
   <li> Package data must be on the Internet-page
   (as automatical as possible).
    <ol>
     <li> md5-checksums </li>
     <li> construction dates and times </li>
    </ol>
   </li>
   <li> Also a ((semi-)automatical) e-mail to the notification list. </li>
   <li> Shall we make a distinction between "major" and "minor" versions? </li>
  </ul>


  \todo The script
  <ul>
   <li> Main steps for the full package:
    <ol>
     <li> mkdir OKplatform etc. </li>
     <li> Copy the user-clone. </li>
     <li> Copy external sources. </li>
     <li> make prebuild </li>
     <li> make html </li>
     <li> Compress. </li>
    </ol>
   </li>
   <li> Variations:
    <ol>
     <li> "minimal" without external sources and html. </li>
     <li> "extern" only contains the external sources. </li>
     <li> "doc" only with the documentation. </li>
    </ol>
   </li>
  </ul>

*/
