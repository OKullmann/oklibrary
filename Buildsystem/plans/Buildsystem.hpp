// Oliver Kullmann, 12.8.2005 (Swansea)

/*!
  \file Buildsystem/plans/Buildsystem.hpp
  \brief Plans for the buildsystem in general

  \bug system_definitions.mak:
  The boost-variable definitions are inconsistent with the
  rest of the build-system, so they should be reverted (as
  discussed).
  OK: It seems this was done??

  \todo License:
  It seems LGPL is suitable; we have to check this (is it compabible with all
  other licenses (for external sources) involved? I guess so). Once decided,
  we have to add appropriate licence texts to every file.

  \todo Make-variables for external libraries
   - In system_definitions.mak we have a definition of Boost which seems wrong
     to me (OK) (or at least misleading --- or we need special documentation at this point, namely
     about the (precise) definition of Boost in the whole build-system).
   - How to define such variables (like Boost and Ubcsat) in general? We need a nice little method
     (plus documentation), so that we can add easily as many external libraries as we want.

  \todo Role of srcdir
  <ul>
   <li>
     What is the role of variable srcdir ? Isn't the definition in makefile_recursive
     superfluous now?
     It is used in makefile_generic (so that we can call makefiles from other places,
     without a change in behaviour; we should also document this), but why the
     definition in makefile_recursive ? Can't makefile_generic define it on its own
     (respectively, shouldn't the definition in makefile_generic  suffice) ?

     Within recursive makefile-invocations we can use the option "--directory=DIR",
     while from the command line, when calling a makefile from another directory,
     the option "-C DIR" can be used. This seems to make srcdir superfluous?
     If so, then is it worth to keep it for convenience?
   </li>
   <li>
     Those settings of srcdir which remain should (if at all) receive some inline comments
     (these settings are quite arcane).
   </li>
  </ul> 

  \todo System documentation:
   - Document the basic version-control settings (location of server, configuration, how to use it).
   - Document the build-system (general ideas, functionality).

  \todo Documentation (Examples and Concepts) OK : this needs to be discussed and updated
  <ul>
    <li> At OKplatform-level we have a new directory Documentation, with the following sub-directories:
      <ol>
        <li> Examples : contains a mirror of the OKlibrary with (many) example applications (build by the
        build system, so that it's always up-to-date).</li>
        <li> Concepts : contains for each module a latex-file discussing the concepts for this module.</li>
      </ol>
    </li>
  </ul>

  \todo Overhaul of the general targets:
   - "all" should not compile the test-programs. Rather we want to have it so 
     that "make new_check" and "make check" does the compilation. For performing
     the checks they need to be compiled; we also need then special targets to 
     just compile the test-programs.
   - we must look at the support for linking with .o files from
     the library itself (including linking with different versions)
   - What is the role of prebuild? Still it is not eliminated --- do we need it?.

  \todo Force make:
  Calling make with the option "-B" (or "--always-make") does not
  work (it leads to an infinite loop) --- why is this so? What can be done
  about it --- it would be nice to be able to force a rebuild, without having to delete
  some directories (this might be dangerous).

  \todo Design   
   - Larger makefiles should be composed (via inclusion) out of smaller makefiles (if possible;
     otherwise there must be a "copy-and-paste"-comment at each place.
   - We should use (more) make-functions.

  \todo Modes of Usage:
  Two modes of usage of the build system:
   - As we have it now, everything in one directory (and also with the possibility of
     having different versions of the OKplatform). ("One world", extending directly
     the library.)
   - Having one global OKplatform, and then a local directory with also aux, bin, doc
     include and lib, but only regarding the local directory. So that for example students
     in the Linux lab can use a central installation of OKplatform, and in the local
     directory only the files related to their production is stored.

  \todo makefile_recursive
   - This should go, and makefile_generic should be able to do all jobs,
     gathering all relevant files from all underlying subdirectories (but as soon
     as one subdirectory doesn't contain makefile_generic, then it and its
     descendants are ignored). It seems important that for example every subdirectory
     of a module can itself contain tests- and testobjects-subdirectories. The
     directory-tree in system_directories must mirror the primary directory-tree.
   
  \todo Compilation information: 
   - We need a standardised way of how to make information about the compilation
     process available to a program (and also the name of the program, etc.), so that
     for example via --version we get as much information as possible.

  \todo Special runs
   - It seems we should create a special target "valgrind-check" where the files are
     especially compiled for Valgrind --- this seems to be needed to do automatic
     checks. So then the build system and the test system would be affected.

  \todo Source code directory structure:
   - We need a rational system for the naming of header files. We should study the
     Boost library here.
   - The namespace should be the directory part of the name for the OKlib (for example OKlib::Algorithms
     is the file OKlib/Algorithms).
   - Directory OKsystem/OKlibrary should then be called OKsystem/OKlib
   - Should we use <> or "" for includes from our library?
   - As proposed in [C++ Templates], we should also provide files per module
     only containing declarations (perhaps parallel to each .hpp file). Ending "_decl" ?
   - Perhaps special naming conventions for files with messages or exceptions ?!
   - Module.hpp provides all includes, Module_decl.hpp all declarational includes

  \todo Version control
  <ul>
   <li> We have a little problems with submissions to the repository, which often span
   many files, so the whole submission process takes a while, and it's not clear from
   outside when it's finished (and the library is again in a well-defined state).
    <ul>
     <li> We need some (e-mail) protocol to handle the situation with CVS. </li>
     <li> Subversion has atomic commits, however it seems that for one commit one can
     only use one log-message, and thus this feature is to weak here to be useful
     (one should check this at the subversion-e-mail list). </li>
     <li> With Git the problem should be easy to solve: All the submissions first
     happen locally, and then, in one go, all changes are submitted to the central
     repository! </li>
    </ul>
   </li>
   <li> The main conceptual disadvantage (shared with CVS) of Subversion is that
    no local repositories are possible; alternatives:
    <ol>
     <li> Git : http://git.or.cz/ looks rather good --- one should try it out! </li>
     <li> svk (http://svk.elixus.org), apparently a further development of Subversion.
     Looks somewhat immature. </li>
     <li> What about Arch? </li>
     <li> BitKeeper (http://www.bitkeeper.com) seems to be only proprietary. </li>
    </ol>
    We should try out Git, in two steps:
     <ol>
      <li> Playing around with it (creating little repositories). (MH, OK, ML) </li>
      <li> Then, in a concentrated effort, the whole OKlibrary is copied to Git, and
      submissions are done for both systems; after a few days we decide which of the
      two systems will be used. </li>
     </ol>
   </li>
   <li> A disadvantage of Subversion compared to CVS is that individual
   files do not have version numbers (these "individual version numbers"
   are better understood as a change-statistics, documenting activity).
    <ul>
     <li> This could easily be changed
      <ul>
       <li> by a new subversion-version </li>
       <li> or by a graphical user interface which determines how often with
       a new version also the considered file has changed. </li>
      </ul>
     One should ask at the subversion-e-mail list (referring to such "file
     version numbers" perhaps as some form of basic statistics). </li>
     <li> What about Git? What is the version-numbering-systems there, and what kind
     of statistics are supported? </li>
    </ul>
   </li>
   <li> CVS cannot handle links:
    <ul>
     <li> Version 1.0.8 of Subversion cannot handle links either --- does this change
     with newer Subversion versions? </li>
     <li> What about Git? </li>
    </ul>
   </li>
   <li> Sending notification-e-mails:
    <ul>
     <li> How to tell the Subversion server to send out e-mails in case of commits?
     "Hook scripts" seem the answer here, especially the commit-email.pl script
     (however it seems that the whole process is not completely straight-forward?). </li>
     <li> How about Git? </li>
    </ul>
   </li>
   <li> Ignoring files:
    <ul>
     <li> Ignoring files is handled by Subversion with the svn:ignore property of
     directories: This property has to be set to (for example) the list of forbidden
     patterns in .cvsignore,
     using
      <p> OKplatform/OKsystem/Transitional> svn propset svn:ignore -F ~/.cvsignore * </p>
     (better, instead of * use
      <p> $(find * -type d -and -not -path "* /.*") </p>
     (where the space in the shell pattern needs to be removed --- we must avoid ending the comment here!)),
     so that in all subdirectories these patterns are ignored). </li>
     <li> What about Git? </li>
    </ul>
   </li>
   <li> Change dates and revision numbers in files:
    <ul>
     <li> In Subversion, the use of $Date and $Revision in macro OKLIB_FILE_ID is
     replaced by
      <ul>
       <li> $LastChangedDate$ </li>
       <li> $LastChangedRevision$ </li>
      </ul>
     and for these files the property svn:keywords has to be set:
      <p> svn propset svn:keywords "LastChangedDate LastChangedRevision" </p>
     Should we configure subversion's automatic property setting to set svn:keywords for
     these files? </li>
     <li> What about Git? </li>
    </ul>
   </li>
   <li> The central repository:
   For a new version control system we have to find out how to establish the role of the repository at
   cs-oksvr as *central*, and how to manage access control (as fine-grained as possible; if possible not
   relying on ssh). </li>
   <li> Models of behaviour for distributed version control:
   Find out how to do the following:
    <ul>
     <li> We have the central repository R and a local repository L. L is changed (with commits, logs
     and everything) to L', while meanwhile R has been changed to R'. Now it should not be possible to
     submit L' to R', since L' is not up-to-date; so first all changes in R' have to be committed to
     L', yielding L'' (without changing R'), and then L'' is submitted to R', yielding R''. </li>
     <li> For the (off-line) laptop csltok: Can we just copy the local repository to disc, then copy this
     to cs-wsok, synchronise it with the local repository on cs-wsok, run the tests, and then submit the
     local repository on cs-wsok to the central repository? Should be no problem. </li>
    </ul>
   </li>
   <li> External sources:
    <ul>
     <li> Optimally, the OKlibrary-package is exactly a clone of the repository (and then one needs to
     run "make" in the top-level directory).
      <ul>
       <li> For this also ExternalSources needs to be in the version control. </li>
       <li> And there needs to be a way to specify "sub-repositories" to separate the public parts
       of the library from the non-public parts. </li>
      </ul>
     </li>
     <li> If we cannot do this, then at least ExternalSources needs to be under version control on its own. </li>
    </ul>
   <li> The different parts in OKplatform:
    <ul>
     <li> As already mentioned in the point "External sources", optimally there is just one repository
     for the whole library (containing the current three repositories Annotations, OKlibrary, Transitional,
     and the new (sub-)repositories ExternalSources and Overview). Then we need the possibility to restrict
     access to parts of the repository (so that annotations and Transitional are not public). </li>
     <li> A problem is, that Annotations currently has public as well as non-public parts; likely this needs to be
     separated. </li>
    </ul>
   </li>
   <li> A distributed version control system as integral part of the library:
    <ul>
     <li> The version control system is built by the library (ExternalSources). </li>
     <li> One has to reflect on how an (external) user of the library might want to
     extend the library (under the version control!). </li>
     <li> Hopefully the distributed version control constitutes also the main part of the
     update-solution for an external user (who extended the library)! </li>
    </ul>
   </li>
  </ul>

  \todo Testing the build system
   - We need some test system for the build system. Optimally, it would run like our normal test
     system; perhaps this is hard to achieve, but at least we need a list of manual checks, well specified,
     which cover all functions of the build system, and which is performed from time to time (manually).
     Then we can partially automate it.

  \todo Full test
   -  makefile at the OKplatform level --- there the
     different versions of gcc and boost are visible. "make check" at
     this level should run through all appropriate combinations of gcc
     and libraries and test them.

  \todo Nightly build
   - Full check-out of the library (yet Transitional and 
     OKlibrary) and full compilation and testing (i.e., create the package,
     un-archive it, build it with "make" and then run "make check" in it).
     Testing should invoke valgrind (with Test_tool="valgrind --quit").

  \todo Complexity system: 
   - "make measurements" will create an xml-file
     (via the boost serialisation library) with information about all
     operations which have been registered. A little viewing-program
     allows to monitor these measurements (as they evolve over time).

  \todo Measurements:
   - Each "make test" etc. should gather summary statistics of the tests performed,
     like the total number of testobjects, the total time spend and so on.
     In order to do so, a test program can be asked to serialise the statistics
     to a file or to standard output, and a simple evaluation program gathers
     these statistics.
   - Similar to the complexity measurements, for every task performed by the build
     system it should be possible to save the measured run time, so that the development over
     time for example of compile times, link times, test times can be followed, and also
     the influence of compilers can be considered. By default, when running make only
     the total times are output, but in a protocol mode everything is written to a file
     (as for the complexity system; it should be possible for example to use the visualisation
     tools there to look at the developments here).

  \todo Compiler versions:
  Do we get slower run-times with g++ 4.1.2 compared to 4.1.1, and thus we
  need to write no-throw-declarations? (Perhaps this needs to be done anyway?)

  \todo Package construction script
   - Writing the package-construction script. (Internal versions should
     have date and time in the name.)

  \todo Competition extraction tools
   - these tools, for a given program
     build a package, which contains all necessary source code and can be build
     using make (producing then the executable). A problem here are the link-libraries.
     Without them, one could use the ability of g++ to produce compilation units (using
     the option "-E" like "g++ -E Program.cpp -o Program.ii"), putting everything into one big file.
     Knowing the link-libraries, just compiling Program.ii with the right .o-files yields the program.
     So when we know all compilation units, we can preprocess them all and putting them
     into the package; the package just compiles the units separately, and then links them together.

  \todo Distributing the library (needs update):
   - We must study how to distribute a copy of the whole CVS-system (so that later a re-merge is
     possible; also updates must be studied).
   - Two download possibilities: Either the whole library (with all included libraries like
     Boost, doxygen, PostgreSQL etc.), or only the minimum.
     This gives two user types: "full user" and "minimal user".
     We must make sure, that also the minimal user can use the test and the complexity system
     (compiling it himself, or using the build system). And we must study, how a full user can keep his
     file structure separate from the library by using links.

  \todo New targets (needs update):
   - "create_new_module" and "create_new_submodule",
     which create a new subdirectory with test-program respectively new
     .hpp, _Tests.hpp and _Testobjects.hpp files (with additional inclusion
     in the testprogram).

  \todo Higher-order build tools:
   - We should investigate CMake (http://www.cmake.org/HTML/Index.html), whether it would be
     useful for us. (autoconf and the like seems outdated ?)

*/
