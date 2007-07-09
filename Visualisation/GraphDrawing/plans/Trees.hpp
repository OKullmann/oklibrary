// Oliver Kullmann, 9.7.2007 (Swansea)

/*!
  \file GraphDrawing/plans/Trees.hpp
  \brief Plans for drawing trees (especially search trees)


  \todo Rheingold-Tilford algorithm outline
  <ul>
   <li> See the MSc thesis of Warren Bailey (supervisor OK). </li>
   <li> Investigate colouring schemes (see discussion of OK with Mark Jones). </li>
   <li> Possible numerical node labelings:
    <ol>
     <li> running time at this node </li>
     <li> number of reductions </li>
     <li> formula sizes </li>
    </ol>
   </li>
   <li> Possible numerical edge labelings:
    <ol>
     <li> the distance value </li>
     <li> the probability in the probability distribution corresponding
     to the branching tuple (see Heuristics/StatisticalAnalysis/plans/BacktrackingProbing.hpp) </li>
     <li> the optimal probability (from the canonical probability distribution
     of the tree; see OK's SAT-handbook article on heuristics) </li>
     <li> the estimated SAT-probability (see Heuristics/StatisticalAnalysis/plans/SatisfiabilityPrediction.hpp) </li>
    </ol>
   </li>
   <li> Possible non-numerical edge labelings:
    <ol>
     <li> Branching literal </li>
    </ol>
   </li>
  </ul>

*/

