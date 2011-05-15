// Oliver Kullmann, 10.9.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Autarkies/Search/plans/milestones.hpp

  \module_version Autarkies/Search 0.0.7 (27.2.2008)


  \par Version 0.0.8

  \par
   In Autarkies/Search/plans/general.hpp the following topics are handled:
    - Shall we abort this development?


  \par Version 0.0.9

  \par
   In Autarkies/Search/plans/general.hpp the following topics are handled:
    - Update namespace usage
    - Link to ComputerAlgebra/Satisfiability/Lisp/Autarkies/plans/general.hpp


  \par Version 0.0.10

  \par
   In Autarkies/Search/plans/AnalyseTotalAssignment.hpp the following topics
   are handled:
    - Complete correct compilation in AutarkySearch
    - Connections with other modules and parts
    - Design and implement class ComputeLargestAutarky


  \par Version 0.0.11

  \par
   In Autarkies/Search/plans/AnalyseTotalAssignment.hpp the following topics
   are handled:
    - Design and implement class AutarkySearchUbcsat


  \par Version 0.0.12

  \par
   In Autarkies/Search/plans/AnalyseTotalAssignment.hpp the following topics
   are handled:
    - AnalyseTotalAssignment.cpp


  \par Version 0.1 :

  \par
   We have a first fully tested implementation using the method of
   AnalyseTotalAssignment, and start experimentation.



  \par Version 0.1.1 :

  \par
   In Autarkies/Search/plans/EvaluatePartialAssignments.hpp the following topics
   are handled:
    - Measures
    - Design


  \par Version 0.2

  \par
   In Autarkies/Search/plans/AnalyseTotalAssignment.hpp the following topics
   are handled:
    - Complete autarky search
    - New file structure


  \par Version 0.3

  \par
   In Autarkies/Search/plans/AnalyseTotalAssignment.hpp the following topics
   are handled:
    - Changing UBCSAT


  \par Version 1.0 :

  \par
   Completely within the OKlibrary we can search for autarkies analysing total
   assignments or searching through partial assignments.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.0.3 : 10.9.2006; initial version number (several plans).
   - 0.0.4 : 9.11.2006; started integrating UBCSAT.
   - 0.0.5 : 31.1.2007; UBCSAT locally installed (under ExternalSources).
   - 0.0.6 : 23.6.2007; updated implementations from C to C++, and usage of corrected Ubcsat-headers (as well as adapted Ubcsat-headers).
   - 0.0.7 : 27.2.2008; update of plans, complete reorganisation; a big change is the development of "procedural specifications" in the computer algebra part.

*/

