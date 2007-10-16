// Oliver Kullmann, 18.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Parallelism/plans/general.hpp
  \brief Plans for the module on parallelism


  \todo Update namespace-usage.


  \todo Run through examples from [C++ Cookbook 2006, Chapter 12]
  <ul>
   <li> 12.1 "Creating a Thread" : DONE (Parallelism/demo_Example_12_1.cpp) </li>
   <li> 12.1 Remarks about thread_group </li>
   <li> 12.2 "Making a Resource Thread-Safe" </li>
   <li> 12.3 "Notifying One Thread from Another" </li>
   <li> 12.4 "Initializing Shared Recources Once" </li>
   <li> 12.5 "Passing an Argument to a Thread Function" </li>
  </ul>


  \todo New test system
  <ol>
   <li> Transfer the tests. </li>
   <li> Extend the test-functionality so that the point where the assert
   happens (see bug-reports) can be pinpointed. </li>
   <li> How to do it right? </li>
  </ol>


  \todo Investigate libraries for processes and threads
  <ul>
   <li> Boost-1_34_0 (Threads and ???) </li>
   <li> Poco </li>
   <li> ACE </li>
  </ul>


  \todo Decide, how to handle parallel computations for the OKlibrary.

*/

/*!
  \namespace OKlib::Programming::Parallelism
  \brief Components for parallelising algorithms

  Alias "Pll".
*/

namespace OKlib {
  namespace Programming {
    namespace Parallelism {
    }
    namespace Pll = Parallelism;
  }
}

