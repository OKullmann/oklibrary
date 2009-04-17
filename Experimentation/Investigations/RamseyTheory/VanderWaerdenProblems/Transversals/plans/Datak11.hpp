// Oliver Kullmann, 16.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/Datak11.hpp
  \brief Investigating the transversal hypergraph of van-der-Waerden hypergraphs for k = 11 (length of arithmetic progressions)


  \todo Elementary statistics for k=11
  <ul>
   <li> Computing the number of minimum transversals:
   \verbatim
oklib_monitor : true;
A11(n):=arithprog_hg(11,n)$
L70_11 : minimum_transversals_mongen(70,A11,[{}])$
   \endverbatim
   </li>
   <li> Using "VdWTransversalsInc 11 1 0 OutputFile" we obtain
   \verbatim
k n tau
11 1 0
11 2 0
11 3 0
11 4 0
11 5 0
11 6 0
11 7 0
11 8 0
11 9 0
11 10 0
11 11 1
11 12 1
11 13 1
11 14 1
11 15 1
11 16 1
11 17 1
11 18 1
11 19 1
11 20 1
11 21 1
11 22 2
11 23 2
11 24 2
11 25 2
11 26 2
11 27 2
11 28 2
11 29 2
11 30 2
11 31 2
11 32 2
11 33 3
11 34 3
11 35 3
11 36 3
11 37 3
11 38 3
11 39 3
11 40 3
11 41 3
11 42 3
11 43 3
11 44 4
11 45 4
11 46 4
11 47 4
11 48 4
11 49 4
11 50 4
11 51 4
11 52 4
11 53 4
11 54 4
11 55 5
11 56 5
11 57 5
11 58 5
11 59 5
11 60 5
11 61 5
11 62 5
11 63 5
11 64 5
11 65 5
11 66 6
11 67 6
11 68 6
11 69 6
11 70 6
11 71 6
11 72 6
11 73 6
11 74 6
11 75 6
11 76 6
11 77 7
11 78 7
11 79 7
11 80 7
11 81 7
11 82 7
11 83 7
11 84 7
11 85 7
11 86 7
11 87 7
11 88 8
11 89 8
11 90 8
11 91 8
11 92 8
11 93 8
11 94 8
11 95 8
11 96 8
11 97 8
11 98 8
11 99 9
11 100 9
11 101 9
11 102 9
11 103 9
11 104 9
11 105 9
11 106 9
11 107 9
11 108 9
11 109 9
11 110 10
11 111 11
11 112 12
11 113 13
11 114 14
11 115 14
11 116 15
11 117 15
11 118 16
11 119 17
11 120 17
   \endverbatim
   </li>
  </ul>

*/
