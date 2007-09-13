// Oliver Kullmann, 13.9.2007 (Swansea)

/*!
  \file Solvers/OKsolver/SAT2002/docus/general.hpp
  \brief General documentation on the original OKsolver

  <h2> Macro options </h2>

  The most important macro options (that is, compile time options):
  <ul>
   <li> If <code>NBAUMRES</code> is defined, then tree resolution pruning
   ("intelligent backtracking") is turned off, while otherwise (the default)
   it is turned on (that is, <code>BAUMRES</code> gets defined).
    <ul>
     <li> If <code>NLITTAB</code> is defined, then the more space efficient
     (but less time time efficient) special datastructure is not used, while
     otherwise (the default) it is used (that is, <code>LITTAB</code> gets
     defined). </li>
    </ul>
   </li>
   <li> If <code>ASSIGNMENT</code> is defined, then if a satisfying assignment
   is found, it is output, while otherwise only the satisfiability status
   is returned. (Internally, <code>ASSIGNMENT</code> is translated yet into
   <code>BELEGUNG</code>.) </li>
  </ul>

*/

