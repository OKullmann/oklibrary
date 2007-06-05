// Oliver Kullmann, 4.6.2007 (Swansea)

/*!
  \file Buildsystem/plans/MakeSystem.hpp
  \brief Plans for the core part of the build system, the
  make- or make-like engine

  \todo CMake
  <ul>
   <li> Can cmake handle our "fractal" or "recursive" directory structure, where we have
   arbitrary nested subdirectories, inside we find standard functional (sub-sub-)directories like
   "tests", "testobjects", "demos", "apps", "messages", "plans", "docus", and from these
   functional directories the source-code files are to be extracted, and name-surgery is
   applied to compute the corresponding directories under system_directories ?!? </li>
   <li> Can cmake handle the uniformity of our system, where for Transitional we only have
   2 makefiles (generic.mak and recursive.mak), and every directory contains (if wished) only
   a symbolic link to one of those (utilising then the definitions.mak files) ?!? We do not
   want to clutter the source directory with code for the build system, but at any point we want
   to be able to issue a make-command. </li>
   <li> Can cmake handle the dynamic aspects of our build system, where with every make-command
   all the programs and libraries to be compiled and all executables to be executed are first determined
   dynamically, namely what exists currently in the source-code tree?!?
    <ol>
     <li> The possibilities to manipulate variables seem very restricted?? There seem to
     exist no functions??? </li>
     <li> What is needed is a "meta-make", which from configuration values, parameter
     values and the current state of the file-system first creates all targets, corresponding
     names and paths etc., and then creates the make-file. One would guess that this
     exactly should be possible with cmake?? With make we have the possibility due to
     the two-stages concept, but it's hard to create the dependencies on the target. </li>
     <li> So ADD_EXECUTABLE and ADD_LIBRARY depend on the output of a shell-command (typically "find")
     --- this doesn't seem possible? But with FOREACH and WHILE it should be possible. </li>
    </ol>
   </li>
   <li> It seems one doesn't have fine-grained control over the separation of the cmake-specific files
   and the library- and binary-files? Apparently cmake expects the user to issue a final "install"
   command, which will put the lib- and bin-files to their "final" destination?!? But we have
   a continuous process! </li>
   <li> It seems that the basic idea between the higher-level "cmake" and the lower-level "make" is
   here, that every time a new files is introduced (or something similar), one must run cmake, while
   otherwise one can just run make. Seems not unreasonable, but of course one introduces a further
   possibility for making mistakes ?!? </li>
   <li> Investigate how our main tasks can be handled by cmake. </li>
   <li> How would the work flow be with the new system (the 2 levels,
   the cmake- and the make-level)? </li>
   <li> Is ccmake useful? DONE (just a program for setting configuration variables, so yes, it is
   useful for that purpose) </li>
  </ul>

*/

