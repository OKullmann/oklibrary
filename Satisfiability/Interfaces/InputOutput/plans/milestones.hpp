// Oliver Kullmann, 10.9.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Interfaces/InputOutput/plans/milestones.hpp

  \module_version Interfaces/InputOutput 0.1.1.2 (31.8.2011)


  \par Version 0.1.2 :

  \par
   In Interfaces/InputOutput/plans/general.hpp the following
   topics are handled:
    - Tests for RenameDimacs
    - Update output for application tests
    - Can not update applications tests for AppendDimacs : DONE

  \par
   In Interfaces/InputOutput/AppendDimacs.cpp the following topics
   are handled:
    - Incomplete specification : DONE

  \par
   In Interfaces/InputOutput/plans/ExtendedDimacsStatistics.hpp the following
   topics are handled:
    - Write application tests
    - Update and extension : DONE

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - What happens if the integers from the file are too big? : DONE


  \par Version 0.1.3 :

  \par
   In Interfaces/InputOutput/plans/general.hpp the following topics
   are handled:
    - Comprehensive statistics

  \par
   In Interfaces/InputOutput/MinOnes2WeightedMaxSAT.cpp the following topics
   are handled:
    - Improve specification

  \par
   In Interfaces/InputOutput/FilterDimacs.cpp the following topics
   are handled:
    - Add application tests
    - Weak implementation
    - Move CLSAdaptorFilter

  \par
   In Interfaces/InputOutput/SortByClauseLength.cpp the following topics
   are handled:
    - Move AllEqual to OrderConstructions

  \par
   In Interfaces/InputOutput/AppendDimacs.cpp the following topics
   are handled:
    - Move CLSAdaptorAppend

  \par
   In Interfaces/InputOutput/RandomShuffleDimacs.cpp the following topics
   are handled:
    - Seed should be uint32_t


  \par Version 0.1.4 :

  \par
   In Interfaces/InputOutput/plans/ClauseSetAdaptors.hpp the following topics
   are handled:
    - Is usage of boost::distance effective?
    - Need "ReverseDimacs" application and clause-set adaptor

  \par
   In Interfaces/InputOutput/plans/Colouring.hpp the following topics
   are handled:
    - Improve implementations
    - Generic translation

  \par
   In Interfaces/InputOutput/SortByClauseLengthDescending.cpp the following
   topics are handled:
    - Merge with SortByClauseLength.cpp

  \par
   In Interfaces/InputOutput/plans/PartialAssignments.hpp the following topics
   are handled:
    - Exceptions
    - Improvements of InputOutput::ReadPass
    - Write unit-tests
  \par
   In Interfaces/InputOutput/plans/ExtendedDimacsStatistics.hpp the following
   topics are handled:
    - Write docus


  \par Version 0.1.5 :

  \par
   In Interfaces/InputOutput/MinOnes2WeightedMaxSAT.cpp the following topics
   are handled:
    - Move to correct place

  \par
   In Interfaces/InputOutput/plans/general.hpp the following topics
   are handled:
    - Elementary file-surgery
    - Update
    - See OKlib/Satisfiability/ProofSystems/DPv/Input_output.hpp


  \par Version 0.1.6 :

  \par
   In Interfaces/InputOutput/plans/general.hpp the following topics
   are handled:
    - Rewrite code for the new namespaces.

  \par
   In Interfaces/InputOutput/plans/ExtendedDimacsStatistics.hpp the following
   topics are handled:
    - More convenient input
    - Improving efficiency

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - Complete the doxygen-documentation.


  \par Version 0.1.7 :

  \par
   In InputOutput/plans/general.hpp the following topics are handled:
    - Write docus

  \par
   In Interfaces/InputOutput/plans/ExtendedDimacsStatistics.hpp the following
   topics are handled:
    - Generalising the computation

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - Write docus


  \par Version 0.1.8 :

  \par
   In Interfaces/InputOutput/plans/general.hpp the following topics
   are handled:
    - Move to the new test system
    - Write demos

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - Write demos


  \par Version 0.1.9 :

  \par
   In Interfaces/InputOutput/plans/general.hpp the following topics
   are handled:
    - Complete the tests

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - It must be tested, whether the integers can be safely negated


  \par Version 0.1.10 :

  \par
   In Interfaces/InputOutput/plans/ClauseSetAdaptors.hpp the following topics
   are handled:
    - Document the basic concept CLSAdaptor
    - Discussing the basic concept CLSAdaptor
    - Write concept for InputOutput::Statistics class


  \par Version 0.1.12 :

  \par
   In Interfaces/InputOutput/plans/ClauseSetAdaptors.hpp the following topics
   are handled:
    - Write tests!
    - CLSAdaptorFullStatistics
    - RawDimacsCLSAdaptor
    - Create "base class" for "raw adaptors"


  \par Version 0.2 :

  \par
   In Interfaces/InputOutput/plans/general.hpp the following topics
   are handled:
    - Complete the functionality

  \par
   In Interfaces/InputOutput/plans/ExtendedToStrictDimacs.hpp the following
   topics are handled:
    - Parameter handling
    - Extended Dimacs format
    - Complete doxygen-documentation
    - Write docus

  \par
   Update milestones.


  \par Version 0.2.1 :

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - Improve class InputOutput::StandardDIMACSInput
    - Use Messages for messages.

  \par
   In Interfaces/InputOutput/plans/ExtendedToStrictDimacs.hpp the following
   topics are handled:
    - Permutations
    

  \par Version 0.2.2 :

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - Exceptions


  \par Version 0.2.3 :

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - Improving class InputOutput::LiteralReadingExtended
    - Error handling


   \par Version 0.2.4 :

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - ListTransfer


  \par Version 0.3 :

  \par
   In Interfaces/InputOutput/plans/general.hpp the following topics
   are handled:
    - Move InputOutput/ExtendedToStrictDimacs.cpp
    - Write Dimacs-parser for weighted (partial) MaxSAT formats


  \par Version 0.4 :

  \par
   In Interfaces/InputOutput/plans/Dimacs.hpp the following topics are handled:
    - Write extended Dimacs-parser for clause-sets with non-boolean variables


  \par Version 0.5 :

  \par
   In Interfaces/InputOutput/plans/Colouring.hpp the following topics
   are handled:
    - Improve design

  \par
   In Interfaces/InputOutput/plans/general.hpp the following topics
   are handled:
    - Input and output of propositional formulas
    - Circuits

  \par
   In Interfaces/InputOutput/plans/ExtendedDimacsStatistics.hpp the following
   topics are handled:
    - Using Messages



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.0.7 : 7.11.2006; initial version number (several implementations).
   - 0.0.8 : 26.9.2007; moved to new place in part Satisfiability, and related updates.
   - 0.0.9 : 5.2.2008; created facilities to read extended Dimacs, and to translate it to standard Dimacs, and extensions and updates of plans.
   - 0.1   : 9.1.2011; systemic updates and improvements, extended existing adaptors (especially regarding statistics and transfers), hypergraph colouring (boolean and non-boolean).
   - 0.1.1 : 14.7.2011; many corrections, improvements, extensions, and new applications for shuffling and sorting, for MaxSAT and PB translations, for input of partial assignments, for file surgery.

*/

