// Oliver Kullmann, 10.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/Dimacs.hpp
  \brief Plans for the sub-module on input/output in DIMACS format


  \todo Add a policy L which allows to handle extended Dimacs format. DONE
  <ul>
   <li> %Variables here are strings with identifier-syntax. </li>
   <li> The policy L manages a maps from identifiers to natural numbers and
   back. </li>
   <li> L is used exactly once, in member function read_clauses, replacing
   \code in >> literal; \endcode </li>
   <li> The main function is
   \code L::operator()(std::istream& in, int_type& literal); \endcode
   which reads the literal from the stream. </li>
   <li> The "strict policy" emplements this bracket-operator just as
   \code in >> literal; \endcode
   (without anything further). </li>
   <li> The "extended policy" contains
    <ol>
     <li> a map m, which assign to every occurring variable string its
     index; </li>
     <li> a counter n for the number of variables; </li>
     <li> a vector, which for each variable index i contains an iterator
     to the corresponding map entry. </li>
    </ol>
   </li>
  </ul>


  \todo Complete the doxygen-documentation.


  \todo Write docus.


  \todo Write demos.


  \todo The constructor of InputOutput::StandardDIMACSInput should take the
  following optional arguments:
  <ul>
   <li> comments require at least one space after "c" (default: no) </li>
   <li> in the parameter line after "p" either exactly one or at least one
   space is required (default: exactly one) </li>
   <li> non-space characters on the parameter line after the second
   parameter lead to an error (default: yes). </li>
   <li> Perhaps this is all packaged into a syntax-strategy-object. </li>
   <li> Compare below "Error handling". </li>
  </ul>


  \todo What happens if the integers from the file are too big?
  <ul>
   <li> We cannot use the stream extractors, since they yield undefined
   behaviour, so it seems necessary to write special
   tools to read and check integers from streams. Or? </li>
   <li> (See my e-mail to Boost from 15/10/2005.) Perhaps the best
   solution here is to wrap int_type into a type with safe reading from
   istreams (setting the stream state accordingly). Or, perhaps better from
   a general design point of view, we should use a BigInteger class here for
   reading. </li>
   <li> Apparently all compilers actually catch the errors? </li>
  </ul>


  \todo It must also be tested, whether the integers can be safely negated.


  \todo The exception safety level must be specified.


  \todo Use Messages for messages.


  \todo For throwing the exceptions a more structured approach should be used
      (so that the exceptions thrown become better testable).


  \todo Consider the code-comments regarding checking.


  \todo Making a distinction between the two types of public member functions
  in InputOutput::LiteralReadingExtended:
  <ul>
   <li> One could make the policy an "inheritance policy", where "read"
   is a protected member function inherited by the Dimacs-parser, while
   the three accessor-functions are public. </li>
  </ul>


  \todo Error handling
  <ul>
   <li> What about writing a generic DIMACS parser, starting with
   ::OKlib::DPv::ParserLiteral and ::OKlib::DPv::DimacsParser
   (located in Input_output.hpp in module DPv) ? </li>
   <li> InputOutput::StandardDIMACSInput does not perform many syntax checks;
   one should precisely define what it allows and what not. </li>
  </ul>


  \todo Write extended Dimacs-parser for clause-sets with non-boolean variables:
  <ul>
   <li> the parameter line now is of the form
   \verbatim
p gcnf n c k
   \endverbatim
   where k specifies the set of possible values {0, ..., k-1} </li>
   <li> literals are of the form
   \verbatim
n,v
   \endverbatim
   where n as usual is the variable (number), while v in {0, ..., k-1}. </li>
   <li> if k=2, then also "+n", "-n" and "n" are literals (so that ordinary
   Dimacs inputs are accepted). </li>
  </ul>


  \todo Write higher level modules like
  ReadClauseCollection<DimacsParser, ClauseCollection> and
  WriteClauseCollection<ClauseCollection, DimacsWriter>
  (likely this should go into a separate file).

*/

