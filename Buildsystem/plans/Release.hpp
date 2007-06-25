// Oliver Kullmann, 7.6.2007 (Swansea)

/*!
  \file Buildsystem/plans/Release.hpp
  \brief Plans regarding package building etc.

  \todo Fundamental "axioms"
  <ul>
   <li> There must be fundamental rules and "axioms" about the library
   and how to participate. </li>
   <li> Important the research character. </li>
   <li> SAT is considered at a very general level.
    <ol>
     <li> Open for CSP (while maintaining the spirit of "generalised SAT", with emphasise
     on algorithms, not on language). </li>
     <li> Open for extensions like QBF. </li>
     <li> Any applications included. </li>
     <li> Though not "core business", for example graph and hypergraph algorithms and libraries
     are important, and are also part of the library. </li>
     <li> While hypergraphs (which are just positive clause-sets) are very close to SAT, also
     areas like algebra and statistics are important for us, so the library supports developments
     also in these areas, but here much more emphasise is put on using external libraries. </li>
     <li> Developing modern C++ is another area which is important in general, so supported
     by the library, but again emphasise on using external libraries (like Boost) as much as
     possible. </li>
     <li> "Purely" theoretical algorithms are welcome. </li>
     <li> All kind of "high flying" ideas are welcome --- however one of the main goals of the
     OKlibrary is the unity of SAT (and science in general), which concretely means that
     an outstanding goal is that all components work together through abstraction (by generic
     and generative programming). </li>
     <li> Nearly everything can be encoded into SAT, and thus belongs potentially to the library. </li>
    </ol>
   </li>
   <li> No "C code", but *modern C++ code*. </li>
   <li> No other programming language (but many other systems like Coq or Sage). </li>
   <li> The master repository (with the names "OKlibrary", "OKplatform", "OKsystem")
   are owned by OK. Everybody can use a clone for his own development, thus naturally
   bound by the GPL licence, so the history must be open, but the name of the system
   must be changed. </li>
   <li> Version numbers (of the library at all and of modules) are important for communicating
   the current standing, and their transition is solely the resonsibility of OK. </li>
   <li> Code which is compilable must be accompanied by (generic) tests! </li>
  </ul>


  \todo Distributing the library
  <ul>
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
   <li> What kind of distribution-package the user get?
   <ol>
    <li> They simply get a clone of the whole library (supposing that we have merged the different
    repositories plus the new ExternalSources-repository into one repository), with doc-directory
    preinstalled (so that they don't need to run anything --- just unpack and see). </li>
    <li> One master-user-clone is created on cs-oksvr, and the users get clones of it as
    described in "Copied clones which know how to connect" in Buildsystem/plans/VersionControl.hpp. </li>
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
   <li> Updates:
   How user can update:
   <ol>
    <li> The simplest category of user only uses the releases (the packages; see
    "Package construction script" below). </li>
    <li> An "active user" has pull-access to the dedicated user-clone. </li>
    <li> Finally a user can become a "developer" (see "Developers" below). </li>
   </ol>
   Perhaps the dedicated user-clone is only updated "every few days".
   </li>
   <li> How can we create special "views" for the users? They should be able to register
   for modules or subjects, and then get commit-notifications related to those modules.
   Sending them also the patches? Or the new files??
   </li>
   <li> A major problem is how to filter out the "secret" parts.
    <ol>
     <li> Hopefully with "git-submodule" we can create sub-repositories
     (see Buildsystem/plans/VersionControl.hpp). </li>
     <li> Likely critical are only the plans-directories. The simplest measure is to
     create a sub-module without the plans-directories. And for special users we add then
     appropriate plans-directories. </li>
     <li> See "Research sub-modules" in Transitional/plans/general.hpp. </li>
     <li> Seems to be solved by simply not separating out the "secret research". </li>
    </ol>
   </li>
  </ul>


  \todo Developers
  <ul>
   <li> Different types of developers:
    <ol>
     <li> main developer (OK) </li>
     <li> core developer (full access to main repository) </li>
     <li> local developer (only access to a controlled clone; local to Swansea) </li>
     <li> external developer (only access to a controlled clone) </li>
    </ol>
   </li>
   <li> See "Developers information" in Buildsystem/plans/Configuration.hpp. </li>
   <li> Different defaults for mailing lists. </li>
   <li> Everybody can submit patches. </li>
   <li> Perhaps everybody except core developers should (always) work with branches ?!
   Submission then to the master-branch. </li>
   <li> Local and external developers each have their own dedicated push+pull clones. Core developers
   connects it with the main repository. </li>
  </ul>


  \todo Package construction script
  <ul>
   <li> Writing the package-construction script. </li>
   <li> A package exists for every new version number of Transitional. </li>
  </ul>


  \todo Release plan
  <ul>
   <li> In Annotations/Release-1_0 we have a release plan for version 1.0
   --- what to do with this plan? </li>
   <li> Release plans perhaps should move to here --- or stay in Annotations? </li>
  </ul>


  \todo Competition extraction tools
  <ul>
   <li> These tools, for a given program build a package, which contains
   all necessary source code and can be build using make (producing then
   the executable). </li>
   <li> A problem here are the link-libraries. Without them, one could use
   the ability of g++ to produce compilation units (using the option "-E"
   like "g++ -E Program.cpp -o Program.ii"), putting everything into one
   big file. </li>
   <li> Knowing the link-libraries, just compiling Program.ii with the
   right .o-files yields the program. </li>
   <li> So when we know all compilation units, we can preprocess them all
   and putting them into the package; the package just compiles the units
   separately, and then links them together. </li>
  </ul>


*/

