// Matthew Gwynne, 20.6.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_e_4.hpp
  \brief Investigations into AES field multiplication by arbitary elements (e) in the 4-bit field


  \todo Basic data
  <ul>
   <li> The CNF-files "ss_byte2_4_field_mul_full_${e}.cnf" are created by the
   Maxima-function output_rijnmult_fullcnf_stdname(e); in
   Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac,
   which are full clause-sets with 16 variables and 2^8 - 2^4 = 240
   clauses:
   \verbatim
maxima> for e : 1 thru 15 do output_ssmult_fullcnf_stdname(e,2,4,ss_polynomial_2_4);
   \endverbatim
   Note we investigate the multiplications which are permutations (i.e., not
   00) here.
   </li>
   <li> Computing the prime implicates statistics:
   \verbatim
maxima> for i : 1 thru 15 do output_ssmult_fullcnf_stdname(i, 2, 4, ss_polynomial_2_4);
shell> for i in $(seq 1 15); do echo "# Multiplication by ${i}"; echo; QuineMcCluskey-n16-O3-DNDEBUG ss_byte2_4_field_mul_full_${i}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG; echo; done

# Multiplication by 1

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8       8      8      8       8       16     NA     NA       8       16     1
 length   count
      2       8

# Multiplication by 2

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8      14      8      8      14       36     NA     NA      14       36     1
 length   count
      2       6
      3       8

# Multiplication by 3

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8     120      8      8     120      528     NA     NA     120      528     1
 length   count
      3      16
      4      40
      5      64

# Multiplication by 4

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8      32      8      8      32      108     NA     NA      32      108     1
 length   count
      2       4
      3      12
      4      16

# Multiplication by 5

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8     120      8      8     120      528     NA     NA     120      528     1
 length   count
      3      16
      4      40
      5      64

# Multiplication by 6

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8     132      8      8     132      580     NA     NA     132      580     1
 length   count
      3      12
      4      56
      5      64

# Multiplication by 7

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8     132      8      8     132      580     NA     NA     132      580     1
 length   count
      3      12
      4      56
      5      64

# Multiplication by 8

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8      74      8      8      74      308     NA     NA      74      308     1
 length   count
      2       2
      3      16
      4      24
      5      32

# Multiplication by 9

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8      14      8      8      14       36     NA     NA      14       36     1
 length   count
      2       6
      3       8

# Multiplication by 10

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8     120      8      8     120      528     NA     NA     120      528     1
 length   count
      3      16
      4      40
      5      64

# Multiplication by 11

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8     120      8      8     120      528     NA     NA     120      528     1
 length   count
      3      16
      4      40
      5      64

# Multiplication by 12

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8     120      8      8     120      528     NA     NA     120      528     1
 length   count
      3      16
      4      40
      5      64

# Multiplication by 13

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8      32      8      8      32      108     NA     NA      32      108     1
 length   count
      2       4
      3      12
      4      16

# Multiplication by 14

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8     120      8      8     120      528     NA     NA     120      528     1
 length   count
      3      16
      4      40
      5      64

# Multiplication by 15

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      8      74      8      8      74      308     NA     NA      74      308     1
 length   count
      2       2
      3      16
      4      24
      5      32
   \endverbatim
   </li>
   <li> Computing minimum representations:
    <ul>
     <li> Computed using
     Satisfiability/Optimisation/minimise_cnf_cryptominisat. </li>
     <li> Statistics:
     \verbatim
maxima> for i : 2 thru 15 do print(i, ":", statistics_fcs(ev_hm(ss_field_cnfs,[4,i])));

2 : [8,9,22,3,2]
3 : [8,16,52,4,3]
4 : [8,11,30,3,2]
5 : [8,16,56,4,3]
6 : [8,16,60,4,3]
7 : [8,16,60,4,3]
8 : [8,13,42,4,2]
9 : [8,9,22,3,2]
10 : [8,16,52,4,3]
11 : [8,16,52,4,3]
12 : [8,16,52,4,3]
13 : [8,11,30,3,2]
14 : [8,16,52,4,3]
15 : [8,13,42,4,2]
     \endverbatim
     </li>
     <li> The hardness of all minimum representations of the AES 4-bit field
     multiplications is always <=3:
      <ul>
       <li> For the definition and computation of hardness in general, see
       "Hardness of boolean function representations" in
       Experimentation/Investigations/BooleanFunctions/plans/general.hpp.
       </li>
       <li> Computing the hardness:
       \verbatim
maxima> for i : 2 thru 15 do print(sconcat("hd(",i, ") = ", hardness_cs(setify(ev_hm(ss_field_cnfs,[4,i])[2]))));
hd(2) = 2
hd(3) = 2
hd(4) = 2
hd(5) = 2
hd(6) = 3
hd(7) = 3
hd(8) = 2
hd(9) = 2
hd(10) = 2
hd(11) = 2
hd(12) = 2
hd(13) = 2
hd(14) = 2
hd(15) = 2
maxima> setify(create_list(hardness_cs(setify(ev_hm(ss_field_cnfs,[4,i])[2])),i,2,15));
{2,3}
       \endverbatim
       </li>
       <li> The minimum representations that we have so far considered, for
       the 4-bit multiplications by constants in {2,...,15} have hardness in
       {2,3}. </li>
       <li> We must investigate *all* minimum representations of the 4-bit
       multiplications by constants in {2,...,15}. </li>
       <li> The hardness for all minimum representations is known for the
       4-bit multiplication by 02, discussed in
       "Generating all minimum representations via hypergraph transversals"
       below. </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo Generating all minimum representations via hypergraph transversals
  <ul>
   <li> Computing all minimum CNFs:
    <ul>
     <li> Generate full CNFs:
     \verbatim
maxima> for i : 1 thru 16 do output_ssmult_fullcnf_stdname(i,2,4,ss_polynomial_2_4);
     \endverbatim
     </li>
     <li> Minimum representations for multiplication by 02:
     \verbatim
shell> ${OKPLATFORM}/OKsystem/OKlib/Satisfiability/Optimisation/all_minimum_cnf AES_byte_field_mul_full_2.cnf
shell> ls -1  AllMinimumCNFs_ss_byte2_4_field_mul_full_2_2011-09-28-115851/MinCNFs/ | wc -l
2
maxima> oklib_load_all()$
maxima> F : read_fcl_f("AllMinimumCNFs_ss_byte2_4_field_mul_full_2_2011-09-28-115851/MinCNFs/1.cnf");

   [[1,2,3,4,5,6,7,8],
    [{-5,2},{-2,5},{-6,3},{-3,6},{-7,-4,-1},{-1,4,7},{-8,1},{-7,4,8},{-4,7,8}]]

maxima> F2 : read_fcl_f("AllMinimumCNFs_ss_byte2_4_field_mul_full_2_2011-09-28-115851/MinCNFs/2.cnf");

   [[1,2,3,4,5,6,7,8],
    [{-5,2},{-2,5},{-6,3},{-3,6},{-7,1,4},{-4,1,7},{-8,-7,-4},{-8,4,7},{-1,8}]]

maxima> setify(map(hardness_cs, [setify(F[2]),setify(F2[2])]));
{2}
     \endverbatim
     </li>
     <li> Minimum representations for multiplication by 03:
     \verbatim
shell> ${OKPLATFORM}/OKsystem/OKlib/Satisfiability/Optimisation/all_minimum_cnf AES_byte_field_mul_full_3.cnf
shell> ls -1  AllMinimumCNFs_ss_byte2_4_field_mul_full_3_2011-09-28-161343/MinCNFs/ | wc -l
33
maxima> oklib_load_all()$
maxima> F_min_3_l : create_list(read_fcl_f(sconcat("AllMinimumCNFs_ss_byte2_4_field_mul_full_3_2011-09-28-161343/MinCNFs/",i,".cnf")),i,1,33);
maxima> setify(map(lambda([FF], hardness_cs(setify(FF[2]))), F_min_3_l));
{2}
     \endverbatim
     </li>
     <li> We need this data for all multiplications. </li>
     <li> There are also todos on improving the minimisation scripts,
     discussed in "Improve minimisation scripts" in
     Satisfiability/Optimisation/plans/general.hpp. </li>
    </ul>
   </li>
  </ul>

*/
