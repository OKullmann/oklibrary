// Oliver Kullmann, 13.6.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/plans/BasicSetOperations.hpp
  \brief Plans for basic set operations
  
  
  \todo Organisation
  <ul>
   <li> Perhaps this module should be called "BasicOperations". </li>
  </ul>


  \todo Class SetAlgorithms::Disjoint
  <ul>
   <li> Write tests in the new test system. </li>
   <li> The implementation using std::set_intersection is very inefficient.
   </li>
   <li> Instead we should copy the implementation of std::set_intersection,
   but stopping as soon as the first common element was found. </li>
   <li> This can be further improved by making stronger use of the
   sorting (for example using binary search). </li>
   <li> Making use of stronger iterator- and container-properties. </li>
  </ul>

*/


