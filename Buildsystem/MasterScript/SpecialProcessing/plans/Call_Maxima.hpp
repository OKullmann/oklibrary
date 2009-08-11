// Oliver Kullmann, 11.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
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
     <li> Thus all files used and all files created by Maxima (itself) must
     reside in the Maxima installation directory. </li>
     <li> Currently this is handled by redefining HOME. </li>
     <li> It would be good if this could be replaced by using the (new)
     command line option "--userdir=", however at least under Ecl and CLisp
     the file .maxima_history is not ruled by userdir (but just by the
     home-directory), and so yet we can not switch. </li>
     <li> Notify the Maxima mailing list about this bug (this should be the
     responsibility of rmaxima). </li>
    </ol>
   </li>
   <li> No interference of different invocations:
    <ol>
     <li> We need to make sure that we can start several Maxima sessions via
     "oklib --maxima" without interferences with themselves or with a run of
     the Maxima test system. </li>
     <li> See "Improving the test system" in
     ComputerAlgebra/TestSystem/Lisp/plans/general.hpp. </li>
     </li> And furthermore several simultaneous runs of the test system should
     be possible. </li>
     <li> The culprit is maxima-init.mac, which is written and then
     possibly corrupted (in the interval between writing of the file,
     and Maxima actually reading it) by these processes. </li>
     <li> The only handle on this initialisation file is given by
     the maxima-option "--userdir=". This directory is the first directory
     where to search for the initialisation-file. </li>
     <li> So the only possibility is to create for each "run" a new
     userdir-directory. </li>
     <li> An oklib-invocation would be one run, and one testsystem-run
     would be one run (just a single run for the whole test-run). </li>
     <li> In this temporary userdir-directory a symbolic link to the
     .maxima/binary directory must be placed, to avoid recompilation of
     some of the packages each time. </li>
     <li> What about the file .maxima_history ? </li>
     <li> Yet (see above) its placement can only by controlled by using
     HOME. </li>
     <li> The standard oklib-invocation should use the standard .maxima_history
     file (read it, and write it upon exit). This needed to be done
     by a symbolic link. </li>
     <li> But testsystem-runs, and also other runs produced by scripts
     shouldn't interfere with the standard history, and so for these
     invocation no link to the .maxima_history should be set. </li>
     <li> Actually, it seems that it is rmaxima, which creates this history
     file --- shouldn't we use for the testruns and other script-runs just
     the maxima-executable ?! In this way we would solve the problem, that
     such automated runs could destroy the Maxima-history. </li>
     <li> So the Maxima configuration-file should have "maxima_call_okl",
     which actually calls *maxima*, and "rmaxima_call_okl", which call
     rmaxima. </li>
    </ol>
   </li>
   <li> If we ever can get rid off redefining HOME, then we need to remember
   that the link to .Xauthority should then no longer be created. </li>
  </ul>

*/

