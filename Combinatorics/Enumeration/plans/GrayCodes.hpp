// Oliver Kullmann, 24.7.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/Enumeration/plans/GrayCodes.hpp
  \brief %Tools to enumerate combinations, that is all elements of A_1 x ... x A_n for finite linearly ordered sets A_i, in the order given by Gray codes.


  \todo Basic concepts
  <ul>
   <li> Write an iterator (at least a forward iterator) that, after
   initialising the A_i in appropriate forms (as a sequence of ranges, or
   simpler by a sequence of natural numbers (the sizes of the A_i)), iterates
   through the code-words for example by representing elements by pairs (i, j)
   of iterators, where i points to the sequence A_i, while j points to the
   element of A_i (or simply by pairs (i, j) of indices). </li>
   <li> More efficient would be to use a vector of length n of iterators, each
   iterator at position i pointing into A_i. </li>
   <li> But it should also be possible to run directly
   through all vectors of length n with respective entries from A_i, so that
   for example we can run through all binary code words of length n. </li>
   <li> One should have special implementations for the special (though 
   "representative") case of A_i = {0, ..., k_i}. </li>
   <li> Having the possibility of enumerating  all those combinations, using
   the entries as indices we can also solve the general problem; the general
   solution, not using indices, would save the user the step of putting things
   into vectors first, and dereferencing indices all the time, but of course
   the general solution introduces its own complexities. </li>
   <li> In the case of binary codes (all |A_i| = 2) in principal it suffices to
   tell the next position of a flip (in the general situation I assume that
   telling the next position where a cyclic movement occurs is sufficient).
   </li>
   <li> It should be possible to obtain this information also when the iterator
   directly does the flip. </li>
  </ul>


  \todo Algorithms
  <ul>
   <li> The first problem is to determine an algorithm, generalising algorithms
   for binary Gray codes (using the Hamming distance on A_1 x ... x A_n). </li>
  </ul>


  \todo Efficiency
  <ul>
   <li> An efficiency problem occurs, when for example we want to enumerate
   all total assignments for a problem instance, and then doubling of the
   code word representation occurs: For the application algorithm we likely
   need our own data structures for total assignments, while the iterator also
   needs an internal representation of the current code word. Best, if in such
   situations the iterator could use here the total assignment (instead of its
   own representation). </li>
   <li> The general problem here is, that we might want to enumerate objects
   with some structure. </li>
  </ul>

*/

