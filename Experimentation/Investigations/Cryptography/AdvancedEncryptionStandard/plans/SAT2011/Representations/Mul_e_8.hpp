// Matthew Gwynne, 22.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_e_8.hpp
  \brief Investigations into AES field multiplication by arbitary elements (e) in the 8-bit field


  \todo Basic data
  <ul>
   <li> The CNF-files "AES_byte_field_mul_full_${e}.cnf" are created by the 
   Maxima-function output_rijnmult_fullcnf_stdname(e); in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac,
   which are full clause-sets with 16 variables and 2^16 - 2^8 = 65280 
   clauses:
   \verbatim
maxima> for e : 1 thru 255 do output_rijnmult_fullcnf_stdname(e);
   \endverbatim
   Note we investigate the multiplications which are permutations (i.e., not 
   00) here.
   </li>
   <li> Computing the prime implicates statistics:
   \verbatim
shell> for e in `seq 1 255`; do 
  mkdir AES_byte_field_mul_full_${e}; 
  cd AES_byte_field_mul_full_${e}; 
  QuineMcCluskeySubsumptionHypergraphFullStatistics-n16-O3-DNDEBUG ../AES_byte_field_mul_full_${e}.cnf f; 
  mv AES_byte_field_mul_full_${e}.cnf_primes_stats prime_stats; 
  mv AES_byte_field_mul_full_${e}.cnf_shg_stats shg_stats; 
  cd ../; 
done
   \endverbatim
   yields the prime implicate statistics in each AES_byte_field_mul_full_${e}
   directory in "prime_stats" files.
   </li>
   <li> We then read in the data row by row from all the statistics files 
   using "read_experiment_dirs" into one data.frame. As this data frame has a 
   row for each clause length and multiplication, we then run 
   "rows2columns_df" to clean this up and create a data.frame with one column
   for each clause length and a row per multiplication. The values in this 
   data.frame are then the number of each clause length for each 
   multiplication.
   \verbatim
R> E = read_experiment_dirs("AES_byte_field_mul_full", list("e"), "prime_stats", header=TRUE, skip=2)
R> E2 = rows2columns_df(E, "length", "count", list("e"))
R> summary(E2)
<edited>
       2                3                4        
 Min.   : 0.000   Min.   : 0.000   Min.   :  0.0  
 1st Qu.: 0.000   1st Qu.: 0.000   1st Qu.: 40.0  
 Median : 0.000   Median : 4.000   Median : 48.0  
 Mean   : 0.502   Mean   : 7.027   Mean   : 50.7  
 3rd Qu.: 0.000   3rd Qu.:12.000   3rd Qu.: 56.0  
 Max.   :16.000   Max.   :28.000   Max.   :136.0  
                                                                          
       5               6              7              8              9       
 Min.   :  0.0   Min.   :   0   Min.   :   0   Min.   :   0   Min.   :   0  
 1st Qu.:208.0   1st Qu.: 800   1st Qu.:2176   1st Qu.:3456   1st Qu.:3328  
 Median :256.0   Median : 960   Median :2624   Median :4864   Median :4864  
 Mean   :254.6   Mean   : 916   Mean   :2442   Mean   :4479   Mean   :4548  
 3rd Qu.:320.0   3rd Qu.:1088   3rd Qu.:2944   3rd Qu.:5696   3rd Qu.:5888  
 Max.   :416.0   Max.   :1536   Max.   :3584   Max.   :7168   Max.   :9216
   \endverbatim
   </li>
   <li> From this data.frame, we can then compute the total number of prime
   implicates for each multiplication by taking the sum of all the 
   clause-lengths in each row:
   \verbatim
R> sizes = unlist(Map(function(i) sum(E2[i,1:17]),1:254))
R> summary(sizes)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max. 
     16   11070   14200   12700   15860   19230
   \endverbatim
   </li>
   <li> The number of prime implicates is very evenly spread:
   \verbatim
R> table(sizes)
   16    58   274   868  1724  2768  2880  2942  3018  4450  5048  6068  6104 
    1     2     2     2     2     2     4     2     2     2     2     2     2 
 6368  6396  6806  7120  7688  7840  8330  8358  8416  8636  8962  9220  9848 
    2     2     4     2     2     2     2     2     2     2     2     2     2 
 9968 10080 10248 11056 11068 11116 11328 11636 11764 11996 12108 12228 12320 
    2     2     2     2     2     2     2     2     2     2     2     2     2 
12324 12384 12508 12764 12836 12928 12942 13104 13252 13300 13320 13456 13508 
    2     2     2     2     2     2     4     2     2     2     2     2     2 
13680 13744 13852 13924 13936 13984 14048 14180 14200 14300 14316 14400 14552 
    2     4     2     2     2     2     2     2     2     2     2     2     2 
14596 14664 14692 14696 14760 14768 14828 14848 15008 15072 15208 15232 15236 
    4     2     2     2     2     2     2     2     2     2     2     2     2 
15252 15312 15352 15368 15376 15476 15508 15736 15744 15756 15804 15848 15868 
    2     2     2     2     4     2     2     2     2     2     2     2     4 
15884 15912 15920 15968 15984 15996 16008 16112 16124 16204 16376 16392 16412 
    2     2     2     4     2     2     2     4     2     2     2     2     2 
16532 16552 16560 16568 16664 16732 16768 16856 16968 17448 17664 18336 18848 
    2     2     2     2     2     2     2     2     2     2     2     2     4 
19232 
    2 
   \endverbatim
   It always seems to jump by around 500-1000, and most have two functions
   with that number of prime implicates (which must be each field element and
   their inverse). Only the identity has one
   </li>
   <li> As an example of one of these groups of four equally sized sets of
   prime implicates, we have:
   \verbatim
R> E2[sizes == 2880,]
     0 1 2  3  4   5   6   7   8    9 10 11 12 13 14 15 16   e
2670 0 0 4 12 48 224 352 704 512 1024  0  0  0  0  0  0  0 241
2738 0 0 4 12 48 224 352 704 512 1024  0  0  0  0  0  0  0 245
3112 0 0 4 12 48 224 352 704 512 1024  0  0  0  0  0  0  0  35
3775 0 0 4 12 48 224 352 704 512 1024  0  0  0  0  0  0  0  70
   \endverbatim
   where we have:
   <ul>
    <li> nat2poly(241,2) = x^7+x^6+x^5+x^4+1 . </li>
    <li> nat2poly(245,2) = x^7+x^6+x^5+x^4+x^2+1 . </li>
    <li> nat2poly(35) = x^5+x+1 (inverse of 241). </li>
    <li> nat2poly(70) = x^6+x^2+x (inverse of 245) . </li>
   </ul>
   Is there a simple relation here?
   </li>
   <li> Is there are relation to the Klein four-group? We should consider
   what clause-set isomorphisms we get here. </li>
   <li> The number of each clause count is exactly the same for 241,245,35 and
   70, and also for all those multiplications which share their number of
   prime implicates with 3 others, except:
   \verbatim
R> E2[sizes == 13744,]
     0 1 2 3  4   5   6    7    8    9 10 11 12 13 14 15 16   e
2279 0 0 0 0 64 368 768 2560 5888 4096  0  0  0  0  0  0  0  22
2313 0 0 0 8 40 256 960 2880 5248 4352  0  0  0  0  0  0  0 221
2806 0 0 0 8 40 256 960 2880 5248 4352  0  0  0  0  0  0  0 248
4268 0 0 0 0 64 368 768 2560 5888 4096  0  0  0  0  0  0  0  96
R> E2[sizes == 14596,]
     0 1 2 3  4   5   6    7    8    9 10 11 12 13 14 15 16   e
817  0 0 0 4 64 320 768 2176 6144 5120  0  0  0  0  0  0  0 142
1616 0 0 0 4 64 320 768 2176 6144 5120  0  0  0  0  0  0  0 185
2704 0 0 0 4 48 336 960 2496 5120 5632  0  0  0  0  0  0  0 242
3520 0 0 0 4 48 336 960 2496 5120 5632  0  0  0  0  0  0  0  56
R> E2[sizes == 15868,]
     0 1 2 3  4   5    6    7    8    9 10 11 12 13 14 15 16   e
1667 0 0 0 4 40 304  928 2688 6272 5632  0  0  0  0  0  0  0 188
1684 0 0 0 4 40 304  928 2688 6272 5632  0  0  0  0  0  0  0 189
3894 0 0 0 4 56 192 1024 3456 4992 6144  0  0  0  0  0  0  0  76
4047 0 0 0 4 56 192 1024 3456 4992 6144  0  0  0  0  0  0  0  84
   \endverbatim
   </li>
   <li> What is special about these cases? </li>
   <li> It seems likely that there is a simple rule for the prime implicates
   for the field multiplication operations (or for all linear maps). </li>
   <li> Interestingly here, we seem to get only very small numbers of prime 
   implicates compared to the random linear maps in 
   "Affine bijections over ZZ_2" at
   Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp .
   </li>
  </ul>  

*/
