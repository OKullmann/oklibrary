// Matthew Henderson, 25.1.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/general.hpp
  \brief Plans for the module on SAT-related input/output.


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


  \todo Improve application InputOutput/ExtendedToStrictDimacs.cpp
  <ul>
   <li> The new parameter-values should be the correct (i.e., precise) ones,
   not the original ones.
    <ol>
     <li> It is an option whether the original parameter values should be
     checked for errors, or just completely ignored. </li>
     <li> Perhaps the current behaviour, just taking over the original
     values, could also be an option. </li>
    </ol>
   </li>
   <li> We should add a comment-line (optionally) for the original
   file-name, for the original parameter values, and if some cleaning
   happened (tautological clauses or repeated literals), then these
   numbers are also stated. </li>
   <li> Really handle extended Dimacs format, using a policy for
   OKlib::InputOutput::StandardDIMACSInput.
    <ol>
     <li> We should have an option for adding to the comment-section
     the map index -> original name. </li>
     <li> This map should be produced by the CLSAdaptor, and a standard
     format is needed, so that it can be extracted by some tool. </li>
     <li> Should we have a command-line option to turn on extended Dimacs
     format? Seems useless: turn it always on (this can handle also standard
     Dimacs input). DONE </li>
    </ol>
   </li>
   <li> Complete doxygen-documentation. </li>
   <li> Write docus:
    <ol>
     <li> The output is cleaned of tautological clauses and repeated
     literals, and the output of clauses is sorted according to natural
     order for integers. </li>
     <li> Mention the public link. </li>
    </ol>
   </li>
   <li> Use Messages. </li>
   <li> Use ProgramOptions. </li>
   <li> Move InputOutput/ExtendedToStrictDimacs.cpp to sub-directory
   applications, once buildsystem is ready; update then public link. </li>
  </ul>


  \todo Complete the functionality.


  \todo Input and output of partial assignments.


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

