// Oliver Kullmann, 1.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/plans/PartialAssignments.hpp
  \brief Plans for input and output of partial assignments


  \todo DONE (InputOutput::ReadPass)
  Input of partial assignments
  <ul>
   <li> A component shall be established, which reads from a stream a
   partial assignment, stores it in a sequence, and also provides some
   associated information. </li>
   <li> Such a stream starts with "v ", followed by non-zero integers, the
   boolean literals to be set to true, separated by space-symbols, and
   concluded by "0". </li>
   <li> DONE (no distinction between the two cases)
   Likely easiest is to throw an exception in case it finds a
   non-integer before the final 0, or there is no final zero. </li>
   <li> Then returned is a structure which besides the sequence contains
   the maximal absolute value of integers and the length of the assignment.
   </li>
   <li> Another error is contradictory literals. </li>
   <li> Repeated literals are just handed over to the sequence. </li>
   <li> On the other hand, we need to store the values anyway in some form
   of a set, since we check for contradictory literals, and so we should
   also absorb repeated literals. </li>
   <li> Perhaps easiest is then to just compute a std::set, not an arbitrary
   sequence. </li>
   <li> The only template parameter is then the literal type. </li>
   <li> Then we don't need to return the length of the assignment. </li>
   <li> Perhaps we provide a functor-class, which computes the set of
   literals, and besides this also has a member function for access to
   the maximal absolute value of literals. </li>
   <li> The whole is very similar (of course) to reading a clause, however
   it seems also different enough for not attempting to establish a common
   basis. </li>
   <li> But the policies for reading literals in
   Satisfiability/Interfaces/InputOutput/Dimacs.hpp could be used. </li>
   <li> Perhaps we should also have the concept of an "adaptor", as
   CLS-adaptors in
   Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp, now called
   "PASS-adaptor" ? </li>
   <li> After construction of the set, the PASS-adaptor would just construct
   a partial assignment via the constructor which takes a sequence of
   literals. </li>
   <li> This all should happen in the constructor-initialiser list. </li>
   <li> One could also just expose the set of literals. </li>
   <li> But then it is not an adaptor, just a class template
   R<Lit, Pass> with a constructor R(input-stream), where upon construction
   the partial assignment as a const-reference and the set directly is
   made available. </li>
   <li> Easier, these data-members are made public and constant. </li>
   <li> DONE (no need for the number of literals, due to pa.size())
   Plus the constant with the maximal absolut value; let's say also a
   constant with the number of (different) literals. </li>
   <li> In case of error an exception is thrown, and construction is
   aborted. </li>
   <li> If we allow non-integer literals, then everything becomes more
   complicated, so perhaps for the first model we only consider integral
   literals. </li>
  </ul>


  \todo Exceptions
  <ul>
   <li> We should have a standard way of declaring exception types. </li>
  </ul>


  \todo Improvements of InputOutput::ReadPass
  <ul>
   <li> Perhaps in maxvar() we should use std::max_element together
   with an iterator-adaptor. </li>
   <li> Perhaps one could also provide a general literal2var iterator
   adaptor. </li>
   <li> Alternatively, it would be better to directly compute max_var
   in fill_litset; perhaps this is possible with C++0x? </li>
  </ul>


  \todo Write unit-tests

*/
