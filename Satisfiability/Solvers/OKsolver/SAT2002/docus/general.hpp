// Oliver Kullmann, 13.9.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/docus/general.hpp
  \brief General documentation on the original OKsolver

  <h2> Macro options </h2>

  The most important macro options (that is, compile time options):
  <ul>
   <li> If <code>NBAUMRES</code> is defined, then tree resolution pruning
   ("intelligent backtracking") is turned off, while otherwise (the default)
   it is turned on (that is, <code>BAUMRES</code> gets defined).
    <ul>
     <li> If <code>NLITTAB</code> is defined, then the more space efficient
     (but less time time efficient) special datastructure is not used, while
     otherwise (the default) it is used (that is, <code>LITTAB</code> gets
     defined). </li>
    </ul>
   </li>
   <li> If <code>ASSIGNMENT</code> is defined, then if a satisfying assignment
   is found, it is output, while otherwise only the satisfiability status
   is returned. (Internally, <code>ASSIGNMENT</code> is translated yet into
   <code>BELEGUNG</code>.) </li>
   <li> If <code>OUTPUTTREEDATAXML</code> is defined, then the search tree
   is output into a file, using a simple XML structure and adorning each
   node with some statistics. </li>
   <li> NOT IMPLEMENTED YET If <code>ALLSAT</code> is defined, then all satisfying assignments
   are found. Currently combination with <code>BAUMRES</code> or
   <code>ASSIGNMENT</code> is not possible (and thus yet we just
   count all satisfying assignments.
    <ul>
     <li> <code>NSAT_BITS</code> specifies the number of bits for the unsigned
     int value of the number of satisfying assignments. The default is 64,
     and it can set to any value supported by your platform. Overflows
     are not detected, and the number of satisfying assignments is
     thus only correct modulo 2^<code>NSAT_BITS</code> (however whether
     the value is 0 or not is correctly reported). </li>
    </ul>
   </li>
  </ul>


  <h2> Precompiled programs </h2>

  The optimised version of a program has the optimisation options
  in the name. The following programs are created (in both versions,
  unoptimised and optimised), as usual in <code>$(bin_dir)</code>:
  <ol>
   <li> <code>OKsolver_2002</code> </li>
   <li> <code>OKsolver_2002_NTP</code> (no tree pruning) </li>
   <li> <code>OKsolver_2002_NLT</code> (tree pruning needs more space, but is faster) </li>
   <li> <code>OKsolver_2002_osa</code> (outputs satisfying assignments) </li>
   <li> <code>OKsolver_2002_NTP_osa</code> </li>
   <li> <code>OKsolver_2002_NLT_osa</code> </li>
  </ol>


  <h2> Command-line options </h2>


  <h2> Signals </h2>

  Given the process identity <code>id</code>, a signal called "SIGNAL" is sent to the process
  via <code>kill -s SIGNAL id</code>.
  <ul>
   <li> Sending the signal <code>SIGUSR1</code> to the OKsolver causes the printout
   of the current statistics. </li>
   <li> Sending the signal <code>SIGUSR2</code> to the OKsolver aborts the computation after
   the same printout as with <code>SIGUSR1</code>. </li>
  </ul>

*/

