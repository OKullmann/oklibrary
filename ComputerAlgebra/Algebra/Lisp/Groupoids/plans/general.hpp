// Oliver Kullmann, 6.7.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/Groupoids/plans/general.hpp
  \brief Plans in general regarding groupoids


  \todo Write tests


  \todo Naming conventions
  <ul>
   <li> See "Naming conventions" in
   ComputerAlgebra/Algebra/Lisp/Groupoids/plans/Homomorphisms.hpp. </li>
   <li> Names like "associative_bydef_grd" perhaps should be renamed to
   "associative_grd_p" ? </li>
  </ul>


  \todo Forms of "constructive" groupoids
  <ul>
   <li> For implicit representations
   (see "Abstract representation of structures") we (still) assume
    <ol>
     <li> that representations of groupoid elements are given by terms (though
     there may be many terms), </li>
     <li> that we can decide equality, </li>
     <li> and that we can decide membership. </li>
    </ol>
   </li>
   <li> One can drop both (decidability) conditions, turning the groupoid
   operations into "promise functions" which only yield defined results
   if the inputs are actually groupoid elements. </li>
   <li> Independently we can consider that we have given a function f() which
   returns a "random element" of the finite groupoid:
    <ol>
     <li> The returned valued must be guaranteed to be an element. </li>
     <li> Strongest is guaranteed uniform distribution. </li>
     <li> In the extreme other case we don't have any guarantee. </li>
    </ol>
   </li>
   <li> Strengthenings of implicit representations:
    <ol>
     <li> A generating set is often at hand. </li>
     <li> Additionally a "rewrite algorithm" might be given, computing for
     each representation of a groupoid element a word in the generators
     representing that element. </li>
     <li> For such a rewrite algorithm one can require "canonicity", that
     is, for inputs representing the same element we obtain the same word
     (thus the rewrite algorithm also solves equality decision). </li>
    </ol>
   </li>
  </ul>


  \todo Straight-line programs
  <ul>
   <li> The most general form of a "straight-line program" to represent
   an element of a groupoid is just a Maxima term. </li>
   <li> Much more special, it is just a sequence of words, the first only
   in the given generators, and all further words are allowed to use all
   previous words, while the final word represents the element. </li>
  </ul>


  \todo Congruence relations
  <ul>
   <li> A new submodule "Quotients.mac" is needed for the handling of
   congruence relations in groupoids. </li>
   <li> See also "Induced congruence relation" in
   ComputerAlgebra/Algebra/Lisp/Groupoids/plans/Homomorphisms.hpp. </li>
   <li> Providing congruence_grd_p, and for a given congruence relation,
   the quotient groupoid. </li>
   <li> Likely we should provide two representations of the quotient groupoid:
    <ol>
     <li> The direct one, with equivalence classes as elements. </li>
     <li> Since Maxima-sets have an underlying order, it seems sensible to
     use the first element of an equivalence class as a representative. </li>
    </ol>
   </li>
  </ul>

*/

