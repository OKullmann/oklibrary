// Oliver Kullmann, 13.9.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2011, 2012, 2013 Oliver Kullmann
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

  <ul>
   <li> See the report "Investigating the behaviour of a SAT solver on random
   formulas" available at
   http://cs.swan.ac.uk/~csoliver/papers.html#OKsolverReport2002 .</li>
   <li> Procedural specifications at the Lisp/Maxima level are given in
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/OKsolver2002.mac. </li>
   <li> A general theoretical underpinning is given in the SAT Handbook article
   " Fundaments of Branching Heuristics" available at
   http://cs.swan.ac.uk/~csoliver/papers.html#Handbook2009Fundaments . </li>
  </ul>


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
   <code>BELEGUNG</code>.) Compare switch "-O" below. </li>
   <li> If <code>OUTPUTTREEDATAXML</code> is defined, then the search tree
   is output into a file, using a simple XML structure and adorning each
   %node with some statistics.
    <ol>
     <li> The output is stored in the file "filename.xml", where filename is
     the (full) name of the input file. </li>
     <li> The embedded DTD is
     \verbatim
<!DOCTYPE t [
  <!ELEMENT t (t?, t?)>
  <!ATTLIST t
    l NMTOKEN #REQUIRED>
]>
     \endverbatim
     </li>
     <li> The attribute-value for "l" is the number of r_2-reductions at the
     node. </li>
     <li> An example
     \verbatim
> OKplatform/system_directories/bin/OKsolver_2002_OUTPUTXML-O3-DNDEBUG S_EasterMonster.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         9
c initial_number_of_variables           729
c initial_number_of_clauses             10551
c initial_number_of_literal_occurrences 23349
c number_of_initial_unit-eliminations   490
c reddiff_maximal_clause_length         3
c reddiff_number_of_variables           490
c reddiff_number_of_clauses             8984
c reddiff_number_of_literal_occurrences 19739
c number_of_2-clauses_after_reduction   1343
c running_time(sec)                     0.0
c number_of_nodes                       9
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                100
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        3
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             S_EasterMonster.cnf

> cat S_EasterMonster.cnf.xml
<?xml version="1.0" standalone="yes" ?>
<!DOCTYPE t [
  <!ELEMENT t (t?, t?)>
  <!ATTLIST t
    l NMTOKEN #REQUIRED>
]>

<t l="0"><t l="7"><t l="9"></t>
<t l="25"></t>
</t>
<t l="9"><t l="12"></t>
<t l="4"><t l="10"></t>
<t l="24"></t>
</t>
</t>
</t>
     \endverbatim
     </li>
     <li> This tree as a labelled (binary) Maxima-tree is (using lists
     of values as labels (here of length 1)):
     \verbatim
T :
  [ [0],
  [ [7],
    [[9]],
    [[25]]
  ],
  [ [9],
    [[12]],
    [ [4],
      [[10]],
      [[24]]
    ]
  ]
]$
     \endverbatim
     This Maxima representation is computed by "Xml2Maxima":
     \verbatim
> Xml2Maxima S_EasterMonster.cnf
> cat S_EasterMonster.cnf.mac
T_oklib :
[[0],[[7],[[9]],[[25]]],[[9],[[12]],[[4],[[10]],[[24]]]]];
     \endverbatim
     Draw with draw_rt(lrt2rt(T_oklib),[]) (ignoring the labels). </li>
    </ol>
   </li>
   <li> If <code>SYSTIME</code> is defined, then instead of the standard time
   measurement from the C-library the Unix/Linux time measurement from
   <code>sys/times.h</code> is used.
    <ul>
     <li> This enables to measure longer times on 32-bit machines, and thus
     this is the default on 32-bit machines. </li>
     <li> While otherwise the standard time measurement is sufficient, and
     so <code>SYSTIME</code> is not defined here. </li>
     <li> To define SYSTIME, pass the option
     \verbatim
CFLAGS="-DSYSTIME"
     \endverbatim
     to the oklib call. </li>
     <li> If on the other hand on a 32-bit machine this system time should
     <em>not</em> be used, then pass the option
     \verbatim
CFLAGS="-UMACHINE_BITS_OKL"
     \endverbatim
     to the oklib call (this makes temporary the machine a non-32-bit
     machine). </li>
    </ul>
   </li>
   <li> NOT IMPLEMENTED YET If <code>ALLSAT</code> is defined, then all
   satisfying assignments are found. Currently combination with
   <code>BAUMRES</code> or <code>ASSIGNMENT</code> is not possible (and thus
   yet we just count all satisfying assignments).
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
   <li> <code>OKsolver_2002_NTP</code> (no tree pruning; link provided
   in <code>$(public_bin_dir_okl)</code>) </li>
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
   </li>
   <li> Output formats:
    <ol>
     <li> <code>-DO</code> for printing the output in Dimacs format (this
     is the default).
      <ul>
       <li> Here the possible results "SATISFIABLE, UNSATISFIABLE, UNKNOWN"
       are in the first line of the output, which starts with "s ". </li>
       <li> Followed by comment-lines (which start with "c ") with statistics.
       </li>
       <li> Finally, if output of satisfying assignments is activated (see
       switch "-O" below), then in one line, starting with "v ", the list of
       literals set to true is output (separated by spaces), concluded by
       " 0" (as with clauses). </li>
      </ul>
     </li>
     <li> <code>-XO</code> for printing the output in XML format. </li>
    </ol>
   </li>
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
       <li> Once a %node at level d is completed (called a "monitoring
       node"), its number is printed out, followed by
        <ol>
         <li> the number of nodes created after the last monitoring node
         has been completed, </li>
         <li> the current average of the number of nodes created per
         monitoring node, </li>
         <li> the predicted total number of nodes, </li>
         <li> the (processor) time it took to process the current monitoring
         %node, </li>
         <li> the current average time it took to process a monitoring %node,
         </li>
         <li> the predicted remaining running time (which is just the
         current average time multiplied with the number of remaining
         monitoring nodes), </li>
         <li> the number of nodes realised (after the last monitoring node)
         to have only one child due to tree-pruning, </li>
         <li> the number of autarkies (after the last monitoring node), </li>
         <li> finally the total tree depth. </li>
        </ol>
       </li>
       <li> If file-output is activated (via "-F"):
        <ol>
         <li> The monitoring output is echoed to file "FullInputFileName.mo".
         </li>
         <li> The output to the file is more machine-readable, and the
         predictions are left out since they are easily computable. </li>
         <li> As an additional output one has the average number of
         2-reductions ("failed literals") since the last monitoring node.
         </li>
         <li> Also output of branching literals for the monitoring levels
         up to 2 levels before the monitoring level is activated (only
         to the file). </li>
         <li> This output consists of three numbers, first the level, then
         the name of the variable, and then the first truth value to be
         visited by the solver. </li>
         <li> However, in order for this to work, currently also output of
         satisfying partial assignments (if found) is to be activated (via
         "-O"). </li>
        </ol>
       </li>
      </ul>
     </li>
     <li> <code>-P</code> for only performing preprocessing (cleaning of input
     and unit-clause-propagation). Some applications:
      <ul>
       <li> By
       \verbatim
> OKsolver_2002-O3-DNDEBUG -P Filename | awk '$2 == "initial_number_of_variables" {print $3}'
       \endverbatim
       the (precise) number of (occurring) variables is computed (without
       any reduction). </li>
       <li> And by
       \verbatim
> OKsolver_2002-O3-DNDEBUG -P Filename | awk '$2 == "initial_number_of_clauses" {print $3}'
       \endverbatim
       the (precise) number of clauses is computed (without any reduction).
       </li>
      </ul>
     </li>
     <li> <code>-R</code> for the special DIMACS return values (default is ON
     in case of DIMACS output, and OFF otherwise). </li>
     <li> <code>-O</code> for output of a satisfying assignment (if found;
     default is OFF, however if macro ASSIGNMENT is set (see above) then the
     default is ON). </li>
     <li> <code>-SF</code> for saving the partial assignments for the splitting
     instances created by "-S" (see below) in a (single) file; default is OFF,
     storing the partial assignments each in its own file.
     </li>
    </ul>
   </li>
   <li> <code>-S=directory</code> resp. <code>-S=file</code>:
    <ol>
     <li> Whether the directory- or the file-form is active depends on "-SF"
     (see above). </li>
     <li> The directory must already exist and must be writable. </li>
     <li> The file will be created if needed. </li>
     <li> Uses the depth-parameter "-D=d" as above for monitoring. </li>
     <li> Stores the partial assignments leading to
     the (reduced) nodes with at least d assignments (decisions and enforced)
     of the splitting tree, from left to right. </li>
     <li> The main usage is for splitting a hard problem into (at most) 2^d
     many subproblems; see below for helper scripts. </li>
     <li> The partial assignments yielding the sub-problems are stored either
     in files 1, ... in <code>directory</code>, or line by line in the
     specified file. </li>
     <li> Additionally the decisions variables for each splitting are output:
      <ol>
       <li> Always in a (single) file, either in file <code>decisions</code>
       in the directory, or in the file <code>filename_decisions</code>. </li>
       <li> The format is one line per splitting, first the number of
       decisions, and then the 0-based indices of the decisions in the
       corresponding partial assignment. </li>
      </ol>
     </li>
     <li> The DIMACS format for partial assignments is used (see above). </li>
     <li> Via the switch "-SD" the interpretation of d is changed:
      <ol>
       <li> The criterion for aborting the development of the splitting tree
       is changed. </li>
       <li> After the switch, the depth of the node is considered; this is
       equal to the number of decision variables on the path to the node. </li>
       <li> The default is to consider the size n of the partial assignment
       leading to the node; so also inferred assignments and autarky
       assignments are taken into account. </li>
       <li> The default is the "n-interpretation", and the switch changes it
       to the "depth-interpretation". </li>
       <li> As usual with such switches, another application switches it back
       to the default value, and so on. </li>
      </ol>
     </li>
     <li> A special case is "-D0": Here now in file <code>directory/1</code>
     (or in the single line of the output-file) one finds the partial
     assignment for the fully reduced input --- not just cleaning-up and
     unit-clause-propagation (as "-P" above), but also r_2-reduction and the
     associated autarky-reduction. </li>
    </ol>
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
     <li> "Reduction" refers to
      <ol>
       <li> elimination of tautological clauses and repeated literal
       occurrences </li>
       <li> unit-clause-propagation. </li>
      </ol>
     </li>
     <li> So <code>initial_number_of_variables</code> is the precise number
     of variables in the input (taking also tautological clauses into
     account), while <code>initial_number_of_clauses</code> is the precise
     number of clauses in the input (again, taking also tautological clauses
     into account). </li>
     <li> The "reddiff"-information specifies the difference achieved by the
     reduction:
      <ol>
       <li> <code>reddiff_maximal_clause_length</code>: how much the maximal
       clause-length was decreased by the reduction. </li>
       <li> <code>reddiff_number_of_variables</code>: how many variables
       were eliminated by the reduction. </li>
       <li> <code>reddiff_number_of_clauses</code>: how many clauses
       were eliminated by the reduction. </li>
       <li> <code>reddiff_number_of_literal_occurrences</code>: how many
       literal occurrences were eliminated by the reduction. </li>
      </ol>
     </li>
     <li> The "proportion"-information refers to the proportion of the
     2^n (total) assignments which has been covered by the splitting-tree:
      <ol>
       <li> <code>proportion_searched</code> refers to the proportion without
       the contribution of tree-pruning. </li>
       <li> <code>proportion_single</code> refers to the proportion due to
       the contribution of tree-pruning alone. </li>
       <li> <code>total_proportion</code> finally is the sum (equal to 1
       for unsatisfiable inputs). </li>
      </ol>
     </li>
     <li> In case the "-S" option is used, we have the
     "splitting"-information:
      <ol>
       <li> <code>splitting_directory</code> is the directory, or file
       if "-SF" is used, containing the splitting data. </li>
       <li> <code>splitting_cases</code> is the number of sub-instances
       generated by the splitting. </li>
      </ol>
     </li>
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


  <h2> Splitting into sub-problems </h2>

  <ul>
   <li> For hard problems we have the script
   <code>SplittingViaOKsolver</code>. </li>
   <li> This is a script for calling the OKsolver with the option "-S"; see
   above for some details of this option. </li>
   <li> As explained above, this uses the depth-option "-D=d" as above, however
   not for monitoring, but for outputting the sub-instances of the splitting
   at depth d. </li>
   <li> The parameters of <code>SplittingViaOKsolver</code> are passed over to
   the OKsolver, adding the parameter "-S=dir" with appropriate value for dir,
   the splitting-directory. </li>
   <li> The script creates the splitting-directory, calls the OKsolver, and
   stores all data related to this computation in files in this directory.
   </li>
   <li> See the documentation of <code>SplittingViaOKsolver</code>. </li>
   <li> To process the created splitting-directory, use
   <code>ProcessSplitViaOKsolver</code> (see the documentation). </li>
   <li> To extract the spliting-information as an iCNF-file, use
   <code>ExtractiCNF</code> (see the documentation). </li>
   <li> If for splitting only the decision-assignments shall be used, use
   <code>ExtractDecisionsiCNF</code> (see the documentation). </li>
   <li> For processing an iCNF-file, we have <code>ProcessiCNF</code> (see the
   documentation). Note that compared to <code>ProcessSplitViaOKsolver</code>
   this has less information available. </li>
   <li> In order to extract the decision-assignments into a DNF (which yields
   an unsatisfiable tree-hitting clause-set) we have the application
   <code>CollectingDecisions</code> (see the documentation). </li>
  </ul>

*/

