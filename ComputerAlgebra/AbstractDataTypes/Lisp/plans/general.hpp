// Oliver Kullmann, 8.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/AbstractDataTypes/Lisp/plans/general.hpp
  \brief Plans for Maxima-components regarding abstract data types


  \todo Create milestones


  \todo Iterators and ranges
  <ul>
   <li> In the Maxima context, it seems that iterators do not refer to
   memory addresses and the like, and so we can make iterators aware
   themselves of being dereferencible or not. </li>
   <li> A forward iterator is a list of length at least one, with the first
   component either "done" or the index. </li>
   <li> If the first component is not "done", then we have at least two
   further components, functions for obtaining the value, and for incrementing
   an iterator (in place). </li>
   <li> Example:
   \verbatim
it : [0,lambda([it],it[1]),lambda([it],block([i:it[1]], if i < 10 then it[1]:i+1 else it[1]:done, it))];
while it[1]#done do (
  print(it[2](it)),
  it = it[3](it)
);
   \endverbatim
   </li>
   <li> Some helper functions:
   \verbatim
state(it) := it[1]$
value(it) := it[2](it)$
increment(it) := it[3](it)$
successor(it) := block([nit:copy(it)], increment(nit), nit)$
decrement(it) := it[4](it)$
predecessor(it) := block([nit:copy(it)], decrement(nit), nit)$
advance(it,n) := it[5](it,n)$

print_range(it) :=
 while state(it)#done do (print(value(it)), increment(it));
it2list(it) := block([res:[]],
  while state(it)#done do (res:cons(value(it),res), increment(it)),
  reverse(res))$

it_generator(n) := [
 0,
 lambda([it],it[1]),
 buildq([n],lambda([it],block([i:it[1]], if i < n then it[1]:i+1 else it[1]:done))),
 lambda([it], it[1]:it[1]-1),
 buildq([n],lambda([it,a],block([i:it[1]+a], if i <= n then it[1]:i else it[1]:done)))
];

print_range(it_generator(10));
   \endverbatim
   </li>
   <li> If we have four components, then we have a bidirectional iterator, and
   if we have five components, then we have a random access iterator. </li>
   <li> All operations on iterators except of state are only defined if
   the iterator is dereferencible, that is, if the state is not "done". </li>
  </ul>

*/
