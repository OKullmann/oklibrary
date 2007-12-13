// Oliver Kullmann, 9.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/Games/plans/general.hpp
  \brief Module for game playing via SAT (and beyond)


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
   <li> "Tic, Tac, Toe" and generalisations could be a nice exercise. </li>
   <li> For Sudoku see module LatinSquares. </li>
   <li> For Poker we should make a clear distinction between the "real
   situation" and the "imagined situation": In the context of this library,
   a "poker assistant" is sought which analyses precisely the current
   factual situation, yielding a "useful" representation on which basis
   the player then can use his "psychological" and "strategic" skills
   for the next move.
    <ol>
     <li> Poker should have some combinatorial content. (But if the aspect
     of betting strategies, modelling players behaviour etc. is too strong,
     then it might not be a good candidate.) </li>
     <li> This is mixed with probabilities, for which a good theory
     perhaps must be compiled in advance. </li>
     <li> The advice could be a series of possible "best moves",
     with associated probability distributions about "what could be"
     and "what could happen". </li>
     <li> First we need some overview on the literature.
      <ul>
       <li> http://en.wikipedia.org/wiki/Poker defines the rules and the
       various games. </li>
       <li> The PhD thesis
       http://www.cs.ualberta.ca/~darse/Papers/billings-phd.html
       should give a first overview. </li>
      </ul>
     </li>
    </ol>
   </li>
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

