// Oliver Kullmann, 28.6.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/plans/Mhash.hpp
  \brief Plans regarding installation of the Mhash library


  \todo Mhash
  <ul>
   <li> mhash.mak needs a complete review; the usage of make-variables
   seems pretty random (we should synchronise it with external_sources_versions.mak
   and with the other build-files; and we need specifications for the variables,
   at least examples). </li>
   <li> Make a bug report about version 0.9.9 of Mhash (
   see the two files in Kryptologie/Mhash/corrected/mutils --- two
   commas too much). </li>
   <li> We need some user-information for mhash (how to use the installed
   files). </li>
   <li> It seems yet only a local build is available? As discussed above, then
   also the option for a global installation should be available. </li>
   <li> The build-instructions for mhash should be as similar as possible
   to those for Boost. </li>
   <li> Shouldn't the default for building mhash be using the recommended
   local gcc-version? </li>
   <li> It seems that cleanmhash should also remove the installation directory.
   (MH) Isn't it the case that we want cleanmhash only to remove the build
   directory - so that the local installation can still be used after cleaning?
   Then to clean everything there is already the target "cleanallmhash". </li>
  </ul>
   
*/

