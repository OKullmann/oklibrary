// Oliver Kullmann, 28.2.2006 (Swansea)

/*!
  \file Buildsystem/plans/ExternalSources.hpp
  \brief Plans for the makefile responsible for handling external sources

  \bug Building Boost 1_34_0
  <ul>
   <li> Report for csltok:
    <ol>
     <li> First try with manual build and simply
     \verbatim
boost_1_34_0> ./configure --prefix=${OKPLATFORM}/ExternalSources/Boost/1_34_0
make install
     \endverbatim
     (system installation with gcc 3.4.3) yields
     \verbatim
...failed updating 216 targets...
...skipped 32 targets...
...updated 5567 targets...
Not all Boost libraries built properly.
     \endverbatim
     First guess is, that these failures are phython-related, so we should
     actively disable phython. Try:
     \verbatim
boost_1_34_0> ./configure --prefix=${OKPLATFORM}/ExternalSources/Boost/1_34_0 --without-libraries=python
make install
     \endverbatim
     (Report to Boost: How to call the libraries is not documented (the only
     library-name mentioned is "Boost.Python").)
     </li>
     <li> Not only python seems problematic:
     \verbatim
`.L1119' referenced in section `.rodata' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_iarchive.o: defined in discarded section `.gnu.linkonce.t._ZNK5boost7archive17archive_exception4whatEv' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_iarchive.o
`.L573' referenced in section `.rodata' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_oarchive.o: defined in discarded section `.gnu.linkonce.t._ZNK5boost7archive17archive_exception4whatEv' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_oarchive.o
     \endverbatim
     ???
     But it seemed to work:
     \verbatim
...updated 5548 targets...
     \endverbatim
     </li>
     <li> Next then is to build using gcc version 4.2.0. </li>
    </ol>
   </li>
   <li> Report for cs-wsok:
    <ol>
     <li> Installation failed:
     \verbatim
ExternalSources> make boost-1_34_0

...failed updating 8 targets...
...skipped 24 targets...
...updated 5517 targets...
     \endverbatim
     First check whether this also occurs with the manual installation, then find out
     which libraries failed. Is the failure the same source as the bug about building boost below? </li>
    </ol>
   </li>
   <li> How to inform bjam about an alternative compiler? What about
   <code> "-sGCC_ROOT_DIRECTORY=$(gcc-base-directory)/$(2)" </code> ?? </li>
   <li> Correct documentation building. </li>
   <li> Additionally to the result of "make install" we need also to move
   the src-directory to, e.g., Boost/1_34_0, and we need to move the doc. </li>
   <li> We should read the installation documentation. DONE (unfortunately, there is not much in it) </li>
   <li> The path to the bjam-sources now is boost_1_34_0/tools/jam/src. DONE </li>
  </ul>

  \bug Building-Boost Errors
  
  <ul>
   <li>
   Suddenly building boost on cs-wsok with a local gcc (version 3.4.3 or
   3.4.6 for example) doesn't work anymore, but there are linking errors.
   It seems that on csltok everything works, so that it could be a
   64bit thing.
  
   What did change?? And it seems that gcc versions for example 4.1.1
   do work?? Since versions below 4.0.0 don't work, while above work,
   it seems that the system-gcc (4.0.2) interferes; perhaps it tries to
   link with the 32bit-version, can't do that, and then falls back to the
   system version?
  
   What is the role of LD_LIBRARY_PATH ?? (On cs-wsok it is empty.)
  
   OK (12.1.2007): When building boost with gcc-version 3.4.3 or 3.4.6, we get
   \verbatim
   ...failed updating 10 targets...
   ...skipped 14 targets...
   \endverbatim
   Why this?
  
   We should check in general whether building boost links to the 32bit
   or to the 64bit version.
   </li>
  </ul>
  
  \bug Building gcc_doc

  <ul>
   <li> Does not work (that is, the link at
   system_directories/doc/index.html is not working),
   since apparently the man-pages need to be build. </li>
   <li> *Only* the documentation is to be extracted, not the whole
   distribution. </li>
  </ul>

  \todo Boost
  <ul>
   <li> It should be possible to say "gcc-version=recommended". </li>
   <li> Instead of "boost boost_recommended=boost-1_33_1" we should use
   "boost boost-version=1_33_1". </li>
   <li> The default for generic.mak is to build the recommended <strong>local</strong>
   gcc-installation --- shouldn't this then be also for building %boost
   the default ? </li>
   <li> Installation of bjam should be improved: Having exactly one
   bjam-installation for each boost-version, and no need to recreate
   it if it's already there. Or, perhaps better: We just leave it in
   the distribution directory? </li>
   <li> Building %boost should include copying the documentation to doc
   (in the subdirectory boost-1_33_1 for example). </li>
   <li> In the long run, it seems that actually supporting different versions
   of Boost is not feasible (the library will likely always use the newest
   version), so finally supporting different Boost version should be dropped
   (but the general machinery is worth keeping)?!? </li>
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
   installation could also be make global. </li>
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
  </ul>
  
  \todo Gcc
  <ul>
   <li> Instead of, e.g., "make gcc-4.1.2", wouldn't it be more consistent with
   building Boost to use instead "make gcc gcc-version=4.1.2" ? </li>
   <li> If variable "gcc-version" is set, then it should have one of the
   allowed values (while otherwise we get an error). </li>
   <li> We must understand, how gcc interacts with 32- and 64-bit
   environments, and how to take control of this. </li>
   <li> There should be make-variables, which allow control over
   some settings for the build of gcc. </li>
   <li> Shouldn't file external_sources_versions.mak be placed in
   subdirectory Buildsystem/ExternalSources ? </li>
  </ul>
  
  \todo PostgreSQL
  - Update PostgreSQL to version 8.1 (or later; and test it).
  - "make initialise-database" should work with the recommended version (and no specification
  of the pgsql-version should be needed).
  - A detailed user-manual is needed (on how to use the installed database).
  
  \todo Other sources:
  - Build a local version of Xerces.
  - Build a local version of gmp.
  - Build the R system (locally and globally).
  - Build and incorporate graphviz (the dot-tool is needed by doxygen for creating the graphs)
  and tulip.
  - SOCI (soci.sourceforge.net) ?
  - gcc-xml (www.gccxml.org) ?
  
  \todo Tools
  - Investigate the NiX system (a system for managing libraries).
  - Investigate the Poco C++ library (http://www.pocoproject.org; see
    also [C Vu, 19:2, pages 12-15]).
     
*/
