// Oliver Kullmann, 7.6.2007 (Swansea)

/*!
  \file Buildsystem/plans/Release.hpp
  \brief Plans regarding package building etc.

  \todo Distributing the library
  <ul>
   <li> A major problem is how to filter out the "secret" parts.
    <ol>
     <li> Hopefully with "git-submodule" we can create sub-repositories
     (see Buildsystem/plans/VersionControl.hpp). </li>
     <li> Likely critical are only the plans-directories. The simplest measure is to
     create a sub-module without the plans-directories. And for special users we add then
     appropriate plans-directories. </li>
    </ol>
   </li>
   <li> Build stages:
   <ol>
    <li> The package is just a clone of the (complete) repository (containing the full
    tree with root "OKplatform"); since we do not want to suppose non-standard
    software to be installed, in directory OKplatform a simple makefile is to found,
    which then can start the build process. Perhaps this makefile is one that is
    created by CMake?! What is the functionality:
    <ol>
     <li> Building all necessary external software. </li>
     <li> Running "prebuild" for the OKlibrary. </li>
    </ol>
    So perhaps there is a configuration step, asking for each external library, whether
    <ol>
     <li> a local build </li>
     <li> a global build </li>
     <li> no build (but using a preinstalled version) </li>
     <li> "don't know" (the build system shall find out) </li>
    </ol>
    is wished, and then all is build. However, it seems, that the configuration step
    is the responsibility of cmake, and not part of the created makefile-functionality?
    </li>
    <li> So perhaps the OKplatform-makefile just creates the cmake-executables, if wished,
    and otherwise the user is to run the cmake-process? Then perhaps the cmake-building
    makefile is to be found in a subdirectory, only invoked if needed. </li>
   </ol>
   </li>
   <li> Download possibilities:
   <ol>
    <li> The whole library (with all included libraries like Boost, doxygen, PostgreSQL etc.). </li>
    <li> Only the minimum. </li>
    <li> In both cases with or without the documentation preinstalled. </li>
    <li> Or perhaps, easier, always with the documentation installed? Currently (17.6.2007)
    the doc-directory is 46 MB, and the .gz-archive is 6.9 MB. </li>
   </ol>
   This gives two user types: "full user" and "minimal user".
   We must make sure, that also the minimal user can use the test and the complexity system
   (compiling it himself, or using the build system). And we must study, how a full user can keep his
   file structure separate from the library by using links. Of course, easier is not to make
   the distinction, but always to supply the full clone (with additionally the documentation
   preinstalled, so that nothing needs to be run or compiled) ?!
   </li>
  </ul>

  \todo Competition extraction tools
   - these tools, for a given program
     build a package, which contains all necessary source code and can be build
     using make (producing then the executable). A problem here are the link-libraries.
     Without them, one could use the ability of g++ to produce compilation units (using
     the option "-E" like "g++ -E Program.cpp -o Program.ii"), putting everything into one big file.
     Knowing the link-libraries, just compiling Program.ii with the right .o-files yields the program.
     So when we know all compilation units, we can preprocess them all and putting them
     into the package; the package just compiles the units separately, and then links them together.

  \todo Package construction script
   - Writing the package-construction script. (Internal versions should
     have date and time in the name.)

*/

