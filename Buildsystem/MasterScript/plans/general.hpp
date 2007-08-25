// Oliver Kullmann, 13.7.2007 (Swansea)

/*!
  \file Buildsystem/MasterScript/plans/general.hpp
  \brief Plans for the masterscript, the "user-interface" to the build system

  Similar to the use of "git" in a Git-repository, we use "oklib"
  in an OKplatform-installation.


  \todo First implementation
  <ul>
   <li> The output is copied to system_directories/log/Makefiles/OKlibBuilding.txt
   resp. to ExternalSources.txt etc.; this is achieved by putting symbolic
   links into directory .oklib. </li>
   <li> It should be possible to explicitely specify the calling place as well
   as explicitely stating the makefile to be used. </li>
   <li> When calling the respective makefile, variables OKplatform etc.
   are appropriately defined. </li>
   <li> First we do not change the old makefiles, but we let oklib
   just replace the old links. DONE </li>
   <li> The necessary links to makefiles are all directly constructed from
   .oklib/Configuration (without access to the configuration-make-variables). DONE </li>
   <li> The choice of makefiles (for the delegation of the targets) is
   hard-coded into oklib, based on the first action of oklib, finding
   the first .oklib-directory on the path upwards, and then situating the
   calling directory. DONE </li>
   <li> Perhaps also special actions are provided for Buildsystem/Makefile
   and Buildsystem/Setup.mak ??
   See Buildsystem/plans/Makefile.hpp and Buildsystem/plans/SetUp.hpp.
   DONE ("--setup" and "--prebuild") </li>
   <li> We need some special options for oklib itself (version at least),
   for which we need a special syntax, perhaps for example "--oklib-version"
   etc. DONE (just "--version") </li>
   <li> Every other option is (in the order given) passed onto the makefile,
   which is selected according to the calling-place. DONE </li>
   <li> DONE Perhaps we create a new sub-directory "Buildsystem/MasterScript". </li>
  </ul>


  \todo Further steps
  <ul>
   <li> Once oklib is available, the temporary definitions of OKplatform etc.
   in the makefiles shall be removed. </li>
   <li> Then one after another the old makefiles are updated, but for now
   the old design is basically still kept. </li>
   <li> Yet, all symbolic links to the generic makefile and the recursive makefile
   are still needed, since otherwise the recursive makefile doesn't enter a directory
   resp. the settings of variable srcdir do not work. It should be possible to
   get rid off the symbolic links to the generic makefile by letting the recursive
   makefile enter a directory if it contains definitions.mak. </li>
  </ul>


  \todo Further design
  <ul>
   <li> The main open question is what to do at places where currently the recursive
   makefile is called (or no makefile at all). For oklibrary-building, the new
   oklib-building-makefile should just compile "overthing below". But there are places
   like OKsystem currently, where "all" invokes recursively two different types
   of makefiles; in the future, we might want that "make all" at OKplatform
   level really compiles everything. Perhaps in such special cases, for such
   "general targets", called at special places, oklib calls all relevant
   makefiles (in some predefined order). See below. </li>
   <li> Another question is whether to allow for example "make boost" at every place.
   See below. </li>
   <li> The make-system has to be redesigned:
    <ol>
     <li> Five (partially new) main makefiles:
      <ol>
       <li> Buildsystem/ExternalSources/Makefile </li>
       <li> Buildsystem/OKlibBuilding/Makefile </li>
       <li> Buildsystem/Html/Makefile (takes over target html from current Buildsystem/OKlibBuilding/Makefile) </li>
       <li> Buildsystem/Latex/Makefile (the current Buildsystem/Annotations.mak) </li>
       <li> Buildsystem/Makefile (the current Buildsystem/makefile). </li>
      </ol>
     </li>
     <li> Perhaps a new master-makefile "Master.mak" includes the configuration makefile and the
     above main makefiles? </li>
     <li> So the target-sets for the main makefiles then should be disjoint?
     For common targets like "all" we could introduce a scoping:
      <ol>
       <li> "esc" for ExternalSources </li>
       <li> "htm" for Html </li>
       <li> "lat" for Latex </li>
       <li> "lib" for OKlibBuilding </li>
      </ol>
      And there must be new "super"-targets like "all = htm::all + lat::all + lib::all".
      Perhaps then also the non-ambiguous targets are additionally provided in their
      qualified form.
     </li>
     <li> The masterscript oklib basically hands down all targets to the master-makefile,
     adding appropriate definitions, and potentially disambiguating targets (according to
     some convenience-rules). </li>
     <li> So the masterscript oklib can be called from any place with an okplatform-installation;
     seems reasonable that from any place "oklib boost" can be called. However for oklib-building
     certain directories might be inhibited (currently this happens by *not* providing a link
     to the generic makefile --- perhaps now we react to the special definitions not
     available). </li>
     <li> The functionality of the OKlibBuilding-makefile depends on the calling-place ("handle
     everything below"), while for example the functionality of the Html-makefile does not.
     But normally the masterscript oklib should not be concerned about this: It just always
     calls the master-makefile, providing all available information. </li>
     <li> One problem has to be solved: At OKsystem-level we have (and users might place)
     another repository besides the Transitional-repository; now the oklib-masterscript should
     also serve this (in principle unknown) module. Perhaps under Configuration we arrange
     for such "external" targets. </li>
    </ol>
   </li>
  </ul>


  \todo Further enhancements:
  <ul>
   <li> --help </li>
  </ul>

*/

