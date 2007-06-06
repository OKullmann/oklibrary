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
     <li> Next then is to build using gcc version 4.2.0: Seems to work
     (but not used yet, since first the template-related (new)
     errors have to be repaired). </li>
    </ol>
   </li>
   <li> Report for cs-wsok:
    <ol>
     <li> Installation failed (using the system-gcc 4.0.2):
     \verbatim
ExternalSources> make boost-1_34_0

...failed updating 8 targets...
...skipped 24 targets...
...updated 5517 targets...
     \endverbatim
     First check whether this also occurs with the manual installation, then find out
     which libraries failed. Is the failure the same source as the bug about building boost below?
     </li>
     <li> Manual installations seems to yield the same result. </li>
     <li> Installation with local gcc "make boost gcc-version=4.1.2" also fails:
     \verbatim
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
     So there are linking problems regarding the wave- and the graph-library. This seems not to be
     of urgent concern for now (but the problem must be fixed).
     </li>
     <li> Building %boost with "make boost gcc-version=4.2.0" yields the same result. </li>
    </ol>
   </li>
   <li> How to inform bjam about an alternative compiler? What about
   <code> "-sGCC_ROOT_DIRECTORY=$(gcc-base-directory)/$(2)" </code> ?? </li>
   <li> Correct documentation building. DONE </li>
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
   or to the 64bit version. See the build-problems on cs-wsok described in
   the bug above.
   </li>
  </ul>


  \bug Building gcc_doc : DONE

  <ul>
   <li> On cs-wsok we get the following build-error:
    \verbatim
~/SAT-Algorithmen/OKplatform/ExternalSources> make gcc-4.2.0
...
make[2]: Entering directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Gcc/gcc-4.2.0_Build'
Doing pdf in gcc
make[3]: Entering directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Gcc/gcc-4.2.0_Build/gcc'
texi2pdf -I . -I /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Gcc/gcc-4.2.0_Build/gcc/../../gcc-4.2.0/gcc/doc -I /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Gcc/gcc-4.2.0_Build/gcc/../../gcc-4.2.0/gcc/doc/include -o doc/gcc.pdf ../../gcc-4.2.0/gcc/doc/gcc.texi
make[3]: texi2pdf: Command not found
make[3]: *** [doc/gcc.pdf] Error 127
make[3]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Gcc/gcc-4.2.0_Build/gcc'
make[2]: *** [pdf-gcc] Error 1
make[2]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Gcc/gcc-4.2.0_Build'
make[1]: *** [do-pdf] Error 2
make[1]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Gcc/gcc-4.2.0_Build'
make: *** [/h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform//ExternalSources/Gcc/_gcc-4.2.0] Error 1
    \endverbatim
    ??? Where to get texi2pdf ? DONE (now in ExternalSources --- just a script, which can be put into ~/bin for
    example; should the installation do something about it???) </li>
   <li> What do do if the subdirectories of doc/Gcc already exist? DONE
    (nothing done if the directories exist) </li>
    <li> When it comes to renaming /share/doc to html, and the target
    directory html already exists, then instead /share/doc is moved *into*
    html --- how to avoid this?? DONE (first rename, then move) </li>
   <li> Apparently the build does ONLY create sub-directories
    <ol>
     <li> info with a few text-files </li>
     <li> man with a few files not containing much </li>
    </ol>
    ??? So the current build is incomplete. But what is to be added
    to the build??? The only installation information provided with
    the gcc-distribution apparently is in gcc-4.2.0/gcc/doc/gccinstall.info,
    and there is no information on building documentation???
    How to read gcc-4.2.0/gcc/doc/gccinstall.info??? (It's just an ugly
    text-file???) DONE (it seems that now all possible documentation is build) </li>
   <li> Target gcc_doc does not work (the link at
   system_directories/doc/index.html is not working). DONE (still the
   linking on the index page is to be performed). </li>
   <li> *Only* the documentation is to be extracted, not the whole
   distribution. DONE </li>
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

  \todo CMake
  <ul>
   <li> System-wide installation DONE
   \verbatim
CMake> tar -xzf ../cmake-2.4.6.tar.gz
CMake> cd cmake-2.4.6
CMake/cmake-2.4.6> ./bootstrap
CMake/cmake-2.4.6> make
# make install
   \endverbatim
   The relevant documentation-directories are "Docs" and "Example".
   </li>
  </ul>

  \todo Boost
  <ul>
   <li> Like with gcc, the build-directory and the bjam-directory should not be a
   prerequisite (so that an unnecessary "make boost" is a noop). </li>
   <li> It should be possible to say "gcc-version=recommended". </li>
   <li> Instead of "boost boost_recommended=boost-1_33_1" we should use
   "boost boost-version=1_33_1". </li>
   <li> The default for generic.mak is to build the recommended <strong>local</strong>
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
  
  \todo %Tools
  - Investigate the NiX system (a system for managing libraries).
  - Investigate the Poco C++ library (http://www.pocoproject.org; see
    also [C Vu, 19:2, pages 12-15]).
     
*/
