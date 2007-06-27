// Oliver Kullmann, 28.2.2006 (Swansea)

/*!
  \file Buildsystem/ExternalSources/plans/general.hpp
  \brief Plans for the makefile responsible for handling external sources


  \todo Renaming
  <ul>
   <li> Perhaps we better call now "Buildsystem/ExternalSources/ExternalSources.mak"
   simply "Buildsystem/ExternalSources/makefile". </li>
   <li> Perhaps we should move "Buildsystem/external_sources_versions.mak"
   to directory Buildsystem/ExternalSources, and then call it
   simply "versions.mak". </li>
  </ul>


  \todo Plans-structure for ExternalSources
  <ul>
   <li> Likely we should now have also a milestones.hpp file. </li>
   <li> Perhaps the version number of sub-module ExternalSources
   is initially set to the current version number of Buildsystem? </li>
  </ul>


  \todo Mailman
  <ul>
   <li> Building mailman on cs-wsok (for testing):
    <ol>
     <li> Building the main program </li>
     <li> What about the web server: Is Apache the best choice for us?
     (Perhaps we need the web server anyway for the OKplatform-Internet
     page?) </li>
    </ol>
   </li>
  </ul>


  \todo CMake-documentation building
  <ul>
   <li> Make also the documentation available. </li>
  </ul>


  \todo Improving building Boost
  <ul>
   <li> Like with gcc, the build-directory and the bjam-directory should not be a
   prerequisite (so that an unnecessary "make boost" is a noop). </li>
   <li> It should be possible to say "gcc-version=recommended". </li>
   <li> Instead of "boost boost_recommended=boost-1_33_1" we should use
   "boost boost-version=1_33_1". </li>
   <li> The default for generic.mak is to use the recommended <strong>local</strong>
   gcc-installation --- shouldn't this then be also for building %boost
   the default ? </li>
   <li> Installation of bjam should be improved: Having exactly one
   bjam-installation for each boost-version, and no need to recreate
   it if it's already there. Or, perhaps better: We just leave it in
   the distribution directory --- this seems reasonable: We don't use
   bjam ourselves, and thus by removing the additional steps
   our systems becomes simpler! </li>
   <li> What happens with copying the documentation- files and directories
   if the target- files and/or directories are already there? DONE ("cp -r"
   copies directories/files which are not already present, and actually
   replaces them if they are already there (in any case!)) </li>
   <li> Building %boost should include copying the documentation to doc
   (in the subdirectory boost-1_33_1 for example). DONE </li>
   <li> In the long run, it seems that actually supporting different versions
   of Boost is not feasible (the library will likely always use the newest
   version), so finally supporting different Boost version should be dropped
   (but the general machinery is worth keeping)?!? DONE Very likely most of the times we will support only one boost-version --- but we need the machinery for the transition to newer version (then for some times 2 versions are around)! And we might try out beta-versions etc. So we need the boost-build-machinery. </li>
  </ul>


  \todo Problems with building Boost (1_34_0)
  <ul>
   <li> Report to Boost: How to call the libraries is not documented, %e.g.,
   the only library-name mentioned is "Boost.Python", while its real name
   is "python". (Important for "--without-libraries=python".)
   </li>
   <li> On cs-ltok (32 bit) we get build-log-messages like
   \verbatim
`.L1119' referenced in section `.rodata' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_iarchive.o: defined in discarded section `.gnu.linkonce.t._ZNK5boost7archive17archive_exception4whatEv' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_iarchive.o
`.L573' referenced in section `.rodata' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_oarchive.o: defined in discarded section `.gnu.linkonce.t._ZNK5boost7archive17archive_exception4whatEv' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_oarchive.o
   \endverbatim
   Does this indicate something we should worry about ?
   </li>
   <li> On cs-wsok (64 bit) we get
   \verbatim
ExternalSources> make boost-1_34_0

...failed updating 8 targets...
...skipped 24 targets...
...updated 5517 targets...
   \endverbatim
   Rerunning reveals
   \verbatim
...failed gcc.link.dll /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform/ExternalSources/Boost/1_34_0+4.1.2_Build/boost/bin.v2/libs/wave/build/gcc-4.1.2/debug/threading-multi/libboost_wave-gcc41-mt-d-1_34.so.1.34.0...
gcc.link.dll /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform/ExternalSources/Boost/1_34_0+4.1.2_Build/boost/bin.v2/libs/graph/build/gcc-4.1.2/debug/threading-multi/libboost_graph-gcc41-mt-d-1_34.so.1.34.0
collect2: ld terminated with signal 11 [Segmentation fault]
   \endverbatim
   So there are linking problems regarding the wave- and the graph-library.
   This seems not to be of urgent concern for now (but the problem must be
   fixed in the future).
   </li>
   <li> How to inform bjam about an alternative compiler? What about
   <code> "-sGCC_ROOT_DIRECTORY=$(gcc-base-directory)/$(2)" </code> ??
   </li>
   <li> When building gcc with local versions, we should make sure that
   the system-gcc doesn't interfere (especially important regarding linking).
    <ol>
     <li> On cs-wsok it seems that the system-gcc (4.0.2) interferes;
     perhaps it tries to link with the 32bit-version, can't do that,
     and then falls back to the system version? </li>
     <li> What is the role of LD_LIBRARY_PATH ?? (On cs-wsok it is empty.) </li>
     <li> We should check in general whether building %boost links to the 32bit
     or to the 64bit version. </li>
    </ol>
   </li>
  </ul>


  \todo Building Ubcsat (OK, ML):
  Review of Ubcsat.mak review
  <ul>
   <li> Remove the hardcoded version-numbers. </li>
   <li> What is the meaning of "tag" and "tag_" ??? The names should tell. </li>
   <li> No file modification in the external sources. DONE </li>
  </ul>


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


  \todo Building documentation:
   - Documentation building should be done automatically when building.
   - We should extract only the documentation from the archives (and
   remove everything else which is not needed).
   

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

  
  \todo Gcc
  <ul>
   <li> Linking to the gcc-documentation (main info-page) should be checked
   for completeness. </li>
   <li> Instead of, %e.g., "make gcc-4.1.2", wouldn't it be more consistent with
   building Boost to use instead "make gcc gcc-version=4.1.2" ? </li>
   <li> If variable "gcc-version" is set, then it should have one of the
   allowed values (while otherwise we get an error). </li>
   <li> We must understand, how gcc interacts with 32- and 64-bit
   environments, and how to take control of this. </li>
   <li> There should be make-variables, which allow control over
   some settings for the build of gcc. </li>
   <li> Shouldn't file external_sources_versions.mak be placed in
   subdirectory Buildsystem/ExternalSources ? </li>
   <li> texti2pdf is a script, which can be put into ~/bin for example,
   and is needed for building the gcc-documentation --- how to provide it?
   Such small utilities could be put under version control (OKlibrary) ? </li>
   <li> If the filestamp does already exist, then we want nothing to happen ---
   however yet the build-directory will be rebuilt if not existent, since
   it is a prerequisite of the rule for the "tag-paths". So it seems
   necessary to remove the build-directory from the prerequisite-list,
   however then it seems impossible to create the build-directory, if actually
   gcc *is* to be build, via the target-mechanism. DONE (it seems reasonable
   to remove the build-directory from the prerequisite-list and to build it
   "manually") </li>
  </ul>
  
  \todo PostgreSQL
  - Test the updated PostgreSQL installation.
  - "make initialise-database" should work with the recommended version (and no specification
  of the pgsql-version should be needed).
  - A detailed user-manual is needed (on how to use the installed database).

  \todo R
  - Installation of R appears to require a Fortran compiler. Therefore the 
    locally installed Gcc should have Fortran language support enabled.
  - Enabling Fortran language support in Gcc requires also that the MPFR
    and GMP libraries are installed, so we should install these locally also?
  
  \todo Other sources:
  - Build a local version of Xerces.
  - Build a local version of gmp.
  - Build the R system (locally and globally).
  - Build and incorporate graphviz (the dot-tool is needed by doxygen for creating the graphs)
  and tulip.
  - SOCI (soci.sourceforge.net) ?
  - gcc-xml (www.gccxml.org) ?
  
  \todo %Tools
  - Investigate the NiX system (a system for managing libraries).
  - Investigate the Poco C++ library (http://www.pocoproject.org; see
    also [C Vu, 19:2, pages 12-15]).
     
*/
