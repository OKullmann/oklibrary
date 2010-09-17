// Oliver Kullmann, 17.9.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Categories/plans/ClauseSets.hpp
  \brief Plans regarding the category of formal clause-sets


  \todo Improve flcls2fcls
  <ul>
   <li> Providing the information on the renaming:
    <ol>
     <li> We should provide the translation between old and new literals. </li>
     <li> Perhaps returning a pair, where the second component is a hash-map.
     </li>
     <li> The question is, in which direction shall the hash-map translate?
     Perhaps most like that one wants for the new literals to know to which
     old literals (in the literal-structure) they correspond. </li>
     <li> On the other hand, the hash-map translating from the old literals
     to the new literals is already been computed, and needed only to be
     returned. Perhaps that's best. </li>
    </ol>
   </li>
  </ul>

*/
