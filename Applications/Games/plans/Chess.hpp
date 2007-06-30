// Oliver Kullmann, 9.6.2007 (Swansea)

/*!
  \file Games/plans/Chess.hpp
  \brief Submodule for chess playing and SAT


  \todo The appropriate decision problem seems to be:
  Given a position P and a natural number k (where the position
  includes the party who is to move), does there exist a legal
  sequence of k moves, so that finally the enemies king is taken?


  \todo Encodings
  <ul>
   <li> Easiest seems to use 64 parameter-variables for the fields,
   with values {Q, K, B, R, K, P} x {w,b}, and one variable per
   move, with values all possible pairs like "w, e2-e4" and
   "b, d7-d6". Then one variable for the party who is to move
   is needed, and some additional variables for information
   whether rochade is been performed etc. Likely for pawns
   rows 1 and 8 should be forbidden.
   </li>
   <li> A position is encoded via setting of the 64 field-variables etc. </li>
   <li> The chess-rules are enforced by clauses like
    <ol>
     <li> If w is to move, then a black move is forbidden. </li>
     <li> If some party has no king anymore, then the only move is the no-move. </li>
     <li> The move "w, e1-f1" is not possible if no white
     figure is at e1, or a white figure is on f1, or the white
     figure on e1 is a bishop or a knight.
     </li>
     <li> For the rochade one needs to express that certain fields are not
     under attack of the other party; this could be expressed by explicitely
     stating all attacking scenarios (for example, that f1 is not under attack
     could be stated as a long list like "no black king an g1", "no black rook
     on f8, while the rest of the column is free" etc.
     </li>
    </ol>
   <li> A move changes all position variables accordingly (for the next stage):
   "If w, e2-e4" then all fields excepts of e2 and e4 are invariant, while e2 is now
   empty, and e4 contains the figure from e2."
   </li>
   <li> Better one has a hierarchy of abstractions for the moves (using active clause-sets).
   For example from one position to the next; as usual, the main challenge is to provide
   good inference for partial assignments.
   </li>
   <li> With active clause-sets much more easily positional knowledge can be incorporated;
   then we more in direction to a general "player" (not just a check-mate solver), but
   then likely we need optimisation. Are there optimisation variants of QBF?
   We need some like "(Quantor-prefix) F(x) >= k", where x is the variable vector, and
   F(X) is an evaluation function. If the decision "F(x) >= k" is rule-based, then perhaps
   we can fit it into the decision-framework? </li>
  </ul>


  \todo Problem formulations
  <ul>
   <li> That in position P white has a check mate in 2 moves can be expressed by the QBF
     <center>
     There exists a move_1 of white such that for moves_2 of black there exists a move_3
     of white such that for all moves_4 of black there exists a move_5 of white, such that
     after that the black king as been taken.
     </center>
     For each of the 6 stages we have the position-variables (in 6 variations) and the
     rules for excluding illegal moves linking the position variables with the five
     move variables. The initial position is encoded by unit-clauses, the final situation
     without the black king is encoded by the assertion, that no field contains the
     black king.
   </li>
   <li> Besides asking whether a position has an enforced winning, one can also
   ask if both sides help, what is the fastest win? For example, what is the shortest
   game (from the initial position)? One problem in the formulation is that either we have
   an for-all quantifier for the last move of black, or we formulate as a rule that a check
   must be eliminated if possible. </li>
   <li> Likely the whole problem is very hard, and a great challenge should be just to
   reach the simple brute-force performance of a simple chess-programm. </li>
  </ul>


  \todo Planning: Discuss the relations to planning problems


  \todo Is there some C/C++ library for chess-programming?
  
*/

