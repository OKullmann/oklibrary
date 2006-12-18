// Oliver Kullmann, 16.12.2006 (Swansea)

/*!
  \file Cryptanalysis/plans/Factorisation.hpp
  \brief Plans for the module on factorisation of natural numbers

  Tools for finding factorisations n = a * b.

  \todo Literature review:
   - Literature on factorisation via SAT and CSP
   - Literature on factorisation in general.
   - Literature on efficient multiplication.

  \todo Interesting good constraint representations:
  <ul>
   <li> Boolean CNF:
    <ol>
     <li> Of course, a generator for the standard school method needs to be
          implemented.
     </li>
     <li> And then generators exploiting more complicated methods should be
          investigated.
     </li>
    </ol>
   </li>
   <li> Constraints in general:
    <ol>
     <li> The next thing is to use a representation as an alliance of
          active clause-sets, using the school method, but for an
          arbitrary basis b. Active clause-sets for addition and
          multiplication of digits seem most natural. In this way
          we can optimise on b.
     </li>
     <li> Likely the more complicated algorithms considered for Boolean
          CNF allow a nicer representation using constraints --- of course
          the challenge is to find good constraints!
     </li>
    </ol>
   </li>
  </ul>

*/
