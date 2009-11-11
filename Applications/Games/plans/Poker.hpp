// Oliver Kullmann, 11.11.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/Games/plans/Poker.hpp
  \brief Module for poker playing


  \todo Links
  <ul>
   <li> http://en.wikipedia.org/wiki/Poker defines the rules and the various 
   games. </li>
   <li> The PhD thesis
   http://www.cs.ualberta.ca/~darse/Papers/billings-phd.html
   should give a first overview. </li>
  </ul>


  \todo Scope
  <ul>
   <li> Poker should have some combinatorial content. (But if the aspect
   of betting strategies, modelling players behaviour etc. is too strong,
   then it might not be a good candidate.) </li>
   <li> This is mixed with probabilities, for which a good theory
   perhaps must be compiled in advance. </li>
   <li> The advice could be a series of possible "best moves",
   with associated probability distributions about "what could be"
   and "what could happen". </li>
   <li> Likely best to concentrate on standard poker, with several
   simplifications. </li>
  </ul>


  \todo Definitions and probability calculations
  <ul>
   <li> At Maxima-level we need good representations of cards, hands etc.,
   together with all probability calculations. </li>
   <li> The next level then are computations for the first task in Poker
   playing, given a hand, choosing 0, 1 or 2 cards to be exchanged. </li>
  </ul>


  \todo Poker assistant
  <ul>
   <li> We should make a clear distinction between the "real situation" and
   the "imagined situation": In the context of this library, a "poker
   assistant" is sought which analyses precisely the current factual
   situation, yielding a "useful" representation on which basis the player
   then can use his "psychological" and "strategic" skills for the next move.
   </li>
   <li> Once one has a reasonable grasp on that, then one could go further,
   implementing a fully automated player. </li>
  </ul>

*/

