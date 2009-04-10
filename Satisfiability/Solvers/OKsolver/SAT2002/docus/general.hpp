// Oliver Kullmann, 13.9.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/docus/general.hpp
  \brief General documentation on the original OKsolver


  <h1> The OKsolver from 2002 </h1>

  This first version of the OKsolver is made accessible here.
  Since it has a rather canonical "semantics", it can serve
  as a kind of standard implementation of
  <ul>
   <li> a look-ahead solver </li>
   <li> who employs full failed-literal-reduction ("r_2") </li>
   <li> and full unit-clause-propagation ("r_1") look-ahead. </li>
  </ul>
  The heuristics is given by (weighted) counting of new clauses,
  and here naturally basic autarky reduction (generalising
  pure-literal-elimination) is utilised.


  <h2> Specification </h2>

  Procedural specifications at the Lisp/Maxima level are given in
  ComputerAlgebra/Satisfiability/Lisp/Backtracking/OKsolver2002.mac.


  <h2> Macro options </h2>

  The most important macro options (that is, compile time options):
  <ul>
   <li> If <code>NBAUMRES</code> is defined, then tree resolution pruning
   ("intelligent backtracking") is turned off, while otherwise (the default)
   it is turned on (that is, <code>BAUMRES</code> gets defined).
    <ul>
     <li> If <code>NLITTAB</code> is defined, then the more space efficient
     (but less time time efficient) special datastructure is not used. </li>
     <li> Otherwise (the default) it is used (that is,
     <code>LITTAB</code> gets defined). </li>
    </ul>
   </li>
   <li> If <code>ASSIGNMENT</code> is defined, then if a satisfying assignment
   is found, it is output, while otherwise only the satisfiability status
   is returned. (Internally, <code>ASSIGNMENT</code> is translated yet into
   <code>BELEGUNG</code>.) </li>
   <li> If <code>OUTPUTTREEDATAXML</code> is defined, then the search tree
   is output into a file, using a simple XML structure and adorning each
   node with some statistics. </li>
   <li> NOT IMPLEMENTED YET If <code>ALLSAT</code> is defined, then all
   satisfying assignments are found. Currently combination with
   <code>BAUMRES</code> or <code>ASSIGNMENT</code> is not possible (and thus
   yet we just count all satisfying assignments.
    <ul>
     <li> <code>NSAT_BITS</code> specifies the number of bits for the unsigned
     int value of the number of satisfying assignments. </li>
     <li> The default is 64, and it can be set to any value supported by your
     platform. </li>
     <li> Overflows are not detected, and the number of satisfying
     assignments is thus only correct modulo 2^<code>NSAT_BITS</code>
     (however whether the value is 0 or not is correctly reported). </li>
    </ul>
   </li>
  </ul>


  <h2> Precompiled programs </h2>

  The optimised version of a program has the optimisation options
  in the name. The following programs are created (in both versions,
  unoptimised and optimised), as usual in <code>$(bin_dir)</code>:
  <ol>
   <li> <code>OKsolver_2002</code> (the default version; a link is also
   provided in the public bin-directory <code>$(public_bin_dir_okl)</code>)
   </li>
   <li> <code>OKsolver_2002_NTP</code> (no tree pruning) </li>
   <li> <code>OKsolver_2002_NLT</code> (tree pruning needs more space, but is
   faster) </li>
   <li> <code>OKsolver_2002_osa</code> (outputs satisfying assignments) </li>
   <li> <code>OKsolver_2002_NTP_osa</code> </li>
   <li> <code>OKsolver_2002_NLT_osa</code> </li>
  </ol>


  <h2> Basic usage </h2>

  <ul>
   <li> Call the OKsolver with one parameter, the filename containing the CNF
   in DIMACS format. </li>
   <li> The OKsolver accepts also arbitrary non-empty sequences of digits and
   letters for variable-names, with the exception that in DIMACS format a first
   character must not be "0", "p" or "c". </li>
   <li> By default satisfying assignments are not output, but this can be
   turned on, and also all output can be printed into a file. </li>
   <li> A monitoring level of the search tree can be established, such that
   finished nodes at this level are reported, and predicated running times are
   computed. </li>
  </ul>


  <h2> Command-line options </h2>

  The solver reads the arguments in order, processes each and exits.

  <ul>
   <li> Without any argument given, just a short instruction is printed. </li>
   <li> Option <code>--help</code> creates a short help message. </li>
   <li> <code>--version</code> prints version information. </li>
   <li> <code>--author</code> prints information about the author. </li>
   <li> <code>--info</code> shows how to interpret the (standard) output. </li>
   <li> <code>--specification</code> prints a solver specification in XML. </li>
   <li> <code>--language=x</code> switches the output-language; supported
   values are x=0 for German and x=1 for English (the default). </li>
   <li> <code>--timeout=n</code> for a time-out in n seconds (wall-clock),
   where n=0 means no time-out. </li>
   <li> <code>--standard=x</code> switches the input-format; supported values
   are
    <ol>
     <li> x=1 for DIMACS (the default; as always, "extended", allowing names
     for variables). </li>
     <li> x=2 uses "%" for beginning a comment (anywhere), "(" for opening a
     clause, ")" for closing a clause, and literals are separated by a comma.
     </li>
     <li> x=3 as x=2 but (arbitrary) space symbols separate literals. </li>
     <li> x=4 as x=3, but no clause-begin, and line-end finishes a clause.
     </li>
    </ol>
    The restrictions for variable-names (not beginning with "0,c,p") only hold
    for DIMACS (x=1). DIMACS does not accept an empty clause, while the other
    formats do. </li>
    <li> Toggles (switching between "ON" and "OFF"):
     <ul>
      <li> <code>-F</code> for printing the results also to files (default is
      OFF). </li>
      <li> <code>-M</code> for monitoring the level d of the backtracking tree
      (default is OFF), where d=6 by default (watching 2^6 = 64 nodes).
       <ul>
        <li> d can be set using<code>-Dd</code>, for example
        <code>-D10</code> for watching 2^10 = 1024 nodes at depth 10.
        Range 0 <= d <= 30. </li>
        <li> Once a node at level d is completed, its number is printed out,
        followed by the number of leaves below it, the total running-time
        until now, and the anticipated running time for the remaining nodes at
        this level, using the current average running time for the monitored
        nodes (always in seconds). </li>
        <li> If file-output is activated, then the monitoring output is echoed
        to file "FullInputFileName.mo". </li>
       </ul>
      </li>
      <li> <code>-P</code> for only performing preprocessing (cleaning of input
      and unit-clause-propagation). Some applications:
       <ul>
        <li> By
        \verbatim
> OKsolver_2002-O3-DNDEBUG -P Filename | awk '$1 == "c" {print substr($4,29)}'
        \endverbatim
        the (precise) number of (occurring) variables is computed (without
        any reduction). </li>
        <li> And by
        \verbatim
> OKsolver_2002-O3-DNDEBUG -P Filename | awk '$1 == "c" {print substr($5,27)}'
        \endverbatim
        the (precise) number of clauses is computed (without any reduction).
        </li>
       </ul>
      </li>
      <li> <code>-R</code> for the special DIMACS return values (default is ON
      in case of DIMACS output, and OFF otherwise). </li>
     </ul>
    </li>
    <li> To be completed. </li>
  </ul>


  <h2> Signals </h2>

  Given the process identity <code>id</code>, a signal called "SIGNAL" is sent
  to the process via <code>kill -s SIGNAL id</code>.
  <ul>
   <li> Sending the signal <code>SIGUSR1</code> to the OKsolver causes the
   printout of the current statistics. </li>
   <li> Sending the signal <code>SIGINT</code> to the OKsolver aborts the
   computation after printing the statistics. This signal is send to a program
   when pressing <code>CTRL C</code> on the command line. </li>
  </ul>


  <h2> Output </h2>

  <ul>
   <li> Explanation of the output for the (default) DIMACS output:
    <ol>
     <li> The number of variables is always (before and after reduction) the
     number of variables actually occurring in the clause-set. </li>
     <li> And also the number of clauses is the precise count (before and
     after reduction). </li>
     <li> "reduction" refers to elimination of tautological clauses,
     repeated literal occurrences and unit-clause-propagation. </li>
     <li> So <code>initial_number_of_variables</code> is the precise number
     of variables in the input (taking also tautological clauses into
     account), while <code>initial_number_of_clauses</code> is the precise
     number of clauses in the input (again, taking also tautological clauses
     into account). </li>
     <li> <code>reduced_maximal_clause_length</code>: how much the maximal
     clause-length was decreased by the reduction. </li>
     <li> <code>reduced_number_of_variables</code>: How many variables
     where eliminated by the reduction. </li>
     <li> <code>reduced_number_of_clauses</code>: How many clauses
     where eliminated by the reduction. </li>
     <li> <code>reduced_number_of_literal_occurrences</code>: How many literal
     occurrences where eliminated by the reduction. </li>
     <li> <code>initial_number_of_2-clauses</code>: Number of 2-clauses
     after reduction. </li>
    </ol>
   </li>
   </li> The return values for the DIMACS output:
    <ol>
     <li> <code>10</code> for "SAT" </li>
     <li> <code>20</code> for "UNSAT" </li>
     <li> <code>0</code> for "unknown". </li>
    </ol>
    For non-DIMACS output the return value is <code>0</code> for all cases of
    proper exit. </li>
   </li>
   <li> 
  </ul>


  <h2> Combination with pre-processors </h2>

  <ul>
   <li> Combination with the Minisat2 pre-processor:
    <ol>
     <li> This combination seems often quite successful, and is provided by the
     wrapper script <code>OKsolver_2002-m2pp</code>. </li>
     <li> Same parameters as the OKsolver, while creating a temporary file
     containing the preprocessed file. </li>
     <li> Two additional outputs are provided before calling the OKsolver:
     Statistics on the original input, and the output of the Minisat2
     preprocessor. </li>
    </ol>
  </ul>

*/

