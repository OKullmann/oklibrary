// Oliver Kullmann, 11.8.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/MasterScript/SpecialProcessing/plans/Call_Maxima.hpp
  \brief Plans for the components reponsible for calling Maxima


  \todo Improve locality
  <ul>
   <li> No interference with user installations:
    <ol>
     <li> We need to make sure that if a user has his own Maxima installation,
     then there is no interference. </li>
     <li> DONE
     Thus all files used and all files created by Maxima (itself) must
     reside in the Maxima installation directory. </li>
     <li> DONE
     Currently this is handled by redefining HOME. </li>
     <li> It would be good if this could be replaced by using the (new)
     command line option "--userdir=", however at least under Ecl and CLisp
     the file .maxima_history is not ruled by userdir (but just by the
     home-directory), and so yet we can not switch. </li>
     <li> Notify the Maxima mailing list about this bug (this should be the
     responsibility of rmaxima). </li>
    </ol>
   </li>
   <li> Automatic runs of Maxima shouldn't change the history:
    <ol>
     <li> It seems that the maxima-executable itself doesn't manage the
     history, but it is rmaxima which does that. </li>
     <li> The history file is Installation_dir/.maxima_history. </li>
     <li> DONE
     This placement is achieved by resetting HOME. </li>
     <li> DONE (it seems that running rmaxima in batch-mode doesn't
     change the history)
     Testsystem-runs, and also other runs produced by scripts
     shouldn't interfere with the standard history. </li>
     <li> Confirm on the Maxima mailing-list that batch-mode doesn't interact
     with the history. </li>
     <li> Shouldn't we use for the testruns and other script-runs just
     the maxima-executable ?! It seems that rmaxima here is of no use. </li>
     <li> So the Maxima configuration-file should have "maxima_call_okl",
     which actually calls *maxima*, and "rmaxima_call_okl", which call
     rmaxima. </li>
    </ol>
   </li>
   <li> No interference of different invocations:
    <ol>
     <li> We need to make sure that we can start several Maxima sessions via
     "oklib --maxima" without interferences with themselves or with a run of
     the Maxima test system. </li>
     </li> And furthermore several simultaneous runs of the test system should
     be possible. </li>
     <li> And finally (most important for experimentation), we need to run
     in parallel Maxima in batch-mode. </li>
     <li> The culprit is maxima-init.mac, which is written and then
     possibly corrupted (in the interval between writing of the file,
     and Maxima actually reading it) by these processes. </li>
     <li> See below on how to possibly handle this. </li>
    </ol>
   </li>
   <li> Set userdir:
    <ol>
     <li> One handle on this initialisation file maxima-init.mac is given by
     the maxima-option "--userdir=". This directory is the first directory
     where to search for the initialisation-file. </li>
     <li> So the possibility is to create for each "run" a new
     userdir-directory. </li>
     <li> An oklib-invocation would be one run, one testsystem-run
     would be one run (just a single run for the whole test-run), and
     one batchmode-invocation would be one run. </li>
     <li> In this temporary userdir-directory a symbolic link to the
     .maxima/binary directory must be placed, to avoid recompilation of
     some of the packages each time. </li>
     <li> So we need special Maxima-calls for
      <ul>
       <li> ordinary usage </li>
       <li> testsystem-runs </li>
       <li> batch-mode </li>
      </ul>
      Especially for the batch-mode we need further support to make it
      more convenient. And the testsystem-runs all just use one common
      directory for all the runs (however different invocations of the
      testsystem create different such directories).
     </li>
     <li> As said above, also for ordinary usage different directories are
     created, however they all share the same history (different from the
     other forms of Maxima-calls, which don't change history). </li>
     <li> The directory names should indicate the type of call plus a time
     stamp, now finer than a second, however still human-readable. </li>
     <li> These temporary directories need to be removed after usage. </li>
    </ol>
   </li>
   <li> Set init-mac:
    <ol>
     <li> Maxima 5.23.2 has the option "--init-mac=<file>". </li>
     <li> Hopefully <file> can be a full path. </li>
     <li> So perhaps for each invocation a new variant of maxima-init.mac is
     to be created, like "maxima-init_1234.mac". </li>
     <li> This should be better than redefining userdir for each invocation
     (see above). </li>
     <li> One problem is that unsuccesful invocation might leave garbage
     initialisation-files. </li>
     <li> Another problem is how to pass the temporary value for
     "$(maxima_init_okl)" to the make-target run_maxima. </li>
    </ol>
   </li>
   <li> If we ever can get rid off redefining HOME, then we need to remember
   that the link to .Xauthority should then no longer be created. </li>
  </ul>


  \todo Setting userdir
  <ul>
   <li> The current setting "HOME=$(maxima_homedir_okl)" should likely be
   replaced by the option "--userdir=$(maxima_homedir_okl)". </li>
   <li> With "maxima_homedir_okl ?= $(maxima_installation_dir_okl)/.maxima".
   </li>
   <li> Compare "Improve locality" above. </li>
  </ul>

*/

