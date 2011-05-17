// Matthew Gwynne, 21.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/BooleanFunctions.hpp
  \brief On investigations regarding arbitary boolean functions

  Investigations in this file are those that are too general to go into more
  specific files.


  \todo Random boolean functions
  <ul>
   <li> Considering random boolean functions on 10 variables:
   \verbatim
shell> OKlib/Experimentation/Investigations/BooleanFunctions/analyse_random_boolean_functions 10 1
^C
   \endverbatim
   and then in R:
   \verbatim
R> E = read_experiment_dirs("random_bf", list("n","seed"), "Random_bf_10.cnf_primes_stats", header=TRUE, skip=2)
R> E2 = rows2columns_df(E, "length", "count", list("n","seed"))
R> summary(E2)
       6                7                8             9
 Min.   :0.0000   Min.   :  9.00   Min.   :297   Min.   : 33.00
 1st Qu.:0.0000   1st Qu.: 44.00   1st Qu.:403   1st Qu.: 82.00
 Median :0.0000   Median : 57.00   Median :428   Median : 95.00
 Mean   :0.2071   Mean   : 58.71   Mean   :429   Mean   : 96.14
 3rd Qu.:0.0000   3rd Qu.: 70.00   3rd Qu.:454   3rd Qu.:109.00
 Max.   :5.0000   Max.   :172.00   Max.   :597   Max.   :179.00
       10          n
 Min.   :0.0000   10:9999
 1st Qu.:0.0000
 Median :0.0000
 Mean   :0.4988
 3rd Qu.:1.0000
 Max.   :6.0000
R> n=9999
R> sizes = unlist(Map(function(i) sum(ET[i,1:11]),1:n))
R> min(sizes)
[1] 464
> mean(sizes)
[1] 584.5926
R> max(sizes)
[1] 726
   \endverbatim
   </li>
   <li> Compared to the DES Sboxes there are far fewer prime implicates for
   these random 10-bit boolean functions, ~585 here compared with >1500 prime
   implicates for DES Sboxes
   (see
   Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp).
   </li>
   <li> Considering random boolean functions on 16 variables:
   \verbatim
shell> OKlib/Experimentation/Investigations/BooleanFunctions/analyse_random_boolean_functions 16 1
^C
   \endverbatim
   and then in R:
   \verbatim
R> E = read_experiment_dirs("random_bf", list("n","seed"), "Random_bf_16.cnf_primes_stats", header=TRUE, skip=2)
R> E2 = rows2columns_df(E, "length", "count", list("n","seed"))
R> summary(E2)
        11
 Min.   :0.000000
 1st Qu.:0.000000
 Median :0.000000
 Mean   :0.002573
 3rd Qu.:0.000000
 Max.   :1.000000

       12              13              14              15
 Min.   : 76.0   Min.   :15326   Min.   :48167   Min.   :1441
 1st Qu.:104.0   1st Qu.:16625   1st Qu.:49431   1st Qu.:1701
 Median :114.0   Median :17020   Median :49779   Median :1749
 Mean   :114.7   Mean   :17027   Mean   :49774   Mean   :1752
 3rd Qu.:125.0   3rd Qu.:17401   3rd Qu.:50102   3rd Qu.:1805
 Max.   :172.0   Max.   :19054   Max.   :51272   Max.   :2041

       16          n
 Min.   :0.0000   16:1166
 1st Qu.:0.0000
 Median :0.0000
 Mean   :0.5189
 3rd Qu.:1.0000
 Max.   :4.0000

R> sizes = unlist(Map(function(i) sum(E2[i,1:17]),1:2712))
R> min(sizes)
[1] 66820
R> max(sizes)
[1] 70541
   \endverbatim
   </li>
   <li> Compared to random permutations in 
   Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp, 
   there are fewer prime implicates overall (66800-71000 compared
   with 124000-149000), and the prime implicates for the random boolean
   functions are larger (here all have at least 11 literals, while there
   none has more than 10). </li>
  </ul>


  \todo Random boolean functions in 10 variables with 64 true points
  <ul>
   <li> A better comparison with DES Sbox in 10 variables is
   given by random boolean functions with (exactly) 64 true points. </li>
   <li> The above random boolean functions on 10 variables have on average
   512 true points. </li>
  </ul>


  \todo Random boolean functions in 16 variables with 256 true points
  <ul>
   <li> A better comparison with random permutations in 16 variables is
   given by random boolean functions with (exactly) 256 true points. </li>
   <li> The above random boolean functions have on average 32768 true points.
   </li>
  </ul>

*/

