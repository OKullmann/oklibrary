// Oliver Kullmann, 9.6.2007 (Swansea)
/* Copyright 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/Games/plans/general.hpp
  \brief Module for game playing via SAT (and beyond)


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Games/Lisp/plans/general.hpp. </li>
  </ul>


  \todo Update namespaces.


  \todo Create milestones.


  \todo What other games are interesting?
  <ul>
   <li> Go seems to be too "strategical" (?). </li>
   <li> "Nine men's morris" ("Muehle" in German) should be a solved game(?);
   this could be alright for SAT. </li>
   <li> Checkers should also be solved(?), and could pose
   some problems (interesting) due to the jump sequences. </li>
   <li> Backgammon perhaps involves too much special knowledge. </li>
   <li> "Tic, Tac, Toe" and generalisations could be a nice exercise;
   see "Generalising positional games to SAT" in
   ComputerAlgebra/Games/Lisp/plans/general.hpp and "Positional games" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp. </li>
   <li> For Sudoku see module LatinSquares. </li>
  </ul>


  \todo "Problem solver"
  <ul>
   <li> Kind of clear, that with (generalised) SAT, also
   with QBF, one does not get a "player", but only a
   "problem solver" --- can this be a subprocedure for
   a "player" ? </li>
   <li> Another aspect could be counting of solutions. </li>
  </ul>


  \todo Tools for translations
  <ul>
   <li> We need some nice tools to ease the translations. </li>
   <li> One approach is via transition relations and model checking. </li>
  </ul>

*/

/*!
  \namespace OKlib::Games
  \brief Components related to (combinatorial) games
*/

namespace OKlib {
  namespace Games {
  }
}

