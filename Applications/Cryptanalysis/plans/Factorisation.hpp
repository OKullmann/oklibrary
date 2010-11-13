// Oliver Kullmann, 16.12.2006 (Swansea)
/* Copyright 2006 - 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/Cryptanalysis/plans/Factorisation.hpp
  \brief Plans for the module on factorisation of natural numbers

  %Tools for finding factorisations n = a * b.


  \todo Literature review
  <ul>
   <li> Literature on factorisation via SAT and CSP. </li>
   <li> Literature on factorisation in general. </li>
   <li> Literature on efficient multiplication. </li>
  </ul>


  \todo Interesting good constraint representations
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
          (strong) active clause-sets, using the school method, but for an
          arbitrary basis b, which gives the domain size for the variables.
          In this way we can optimise on b.<p></p>

          (Strong) Active clause-sets for addition and
          multiplication of digits seem most natural. A further parameter
          could be the number k of digits considered as one block (or should
          this be handled by using a larger b? seems so!).<p></p>

          The implementation of strong
          active clause-sets likely utilises (large) tables, and possibly
          finite automata. From constraints a+b=c we might get equations,
          so we need them as literals; from constraints a*b=c we could also
          get for example a=c/b, and the question is whether we should handle
          also such functional dependencies.
     </li>
     <li> Likely the more complicated multiplications algorithms
          allow a nicer representation using constraints --- of course
          the challenge is to find good constraints!
     </li>
     <li> We can also investigate different representations of numbers (not
          using the decimal expansion).
     </li>
     <li> And from methods for factorisation perhaps we could infer
          additional constraints (at least).
     </li>
    </ol>
   </li>
  </ul>

*/
