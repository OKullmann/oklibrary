// Matthew Henderson, 25.1.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/general.hpp
  \brief Plans for the module on SAT-related input/output.


  \todo Input and output of partial assignments
  <ul>
   <li> Reading a partial assignment:
    <ol>
     <li> A component shall be established, which reads from a stream a
     partial assignment, stores it in a sequence, and also provides some
     associated information. </li>
     <li> Such a stream starts with "v ", followed by non-zero integers, the
     boolean literals to be set to true, separated by space-symbols, and
     concluded by "0". </li>
     <li> Likely easiest is to throw an exception in case it finds a
     non-integer before the final 0, or there is no final zero. </li>
     <li> Then returned is a structure which besides the sequence contains
     the maximal absolute value of integers and the length of the assignment.
     </li>
     <li> Another error is contradictory literals. </li>
     <li> Repeated literals are just handed over to the sequence. </li>
     <li> On the other hand, we need to store the values anyway in some form
     of a set, since we check for contradictory literals, and so we should
     also absorb repeated literals. </li>
     <li> Perhaps easiest is then to just compute a std::set, not an arbitrary
     sequence. </li>
     <li> The only template parameter is then the literal type. </li>
     <li> Then we don't need to return the length of the assignment. </li>
     <li> Perhaps we provide a functor-class, which computes the set of
     literals, and besides this also has a member function for access to
     the maximal absolute value of literals. </li>
     <li> The whole is very similar (of course) to reading a clause, however
     it seems also different enough for not attempting to establish a common
     basis. </li>
     <li> But the policies for reading literals in
     Satisfiability/Interfaces/InputOutput/Dimacs.hpp could be used. </li>
     <li> Perhaps we should also have the concept of an "adaptor", as
     CLS-adaptors in
     Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp, now called
     "PASS-adaptor" ? </li>
     <li> After construction of the set, the PASS-adaptor would just construct
     a partial assignment via the constructor which takes a sequence of
     literals. </li>
     <li> This all should happen in the constructor-initialiser list. </li>
     <li> One could also just expose the set of literals. </li>
     <li> But then it is not an adaptor, just a class template
     R<Lit, Pass> with a constructor R(input-stream), where upon construction
     the partial assignment as a const-reference and the set directly is
     made available. </li>
     <li> Easier, these data-members are made public and constant. </li>
     <li> Plus the constant with the maximal absolut value; let's say also a
     constant with the number of (different) literals. </li>
     <li> In case of error an exception is thrown, and construction is
     aborted. </li>
     <li> If we allow non-integer literals, then everything becomes more
     complicated, so perhaps for the first model we only consider integral
     literals. </li>
    </ol>
   </li>
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

