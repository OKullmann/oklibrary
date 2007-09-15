// Oliver Kullmann, 26.6.2007 (Swansea)

/*!
  \file Buildsystem/ReleaseProcess/plans/PackageBuilding.hpp
  \brief Plans regarding package building


  \todo CreatePackage extensions
  <ul>
   <li> Perhaps there should be additional targets for the oklib-masterscript
   to "compile and test" everything (to ease building a new environment). </li>
   <li> For a package created, a ((semi-)automatical) e-mail to the notification list
   should be sent. </li>
   <li> Release-packages must be stored in an accessible depot. At least the
   core-packages need to be backed-up on cs-oksvr. See
   "ExternalSources repository" in Buildsystem/ReleaseProcess/plans/Release.hpp. </li>
   <li> Checking is separate, and has to be done (fully) in advance
   see Buildsystem/plans/CheckBuildSystem.hpp. </li>
   <li> The appropriate "user clone" has to be created in advance
   (see Buildsystem/ReleaseProcess/plans/Release.hpp) --- and also this clone has to be
   tested. </li>
  </ul>

*/
