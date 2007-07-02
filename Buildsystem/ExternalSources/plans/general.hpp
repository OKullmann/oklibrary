// Oliver Kullmann, 28.2.2006 (Swansea)

/*!
  \file Buildsystem/ExternalSources/plans/general.hpp
  \brief Plans for the makefile responsible for handling external sources


  \todo Documenting ExternalSources.mak
  <ul>
   <li> docus/ExternalSources.hpp is for the *user* documentation. So the
   internals of the buildsystem should only be discussed in there as far
   as they are relevant for using (and extending) the buildsystem for
   external sources, and this information is not part of the "General
   overview", but of an appendix explaining the extension process. </li>
   <li> The old OKBuildsystem/ExternalSources/doc.mak is gone?
   So documentation building should happen automatically (when building
   the package). </li>
   <li> Document packages:
    <ul>
     <li> Mhash (remark about corrected files; content of package) </li>
     <li> Postgresql </li>
     <li> UBCSAT : DONE </li>
    </ul>
   </li>
   <li> Move the documentation from ExternalSources.mak to
   Buildsystem/docus/ExternalSources.hpp. DONE </li>
  </ul>


  \todo Downloading sources
  <ul>
   <li> Via "make getsources" the system should downloaded all the packages from
   \verbatim
cs-oksvr.swan.ac.uk:/work/Repositories/ExternalSources_recommended
   \endverbatim
  </li>
  </ul>


  \todo Renaming
  <ul>
   <li> Perhaps we better call now "Buildsystem/ExternalSources/ExternalSources.mak"
   simply "Buildsystem/ExternalSources/makefile". </li>
   <li> Perhaps we should move "Buildsystem/external_sources_versions.mak"
   to directory Buildsystem/ExternalSources, and then call it
   simply "versions.mak". </li>
  </ul>


  \todo Building documentation
  <ul>
   <li> Documentation building should be done automatically when building. </li>
   <li> We should extract only the documentation from the archives (and
   remove everything else which is not needed). </li>
  </ul>
   

  \todo General
  <ul>
   <li> It would ge good, if after doing a local installation, easily the
   installation could also be made global. </li>
   <li> Optionally there should be also local versions of valgrind and
   doxygen (and other tools). This is necessary on systems where the user
   does not have root access. </li>
   <li> For "make all" we have the following problems:
   <ul>
    <li> The user cannot wait for "sudo". </li>
    <li> Perhaps for example asciidoc is already installed, and this
    suffices? </li>
   </ul>
   </li>
   <li> Regarding documentation building: Currently boost.mak overrides old
   (existing) documentation, while gcc.mak leaves it intact --- should we
   have a general policy about it? Yet the plan is to make experiences and
   see what is better.
    <ul>
     <li> It seems that "always replace", exploiting this behaviour of
     the cp-command, seems more standard. </li>
     <li> One point in favour for the different treatment in gcc.mak is,
     that there  we want to eliminate the doc-subdirectory of the installation
     directory, and then "mv" seems safer than the sequence "cp -r rm -r"
     (what if some parameters are not set right, and rm goes wild?);
     perhaps this is not very strong. </li>
    </ul>
   </li>
  </ul>

  
  \todo Other sources:
  <ul>
   <li> Build a local version of Xerces. </li>
   <li> Build and incorporate graphviz (the dot-tool is needed by doxygen for creating the graphs)
   and tulip. </li>
   <li> SOCI (soci.sourceforge.net) ? </li>
   <li> gcc-xml (www.gccxml.org) ? </li>
  </ul>


  \todo %Tools
  <ul>
   <li> Investigate the NiX system (a system for managing libraries). </li>
   <li> Investigate the Poco C++ library (http://www.pocoproject.org; see
   also [C Vu, 19:2, pages 12-15]). </li>
  </ul>


  \todo Plans-structure for ExternalSources : DONE
  <ul>
   <li> Perhaps the version number of sub-module ExternalSources
   is initially set to the current version number of Buildsystem? DONE (used a lower
   number to indicate, that the ExternalSources-part of the buildsystem is less
   refined) </li>
   <li> Likely we should now have also a milestones.hpp file. DONE </li>
  </ul>

*/
