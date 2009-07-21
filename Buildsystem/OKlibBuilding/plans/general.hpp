// Oliver Kullmann, 28.2.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/OKlibBuilding/plans/general.hpp
  \brief Build-related plans for the OKlibrary (in the strict sense)


  \bug Mixing C and C++
  <ul>
   <li> If we have files %XXX.c and XXX.cpp (with the same prefix) then
   the build-system apparently does not work properly. </li>
   <li> Either this gets corrected, or at least documented
   (it appears we should forbid this). </li>
   <li> The build-system should also warn about this (and perhaps
   also about other assumptions not met). </li>
  </ul>


  \bug Incorrect build of new tests I
  <ul>
   <li> The buildsystem does not handle the dependencies for the
   module-test-program. </li>
  </ul>


  \bug Incorrect build of new tests II
  <ul>
   <li> While error I is as expected (at this time), the buildsystem behaves
   differently when called within a module or from a higher level:
    <ol>
     <li> Target new_check works from for example from level OKsystem %w.r.t.
     building test-object files if needed, </li>
     <li> but from within the module this doesn't work. </li>
    </ol>
   </ul>


  \todo %Test system
  <ul>
   <li> The most immediate thing is to make the test system work correctly,
   which (at least?) means that the make-system gets to know the relevant
   dependencies, so that recompilation of tests happens whenever necessary
   (at this time (18.6.2006) the test object files need to be manually deleted
   (via "make cleantestobj") to enforce recompilation). </li>
   <li> We need special test-modes for more extensive messages. </li>
   <li> The output of testprograms is copied into a suitable subdirectory of
   system_directories/log. Perhaps also the test time-stamps should go here?
   </li>
  </ul>


  \todo Setting the paths to GCC and Boost link libraries
  <ul>
   <li> The setting of the Make variable "alternative_library_path" needs to
   be reviewed. </li>
   <li> For example, the name "alternative_library_path" is no longer
   appropriate. </li>
   <li> DONE (machine_bits_okl provides either value 32 or 64; using "uname"
   instead of the environment variables below)
   The build system should find out whether the platform is 32- or
   64-bit, and select then only the needed links (environment variables
   HOSTTYPE and CPU). </li>
  </ul>
  

  \todo User control of system versions
  <ul>
   <li> There should be a simple method for the user to specify that the
   buildsystem uses the system-wide installations of GCC and Boost. </li>
   <li> It should be possible to specify only the system-wide GCC or only the
   system-wide Boost, or both. </li>
  </ul>
  

  \todo General_options
  <ul>
   <li> The current function of "General_options" is taken over by the new
   variable "Debug_options". </li>
  </ul>


  \todo Dependency files
  <ul>
   <li> Since options might define macros (which might control conditional
   inclusion), the options when computing the dependency files should be
   identical to those when actually compiling the files. </li>
   <li> If when creating the .d-files an error occurs (for example
   due to an inaccessible header file), then for some reason subsequent
   "make check" erroneously succeeds. </li>
   <li> Using the gcc option "-MP" ? </li>
   <li> We have the following problem:
    <ol>
     <li> If one is using different paths due to symbolic links, then the
     dependency files contain unusable information (and must be deleted with
     "make cleandep"). </li>
     <li> This problem seems hard to solve (one had to find out that different
     paths lead to the same file). </li>
     <li> So it must be documented well. </li>
    </ol>
   </li>
  </ul>


  \todo Verbosity
  <ul>
   <li> By default the make-process outputs it's basic assumptions (what
   compiler to be used, what external libraries, etc.). </li>
  </ul>


  \todo Targets
  <ul>
   <li> It should be possible to build just one application, or just one
   implementation. </li>
  </ul>


  \todo Role of srcdir
  <ul>
   <li> An update of the following discussion is needed; compare
   Buildsystem/OKlibBuilding/plans/TargetSpecifications.hpp. </li>
   <li> What is the role of variable srcdir ? Isn't the definition in
   Recursive.mak superfluous now? </li>
   <li> It is used in makefile_generic (so that we can call makefiles from
   other places, without a change in behaviour; we should also document this),
   but why the definition in Recursive.mak ? </li>
   <li> Can't makefile_generic define it on its own
   (respectively, shouldn't the definition in makefile_generic suffice) ? </li>
   <li> Within recursive makefile-invocations we can use the option
   "--directory=DIR", while from the command line, when calling a makefile
   from another directory, the option "-C DIR" can be used. </li>
   <li> This seems to make srcdir superfluous? </li>
   <li> If so, then is it worth to keep it for convenience? </li>
   <li> Those settings of srcdir which remain should (if at all) receive some
   inline comments (these settings are quite arcane). </li>
  </ul> 


  \todo Cleaning
  <ul>
   <li> We need cleaning tools which clean up directories (not single files).
   </li>
   <li> We need specialised cleaning for applications and link-libraries. </li>
   <li> We need specialised cleaning regarding the test system:
    <ol>
     <li> Cleaning of special or all versions of the test-timestamps. </li>
     <li> Cleaning of test-objectfiles and test-programs. </li>
     <li> Cleaning of test-dependencies. </li>
    </ol>
   </li>
  </li>


  \todo %Test cleaning
  <ul>
   <li> We need specialised cleaning regarding the test system. </li>
   <li> Cleaning of special or all versions of the test-timestamps. </li>
   <li> Cleaning of test-objectfiles and test-programs. </li>
   <li> Cleaning of test-dependencies. </li>
  </ul>


  \todo Error messages of gcc should be processed
  <ul>
   <li> We should support using a tool like TextFilt or STLFilt. </li>
  </ul>


  \todo Linking and options:
  <ul>
   <li> We need global control over dynamic/static linking. Is it the case
   that dynamic linking only happens with .so files? </li>
   <li> Can investigate with strace tool.  </li>
   <li> Is it possible to specify the path exactly to a link-library at
   runtime? </li>
   <li> Which compiler options are effective when linking? "Our" options are 
   <ul>
    <li> -ansi (cc1plus) </li>
    <li> -pedantic (cc1plus)</li>
    <li> -Wall (cc1plus)</li>
    <li> -g (cc1plus) (mentioned as "ignored" in the ld man page)</li>
    <li> -03 (cc1plus)</li>
    <li> -DNDEBUG (cc1plus)</li>
   </ul>
   Here, the names in brackets indicate, in the
   case of ld (linker) and cpp (preprocessor), the executables whose man 
   page documents the option and, in the case of cc1plus whether the help
   file documents the option. It seems, at first glance, that all of "our"
   options are only effective for the compiler.
   </li>
   <li> CXXFLAGS is not used when linking the compilation units together ---
   is this how it should be, and how to set options for the linking stage?!
   </li>
   <li> What is the meaning of the strip-binutil-tool? Shall we use it?
   (Always? Sometimes?) At least we should have the option. </li>
  </ul>


  \todo Force make
  <ul>
   <li> Calling make with the option "-B" (or "--always-make") does not
   work (it leads to an infinite loop) --- why is this so? </li>
   <li> What can be done about it --- it would be nice to be able to force a
   rebuild, without having to delete some directories (this might be
   dangerous). </li>
  </ul>


  \todo Compilation information
  <ul>
   <li> We need a standardised way of how to make information about the
   compilation process available to a program (and also the name of the
   program, etc.), so that for example via --version we get as much
   information as possible. </li>
  </ul>


  \todo Nightly build
  <ul>
   <li> Full check-out of the library and full compilation and testing
   (i.e., create the package, un-archive it, build it with "make" and then
   run "make check" in it). </li>
   <li> Testing should invoke valgrind (with Test_tool="valgrind --quit").
   </li>
  </ul>


  \todo Complexity system
  <ul>
   <li> "make measurements" will create an xml-file
   (via the boost serialisation library) with information about all
   operations which have been registered. </li>
   <li> A little viewing-program allows to monitor these measurements (as they
   evolve over time). </li>
  </ul>


  \todo Measurements
  <ul>
   <li> Each "make test" etc. should gather summary statistics of the tests
   performed, like the total number of testobjects, the total time spend and
   so on. </li>
   <li> In order to do so, a test program can be asked to serialise the
   statistics to a file or to standard output, and a simple evaluation program
   gathers these statistics. </li>
   <li> Similar to the complexity measurements, for every task performed by
   the build system it should be possible to save the measured run time, so
   that the development over time for example of compile times, link times,
   test times can be followed, and also the influence of compilers can be
   considered. </li>
   <li> By default, when running make only the total times are output, but in
   a protocol mode everything is written to a file (as for the complexity
   system; it should be possible for example to use the visualisation
   tools there to look at the developments here). </li>
  </ul>


  \todo New targets (needs update)
  <ul>
   <li> "create_new_module" and "create_new_submodule",
   which create a new subdirectory with test-program respectively new
   .hpp, _Tests.hpp and _Testobjects.hpp files (with additional inclusion
   in the testprogram). </li>
   <li> "--create-links" as command for oklib, for creating symbolic links
   in ~/bin for all main programs:
    <ol>
     <li> all solver and related programs </li>
    </ol>
   </li>
  </ul>

*/
