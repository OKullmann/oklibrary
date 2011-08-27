// Oliver Kullmann, 10.11.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/Dimacs.hpp
  \brief Plans for the sub-module on input/output in DIMACS format


  \todo Complete the doxygen-documentation.


  \todo Write docus.


  \todo Write demos.


  \todo Connections to other modules
  <ul>
   <li> See "Input and output" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp. </li>
  </ul>


  \todo DONE (at least from a practical point of view the problem doesn't exist)
  What happens if the integers from the file are too big?
  <ul>
   <li> We cannot use the stream extractors, since they yield undefined
   behaviour, so it seems necessary to write special
   tools to read and check integers from streams. Or? </li>
   <li> (See my e-mail to Boost from 15/10/2005.) Perhaps the best
   solution here is to wrap int_type into a type with safe reading from
   istreams (setting the stream state accordingly). Or, perhaps better from
   a general design point of view, we should use a BigInteger class here for
   reading. </li>
   <li> DONE (seems to be the case)
   Apparently all compilers actually catch the errors? </li>
  </ul>


  \todo It must be tested, whether the integers can be safely negated


  \todo Improve class InputOutput::StandardDIMACSInput
  <ul>
   <li> The constructor of InputOutput::StandardDIMACSInput should take the
   following optional arguments:
    <ol>
     <li> comments require at least one space after "c" (default: no) </li>
     <li> in the parameter line after "p" either exactly one or at least one
     space is required (default: exactly one) </li>
     <li> non-space characters on the parameter line after the second
     parameter lead to an error (default: yes). </li>
     <li> Perhaps this is all packaged into a syntax-strategy-object. </li>
     <li> Compare below "Error handling". </li>
    </ol>
   </li>
  </ul>


  \todo Use Messages for messages.


  \todo Exceptions
  <ul>
   <li> The exception safety level must be specified. </li>
   <li> For throwing the exceptions a more structured approach should be used
   (so that the exceptions thrown become better testable). </li>
  </ul>


  \todo Improving class InputOutput::LiteralReadingExtended
  <ul>
   <li> Making a distinction between the two types of public member functions
   in InputOutput::LiteralReadingExtended:
    <ol>
     <li> One could make the policy an "inheritance policy", where "read"
     is a protected member function inherited by the Dimacs-parser, while
     the three accessor-functions are public. </li>
    </ol>
   </li>
  </ul>


  \todo Error handling
  <ul>
   <li> What about writing a generic DIMACS parser, starting with
   ::OKlib::DPv::ParserLiteral and ::OKlib::DPv::DimacsParser
   (located in Input_output.hpp in module DPv) ? </li>
   <li> InputOutput::StandardDIMACSInput does not perform many syntax checks;
   one should precisely define what it allows and what not. </li>
  </ul>


  \todo ListTransfer
  <ul>
   <li> Is this submodule really the right place? </li>
   <li> Once namespaces are updated, one should have a look whether
   the name "ListTransfer" is appropriate. </li>
  </ul>


  \todo Write extended Dimacs-parser for clause-sets with non-boolean
  variables
  <ul>
   <li> The parameter line now is of the form
   \verbatim
p gcnf n c k
   \endverbatim
   where k specifies the set of possible values {0, ..., k-1}. </li>
   <li> Literals are of the form
   \verbatim
n,v
   \endverbatim
   where n as usual is the variable (number), while v in {0, ..., k-1}. </li>
   <li> If k=2, then also "+n", "-n" and "n" are literals (so that ordinary
   Dimacs inputs are accepted). </li>
  </ul>


  \todo Write Dimacs-parser for weighted (partial) MaxSAT formats
  <ul>
   <li> The basic weighted Dimacs format for weighted partial
   MaxSAT problems is simply
   \verbatim
c comment line
p wcnf num_vars num_clauses maximum_weight
weight l1 l2 l3 ...
weight l4 l5 l6 ...
   \endverbatim
   where the standard Dimacs p-line has been extended to use the
   "wcnf" type and the maximum_weight parameter is the maximum weight
   that one will use as a clause-weight. Hard clauses have the maximum
   weight and soft clauses have a weight less than top. </li>
   <li> If one fails to specify a maximum_weight then you have a
   weighted MaxSAT problem. </li>
   <li> If one has the maximum weight but uses only the maximum weight
   and 1 for hard and soft clauses, then you have a partial MaxSAT
   problem. </li>
   <li> See http://www.maxsat.udl.cat/08/index.php?disp=requirements for
   more details. </li>
   <li> We need notions of how to handle this, and consideration of the
   way to store weights in datastructures. </li>
  </ul>


  \todo Write higher level modules
  <ul>
   <li> Like ReadClauseCollection<DimacsParser, ClauseCollection> and
   WriteClauseCollection<ClauseCollection, DimacsWriter>
   (likely this should go into a separate file). </li>
  </li>

*/

