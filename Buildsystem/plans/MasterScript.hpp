// Oliver Kullmann, 13.7.2007 (Swansea)

/*!
  \file Buildsystem/plans/MasterScript.hpp
  \brief Plans for the masterscript, the "user-interface" to the build system

  Similar to the use of "git" in a Git-repository, we use "oklib"
  in an OKplatform-installation.


  \todo Design
  <ul>
   <li> The make-system has to be redesigned:
    <ol>
     <li> Five main makefiles:
      <ol>
       <li> Buildsystem/ExternalSources/Makefile </li>
       <li> Buildsystem/OKlibBuilding/Makefile (the current Buildsystem/generic.mak) </li>
       <li> Buildsystem/Html/Makefile (takes over target html from current Buildsystem/generic.mak) </li>
       <li> Buildsystem/Latex/Makefile (the current Buildsystem/Annotations.mak) </li>
       <li> Buildsystem/Makefile (the current Buildsystem/makefile). </li>
      </ol>
     </li>
     <li> The master-makefile "Master.mak" includes the configuration makefile and the
     aboive main makefiles. </li>
     <li> So the target-sets for the main makefiles must be disjoint! For common targets like
     "all" we could introduce a scoping:
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
     adding appropriate definitions, and potentially disambiguate targets (according to
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
   <li>
   <ol>
    <li> Find the first .oklib-directory on the path upwards. </li>
    <li> Call make in the current directory, with OKplatform defined
    and all other arguments forwarded. </li>
    <li> The output of make is copied to
    system_directories/log/GenericOutput.txt (this filename is a
    configuration-variable) resp. to ExternalSourcesOutput.txt. </li>
    <li> I (OK) don't think that one needs to distinguish between
    the generic makefile and the various recursive makefiles --- this
    distinction will likely vanish anyway. </li>
   </ol>
   Once oklib is available, the temporary definitions of OKplatform etc.
   in the makefiles shall be removed.
   <li>
  </ul>


  \todo Implementation
  <ul>
   <li> A transition plan might be useful (stepwise implementation). </li>
   <li> The actions performed by masterscript oklib:
    <ol>
     <li> Find the first .oklib-directory on the path upwards. </li>
     <li> Call the master-makefile, with OKplatform defined
     and all other arguments forwarded. </li>
     <li> See "Design" above for potential disambiguation of targets. </li>
     <li> The output is copied to system_directories/log/Makefiles/OKlibBuilding.txt
     resp. to ExternalSources.txt etc. (these filenames are configuration-variables).  </li>
    </ol>
   </li>
   <li> Once oklib is available, the temporary definitions of OKplatform etc.
   in the makefiles shall be removed. </li>
  </ul>


  \todo Further enhancements:
  <ul>
   <li> --help </li>
  </ul>

*/

