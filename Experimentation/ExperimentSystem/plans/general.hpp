// Oliver Kullmann, 2.11.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/plans/general.hpp
  \brief Launching and monitoring experiments on remote machines


  \todo Update namespaces


  \todo Organisation
  <ul>
   <li> The script "DataCollection.R" in Experimentation/ExperimentSystem
   is misnamed, since it considers only some very special form of experiments.
   </li>
   <li> It is also misplaced, as a special tool. </li>
   <li> A directory ExperimentSystem/HelpingInvestigations shall be created,
   with subdirectories which have the same names of those in
   Experimentation/Investigations, and where special special R-scripts
   helping these investigations are placed. </li>
   <li> ExperimentSystem/DataCollection.R shall be moved there, with all
   appropriate updates. </li>
   <li> At this time then a proper specification of the functionality in
   ExperimentSystem/DataCollection.R (currently) is needed. </li>
  </ul>


  \todo Simple script for monitoring remote processes
  <ul>
   <li> See "Translation via addition" in
   RamseyTheory/VanderWaerdenProblems/Transversals/plans/UsingSAT.hpp
   for first plans. </li>
   <li> See "Adding distribution power to SplittingViaOKsolver" in
   ExperimentSystem/plans/DistributedSolving.hpp for a concrete project. </li>
   <li> See "Launching and monitoring" for more specific requirements. </li>
   <li> Such a script perhaps visits every hour each process, restarts it if
   necessary, and performs also some output-action, stored to some file
   dedicated to the experiment. </li>
   <li> So that in most cases one just needs to inspect local files, and, more
   important, is sure that the experiment runs continuously. </li>
   <li> In case the process can't be restarted an e-mail is sent to the
   administrator. </li>
   <li> It seems easiest just to write the various outputs into files;
   one could also think of adding a line to a table in a html document or
   an rss feed, but I (OK) definitely prefer simple files. </li>
   <li> The experiments are stored via a simple format in a file, which
   can be arbitrary changed (but one should make sure that the hourly
   action is not happening just then, so only working with a copy of the
   configuration file, and overwriting the old only at the end). </li>
   <li> A configuration line could contain the command to login, the
   command to check whether the process is running, the command to
   re-start the process, and the command for producing output. </li>
   <li> How can we just find out whether *new* output happened, and only
   show this? </li>
   <li> Using "ps" for checking the status of a process seems appropriate;
   but one should not just check the pid (another process could have attained
   the same pid). </li>
  </ul>


  \todo Transfer
  <ul>
   <li> First step is to transfer the experiment-system from OKgenerator
   (see directory ExperimentSystem/RandomGenerator) to here. </li>
   <li> Compilation is not an issue yet, but renaming and initial
   documentation. </li>
  </ul>


  \todo Improve naming and documentation of rows2column_df
  <ul>
   <li> "rows2columns_df" in DataCollection.R was a quickly written
   function to allow one to aggregate data from various rows in a data.frame.
   Certain columns remain constant, and another "key" column ranges over
   a fixed set, associating with values in a "value" column. The rows are
   then aggregated such that new columns are introduced for each key, and
   there is a row for each unique set of values in the constant column, where
   each column in the row is given values from the "value" column associated
   with each key. </li>
   <li> The above needs to be better written and placed in the documentation.
   </li>
   <li> The name and placement of the function also needs to be considered.
   </li>
  </ul>


  \todo Working again
  <ul>
   <li> Make the tools compiling and running correctly. </li>
  </ul>


  \todo Necessary extensions (improvements) of the old experiment-system
  <ul>
   <li> Descriptors:
    <ol>
     <li> It must be possible to process "ad-hoc experiments"; the simplest
     way to do so is to allow as input a directory-name, and then all
     files in there will be processed. </li>
     <li> Additional parameters specifying the experiment like the
     abortion-time. </li>
    </ol>
   </li>
   <li> Database (see module OKDatabase) must be extended to administer
   also ad-hoc experiments. </li>
  </ul>


  \todo Launching and monitoring
  <ul>
   <li> The launch-and-monitor system follows the old 3 steps (preparation,
   processing, transfer), but now all these steps can be launched
   automatically. </li>
   <li> So a database of available machines is needed (access via ssh). </li>
   <li> When starting processing an experiment, the pid is extracted, so
   that then the output of the ps-command for this process can be shown. </li>
   <li> Also "ls -l" for the experiment-directory and "less" for the files
   in them must be supported. </li>
   <li> For launching a new experiment (always on a fixed machine; we
   simply do not support transferring experiments to different machines,
   but it must be easy to abort an experiment and transfer the data
   obtained so far) one can ask for the available machines (showing a
   status report about availability, how many experiments are currently
   running on it, how many processors the machine has, bogomips etc.) and
   then, via some command perhaps using a syntax like chmod for
   "prepare yes/no, process yes/no, transfer yes/no", say, +-+, an
   experiment can be started. </li>
   <li> Perhaps we use simple single tools for the different actions
   (querying the status, launching an experiment), and use simple
   copy-and-paste to transfer data from one step to another. </li>
   <li> See "Simply script for monitoring remote processes" above for
   a simple but flexible tool which just monitors (arbitrary) processes. </li>
   <li> "autoson" by McKay http://cs.anu.edu.au/~bdm/autoson/ :
    <ol>
     <li> The licence situation is unclear: We can only use software
     which can be freely redistributed; if it is enough just to keep the
     package, then this should be alright. </li>
     <li> A serious restrictions seems to be the common filesystem: So
     apparently actually the software is not usable for us, since in most
     situations machines are just connected by ssh ?? </li>
     <li> In most cases, such as on PCs in the CS linux lab, the machines have
     at least the user's home directory in common (via NFS etc). Also, there 
     are always solutions such as sshfs (see 
     http://fuse.sourceforge.net/sshfs.html ), although admittedly, these add 
     further complexity. </li>
    </ol>
   </li>
   <li> BOINC http://boinc.berkeley.edu/ :
    <ul>
     <li> Documentation available at 
     http://boinc.berkeley.edu/trac/wiki/ProjectMain . </li>
     <li> BOINC is intended for grid computing and could be useful for small 
     experiments on university computers, but then could be easily expanded
     to allow volunteers to offer computing resources for larger experiments. 
     </li>
     <li> BOINC also offers a wrapper script, so arbitrary applications
     can be run using it's system rather than just custom BOINC-specific
     applications (although applications that communicate directly
     with the BOINC system may be able to store more information).
     </li>
    </ul>
   </li>
   <li> Condor http://www.cs.wisc.edu/condor/ :
    <ul>
     <li> This might be the right tool, with the needed flexibility. </li>
     <li> OK has contacted condor-admin@cs.wisc.edu to explore whether it
     can handle our standard approach (lanch/start+restart/submit). </li>
    </ul>
   </li>
  </ul>


  \todo Tests
  <ul>
   <li> The tests must be written in such a fashion, that they are largely
   testable without accessing ssh; for testing the parts essentially using
   ssh one can then simply use  the host machine of the testing process. </li>
  </ul>

*/

/*!
  \namespace OKlib::ExperimentSystem
  \brief Components for performing experiments
*/

namespace OKlib {
  namespace ExperimentSystem {
  }
}

