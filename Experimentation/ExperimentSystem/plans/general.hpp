// Oliver Kullmann, 2.11.2005 (Swansea)
/* Copyright 2005 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/plans/general.hpp
  \brief Launching and monitoring experiments on remote machines.


  \todo Update namespaces.


  \todo Transfer
  <ul>
   <li> First step is to transfer the experiment-system from OKgenerator
   (see directory ExperimentSystem/RandomGenerator) to here. </li>
   <li> Compilation is not an issue yet, but renaming and initial
   documentation. </li>
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

