// Matthew Henderson, 25.1.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/general.hpp
  \brief Plans for the module on SAT-related input/output


  \todo Tests for error cases in scripts
  <ul>
   <li> For each of the scripts in this directory, we should
   test that the scripts error under the conditions we expect them
   to. </li>
   <li> For instance, if we provide AppendDimacs with a clause-set
   which lists the number of variables of the p-line as less than
   the actual number of variables in the clause-set, then we should
   get an error such as:
   \verbatim
terminate called after throwing an instance of 'OKlib::InputOutput::ClauseInputError'
  what():  OKlib::InputOutput::StandardDIMACSInput::read_clauses:
  literal 2 has variable index larger than the specified upper bound 1
line 1, column 4, total characters read 14
Aborted
   \endverbatim
   </li>
   <li> To test the errors, we should extend the application tests with
   a "error_output" directory, which stores templates to match against
   the error output for each test. </li>
   <li> Then standard output from each program is checked against
   templates in dimacs_output, and error output is checked against
   error_output. </li>
  </ul>


  \todo Tests for RenameDimacs
  <ul>
   <li> We need unit-tests and application tests for RenameDimacs. </li>
  </ul>


  \todo Update output for application tests
  <ul>
   <li> Application tests should print:
    <ul>
     <li> which script is currently running;
     <li> what is matched against what by the experiment script (templates);
     </li>
    </ul>
   </li>
   <li> The indentation and spacing of the application tests should also
   make it easy to discern which block of text refers to which application
   test, and to which portion of that test's output. </li>
   <li> The following application tests need to be updated:
    <ul>
     <li> ManipParam. </li>
     <li> MinOnes2PseudoBoolean. </li>
     <li> MinOnes2WeightedMaxSAT. </li>
     <li> RandomShuffleDimacs. </li>
     <li> SortByClauseLength. </li>
    </ul>
   </li>
   <li> See application tests for AppendDimacs. </li>
  </ul>


  \todo Strange tool statistics_add_missing_clause_lengths.awk
  <ul>
   <li> First, it needs at least the usual safety-settings. </li>
   <li> And even for such tiny programs, the usual coding standards must be
   adhered to. </li>
   <li> Then, what is the motivation for this? </li>
   <li> Likely, it should be removed. </li>
   <li> Cluttering the OKlibrary with such strange hacks should be avoided.
   </li>
  </ul>


  \todo Comprehensive statistics
  <ul>
   <li> We need urgently a tool which computes "all" statistics. </li>
   <li> See "Comprehensive statistics" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Statistics.hpp
   for the Maxima role-model. </li>
   <li> Since at the C++-level we have larger inputs, we need some easy method
   to specify the statistics which are computed. </li>
   <li> Perhaps the application reads a configuration file, which contains
   identifiers for all the statistics to be computed. </li>
   <li> We provide standard configuration files for all statistics, and
   statistics for medium and for large instances. </li>
   <li> For that we must establish same convention. </li>
  </ul>


  \todo Elementary file-surgery
  <ul>
   <li> For adapting a single Dimacs-file we have
   InputOutput/ExtendedToStrictDimacs.cpp (see
   Interfaces/InputOutput/plans/ExtendedToStrictDimacs.hpp). </li>
   <li> DONE (wrote C++ application - see AppendDimacs.cpp)
   Appending files:
    <ol>
     <li> We need a script
     \verbatim
AppendDimacs file_1 ... file_n
     \endverbatim
     which creates a new comment "Append current-date file_1 ... file_n:",
     followed by the comments from the files in the given order (each with
     heading "File filename"), then the parameter line "p n c", where n is the
     maximum of the n-values of file_i, while c is the sum of c-values of
     file_i, and finally the clauses from file_1, ..., file_n (of course,
     without the comment- and parameter-lines). </li>
     <li> An awk-script should do the job (we just need to filter lines, and
     to compute the sum and the maximum). </li>
     <li> Two passes over all files are used, but for the first pass each
     file-reading is aborted as soon as the parameter-line was read. </li>
     <li> So the first pass just computes the new n and c, and copies the
     comments to standard output. </li>
     <li> In the second pass then first the new parameter-line is output, and
     then all lines after the parameter-line in files file_1, ..., file_n are
     copied to the output. </li>
     <li> There is also the possibility, that some of the file_i do not contain
     a parameter-line, in which case the maximum n and the clause-count c
     for this file is also to be computed in the first pass. </li>
     <li> Computing c can just be done by counting the number of occurrences
     of " 0", while for computing n the maximum of the absolute values of
     numbers read is to be computed. </li>
    </ol>
   </li>
   <li> Perhaps adding a single clause is worth a dedicated tool. </li>
  </ul>


  \todo Rewrite code for the new namespaces.


  \todo Update
  <ul>
   <li> Get an overview on the current state of the module, create new
   plans-files for the sub-modules, move the todo's to there, update
   the doxygen-documentation. </li>
   <li> Especially docus and example-applications are needed! </li>
   <li> Files involved:
    <ol>
     <li> InputOutput/ClauseSetAdaptors.hpp : </li>
     <li> InputOutput/Exceptions.hpp : </li>
     <li> InputOutput/ExtendedDimacsStatistics.cpp : </li>
     <li> InputOutput/ExtendedToStrictDimacs.cpp : DONE </li>
     <li> InputOutput/Dimacs.hpp : DONE </li>
    </ol>
   </li>
   <li> Update milestones. </li>
  </ul>


  \todo See OKlib/Satisfiability/ProofSystems/DPv/Input_output.hpp


  \todo Write docus
  <ul>
   <li> First task: How to create Dimacs-output.
    <ol>
     <li> The class template InputOutput::CLSAdaptorDIMACSOutput, a model of
     concept CLSAdaptor, is the main component. </li>
     <li> The idea of such a CLSAdaptor is, that it provides event-handlers
     for registering clauses etc., and delivers the collected information
     to another component, in edited form. </li>
     <li> If the clause-set F to be output is given as a sequence of
     sequences, then class template InputOutput::ListTransfer is the component
     to be plugged into a CLSAdaptor; in this case, where we just want to
     output it, this is CLSAdaptorDIMACSOutput. </li>
     <li> As a convenience instantiation, InputOutput::List2DIMACSOutput is
     provided, which can be used by
     \code
InputOutput::List2DIMACSOutput(F, out, comment)
     \endcode
     for an ostream "out" and an optional string "comment". </li>
    </ol>
   </li>
   <li> Interfaces/InputOutput/ClauseSetAdaptors.hpp provides various
   adaptors, for transfer into some data-structure, or directly to output,
   or just counting. </li>
   <li> While Interfaces/InputOutput/Dimacs.hpp yields basically components
   which take a CLS-adaptor as template parameter, and supply it with the
   input from some source in DIMACS format. </li>
  </ul>


  \todo Move to the new test system.


  \todo Complete the tests.


  \todo Write demos.


  \todo Complete the functionality.


  \todo Input and output of propositional formulas
  <ul>
   <li> Formats:
    <ol>
     <li> Edimacs format: http://www.satcompetition.org/2005/edimacs.pdf </li>
     <li> ISCAS ??? </li>
     <li> AIGER http://fmv.jku.at/aiger/ ? </li>
     <li> Maxima terms
      <ol>
       <li> See
       ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/Formulas.hpp
       for the representation of propositional logic at Maxima/Lisp level.
       </li>
       <li> Writing these terms to files such that Maxima can directly read
       them yields a system for representing propositional formulas. </li>
      </ol>
     </li>
     <li> Strictly bracketed "~,&,|,<->,->"-formulas
      <ul>
       <li> Identifiers use letters a-z,A_Z, digits 0-9 and
       underscores. </li>
       <li> Binary operators "&,|,<->,->" have at least one space
       left and right, and terms are enclosed in brackets: "(a op b)". </li>
       <li> Negation terms don't need brackets, i.e., they have the form
       "~Term" (possibly with space symbols after "~"), where Term is either
       "Identifier" or "(a op b)". </li>
       <li> Constants are "true" and "false". </li>
       <li> Except of these rules, space symbols can be used arbitrarily. </li>
       <li> And also additional brackets (well-formed) can be used arbitrarily.
       </li>
      </ul>
     </li>
     <li> See "BDD's and boolean functions" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp. </li>
    </ol>
   </li>
   <li> Benchmark collections
    <ol>
     <li> SAT 2005 ? </li>
     <li> In [Jain, Bartzis, Clarke; SAT 2006] some benchmarks are mentioned.
     </li>
    </ol>
   </li>   
  </ul>


  \todo Circuits
  <ul>
   <li> What systems exist to represent boolean circuits (and more general
   forms)? </li>
   <li> Likely they are similar to "Straight-line programs" as discussed in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp. </li>
  </ul>


  \bug DONE (updated tests; added todo for error cases)
  Can not update applications tests for AppendDimacs
  <ul>
   <li> DONE (see "Tests for error cases in scripts")
   Test-output, also in the error case, should not be that large. </li>
   <li> DONE (updated formatting on AppendDimacs application test output)
   The output can not be easily inspected to see what is the case, and
   what has to be changed. </li>
   <li> DONE After improving the test (just removing this irrelevant case, and
   replacing it with various cases which test the various error conditions!),
   the tests need to be updated. </li>
   <li> DONE We need small, easily readable application test data which covers
   the following specified criteria for AppendDimacs:
    <ul>
     <li> DONE (see all application tests; only testing up to 3)
     AppendDimacs takes from 0 arguments up to ARG_MAX. </li>
     <li> DONE (see
     app_tests/test_cases/basic/AppendDimacs/input/FormattingAndReductions)
     The clauses from the input clause-sets are output in
     the order they are given (as arguments, and in the clause-lists). </li>
     <li> DONE (see
     app_tests/test_cases/basic/AppendDimacs/input/FormattingAndReductions)
     The comments from the input clause-sets are output in
     the order they are given. </li>
     <li> DONE (see
     app_tests/test_cases/basic/AppendDimacs/input/FormattingAndReductions)
     All formatting is lost. </li>
     <li> DONE (see
     app_tests/test_cases/basic/AppendDimacs/input/FormattingAndReductions)
     Tautological clauses are removed. </li>
     <li> DONE (see
     app_tests/test_cases/basic/AppendDimacs/input/FormattingAndReductions)
     Clauses are output in numerical order of the literals. </li>
     <li> DONE (see
     app_tests/test_cases/basic/AppendDimacs/input/FormattingAndReductions)
     Repeated literals are removed. </li>
     <li> DONE (see
     app_tests/test_cases/basic/AppendDimacs/input/IncorrectVarClauseNumbersN)
     The number of variables in the new p-line is the
     maximum of all variable-indices across the input
     clause-sets after removal of tautological clauses
     and repeated literals. </li>
     <li> DONE (see
     app_tests/test_cases/basic/AppendDimacs/input/IncorrectVarClauseNumbersN)
     The number of clauses in the new p-line is
     the number of clauses result of appending the argument
     clause-lists, and removing tautological clauses and
     repeated literals. </li>
     <li> DONE (See app_tests/test_cases/basic/AppendDimacs/input/EmptyN and
     app_tests/test_cases/basic/AppendDimacs/input/EmptyClause1)
     Testing extreme cases, empty clause-set and presense of the
     empty-clause. </li>
    </ul>
   </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Interfaces::InputOutput
  \brief Components for SAT-related input/output

  Alias "InO".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Interfaces {
      namespace InputOutput {
      }
      namespace InO = InputOutput;
    }
  }
}

