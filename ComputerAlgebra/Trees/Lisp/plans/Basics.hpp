// Oliver Kullmann, 3.9.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Trees/Lisp/plans/Basics.hpp
  \brief Plans regarding basic tree-functionality


  \todo rt2lrt_il
  <ul>
   <li> A more intelligent and more efficient implementation is needed. </li>
   <li> Currently the effort is quadratic, while it should be linear. </li>
  </ul>


  \todo Generating all binary trees
  <ul>
   <li> Why is the formula num_all2i_rt(i) = catalan_number(i) correct? </li>
  </ul>


  \todo Catalan numbers
  <ul>
   <li> Perhaps the function catalan_number should move to supermodule
   NumberTheory? </li>
  </ul>


  \todo Random binary trees
  <ul>
   <li> Write tests for random2_rt. </li>
   <li> Experiment for determining whether this process might establish fair
   sampling of all binary trees with m nodes:
   \verbatim
test_random2_rt(m,N) := block([h : sm2hm({})],
 thru N do
    enter_new_occurrence(h,random2_rt(m)),
 map(second,get_distribution(h)))$

test_random2_rt(9,100000);
 [4058,4140,8282,4189,4145,12370,12471,12557,4285,4228,12494,8447,4157,4177]
test_random2_rt(13,1000000);
Evaluation took 1234.3570 seconds (2053.0710 elapsed)
(%o77) [1373,1391,2836,1405,1398,4209,4216,4222,1365,1316,4157,2744,1388,1372,5667,5487,11218,5583,
        5509,8330,8360,5523,1369,1383,5536,2799,1393,1419,8457,8361,11193,4168,4097,5376,4235,1401,
        1371,5532,4216,2796,1379,1368,6879,6871,13862,6997,6869,21205,20919,20982,6795,6967,20948,
        13956,6965,6881,14063,13844,27861,13502,13796,13922,13884,6962,1383,1363,6947,2790,1347,
        1350,13895,13935,13864,4135,4132,6993,4121,1418,1387,6918,4010,2760,1420,1382,13851,13809,
        28067,13848,13617,27968,27652,20802,5470,5499,20632,11203,5552,5513,13808,13870,21116,8454,
        8396,6874,5603,1380,1355,6819,5548,2830,1399,1405,13925,13972,20987,8306,8354,13727,11031,
        4210,4207,6911,5468,4190,1385,1397,6846,5542,4209,2781,1412,1402]
   \endverbatim
   So it does not establish the uniform distribution. </li>
   <li> So for m=9 there are three types of trees, while for m=13 there appear
   to be ten. </li>
  </ul>

*/
