// Oliver Kullmann, 15.8.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/DistributedSolving/plans/RProcessSplitViaOKsolver.hpp
  \brief Plans regarding remote-distributed SAT solving via SplittingViaOKsolver


  \todo Basic prototype
  <ul>
   <li> The "server" is just a directory with files (at least)
    <ol>
     <li> current_subinstance: the number of the next unsolved instance. </li>
     <li> RequestLog: per line the request-identifier and start and end of the
     interval delivered. </li>
     <li> SplitViaOKsolver: a link to that directory. </li>
     <li> Lock: an empty directory, whose presence shows that no other process
     can now read and update the server-directory. </li>
    </ol>
   </li>
   <li> A "processing-unit" has a working directory with files (at least)
    <ol>
     <li> Log </li>
     <li> Parameter </li>
     <li> Server (information on the server-directory and machine) </li>
     <li> Environment </li>
     <li> SubinstanceStatistics </li>
     <li> Instances (a directory) </li>
     <li> Data (information on the partial assignments) </li>
     <li> F </li>
     <li> current_subinstance (the line in Data to be processed next) </li>
     <li> N (the number of sub-instances to fetch at once). </li>
    </ol>
   </li>
   <li> RProcessSplitViaOKsolver takes as arguments, like
   ProcessSplitViaOKsolver, either a processing-unit-directory, for
   continuation, or information on the server-directory. </li>
   <li> Once all instances in the processing-unit-directory have been
   completed, the process reads a new chunk (size N) from the server-directory
   and updates it appropriately. </li>
   <li> That the read-and-update operation of the server-directory happens
   automatically, is simply guaranteed by using a "lock-directory" as
   described in
   http://stackoverflow.com/questions/185451/quick-and-dirty-way-to-ensure-only-one-instance-of-a-shell-script-is-running-at
   which comes from
   http://mywiki.wooledge.org/BashFAQ/045
   via the following operation in the read-and-update-script:
   \verbatim
while ! mkdir Lock 2>/dev/null; do
  sleep 1s
done
   \endverbatim
   It should happen that every such read-and-update-process succeeds after
   some time. If not after, say, 10s, then it gives up and the processing-
   process is stopped. </li>
   <li> Perhaps using "lockfile" is even better. </li>
  <ul>

*/
