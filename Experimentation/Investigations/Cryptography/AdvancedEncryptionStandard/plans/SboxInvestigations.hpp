// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SboxInvestigations.hpp
  \brief On investigations into the AES Sbox


  \todo Connections
  <ul>
   <li> See Investigations/BooleanFunctions/plans/Permutations.hpp for
   general investigations on permutations of {0,1}^n. </li>
  </ul>


  \todo Investigate CNF representations
  <ul>
   <li> One needs to systematically explore CNF representations, with and
   without new variables. </li>
   <li> First to study the set of prime implicates.
    <ol>
     <li> Basic statistics? </li>
     <li> Necessary clauses? </li>
     <li> Minimum representations? </li>
     <li> r-bases for r in {r_1,r_2}? (See rand_rbase_cs(F,r) in
     ComputerAlgebra/Satisfiability/Lisp/Primality/RBases.mac.) </li>
    </ol>
   </li>
  </ul>


  \todo Understanding prime implicates after any partial assignment
  <ul>
   <li> To consider the AES Sbox as an "active clause", we want to first be 
   able, given a partial assignment, to infer as many forced assignments
   as possible. This can be done simply with the DNF representation. </li>
   <li> However, secondly one needs, given a partial assignment, to be able to
   determine various measures for heuristics. </li>
   <li> Therefore, investigating several statistics (most notably the number of
   clauses for a given variable) of the prime implicates of the clause-set, 
   formed after taking the Sbox and applying each partial assignment, is 
   necessary to try and discern a pattern. </li>
   <li> If such patterns can be deduced for particular clause-set measures,
   then the active clause can use this pattern, given a partial assignment, 
   to return reasonable values for these measures which can be used for 
   statistics. </li>
   <li> A C++ implementation of such a system whereby the set of prime 
   implicates is taken as input, and each partial assignment along with
   the relevant statistics is returned is necessary. </li>
   <li> Such a C++ implementation would need to be able to apply a partial
   assignment to a clause-set and then compute various statistics on the 
   result. This would need to be done for every partial assignment. </li>
   <li> After applying the partial assignment, to gain the prime implicates
   of the new boolean function, one must simply apply subsumption elimination
   to the new clause-set (which is just result of applying a partial assignment
   to the prime implicates of the original function). This can be done using 
   functionality already in the library (MG: Where?). </li>
  </ul>


  \todo Investigating Sbox given 8 bit input
  <ul>
   <li> Given 8 bits input to the Sbox, the output should be immediately 
   determined without any decisions needed. Is this actually the case? </li>
   <li> This question is motivated by the fact that the full AES translation
   using a representation of the sbox and field multiplications without new 
   variables seems to make some decisions when given all the input bits it 
   needs, and this should not be the case. </li>
   <li> The "default" Sbox representation with no new variables currently used 
   can be generated in the following way:
   \verbatim
h2_aes : rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit_tb(4,4))$
p2_aes : replace_by_prime_implicates_hitting(h2_aes)$
Sbox44ICCNF : first_irr_fcs(cs_to_fcs(p2_aes), dll_simplest_trivial2)$
   \endverbatim
   In the following, we shall call this Sbox representation Sbox44ICCNF.
   </li>
   <li> Looking at this in maxima, with all input bits set to 0:
   \verbatim
(%i7) apply_pa({-1,-2,-3,-4,-5,-6,-7,-8}, Sbox44ICCNF[2]);
Evaluation took 0.0200 seconds (0.0200 elapsed)
(%o7) {{-16,-15,-14,-13,11,12},{-16,-14,-12,11,13},{-16,-14,-11,15},{-16,-13,11},{-16,-12,15},
       {-16,-11,-9},{-16,-10,11,14},{-16,9,10},{-16,13,15},{-15,-14,-13,11,16},{-15,-14,11,13},
       {-15,-13,-12,11,16},{-15,-13,9,16},{-15,-13,10,16},{-15,-12,-10,11,13},{-15,-12,-9,11,14},
       {-15,-12,11,13,16},{-15,-11,10},{-15,-10,12,13,14,16},{-15,-9,10,11,12},{-15,9,10,14},
       {-15,9,11,14},{-15,10,12,16},{-14,-13,9,11},{-14,-12,-11,10},{-14,-12,-9,13,15},
       {-14,-12,9,10},{-14,-12,9,11,15},{-14,-12,9,11,16},{-14,-12,11},{-14,-11,-9,12,16},
       {-14,-9,10,11,16},{-14,9},{-14,9,10},{-14,9,15},{-14,10,16},{-14,11,16},{-14,12,16},
       {-14,13,16},{-13,-12,-11,14,16},{-13,-12,-9,14},{-13,-12,9},{-13,9,10,12,15},{-13,9,11,14},
       {-13,9,14},{-13,10,16},{-13,12,15,16},{-12,-11,10,13,14,16},{-12,9,10,11,15},{-12,9,11,15},
       {-12,9,13,14},{-11,-9,10,16},{-11,-9,14,15,16},{-11,9,10,14,15},{-11,10,14,15},{-11,14,16},
       {-10,-9,11,14},{-10,9,14,16},{-10,16},{-9,10,11},{-9,10,12,13},{-9,11,12,15},{-9,14,15},
       {-9,14,15,16},{-9,15,16},{9,10,12,13,16},{9,10,12,14},{9,11,12,13,14,15},{9,11,12,15,16},
       {9,11,13},{9,12,14,16},{9,12,16},{9,13,14,16},{10,11,12,14},{10,11,13,14,15,16},{10,12,15},
       {10,13,16},{10,14},{11,12,16},{11,13,14},{11,15,16}}
(%i8) statistics_cs(apply_pa({-1,-2,-3,-4,-5,-6,-7,-8}, Sbox44ICCNF[2]));
Evaluation took 0.0280 seconds (0.0270 elapsed)
(%o8) [8,81,320,6,2]
   \endverbatim
   </li>
   <li> With several different assignments in maxima: 
   \verbatim
(%i5) statistics_cs(apply_pa({-1,-2,-3,-4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0270 elapsed)
(%o5) [8,81,320,6,2]
(%i6) statistics_cs(apply_pa({1,-2,-3,-4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0280 seconds (0.0260 elapsed)
(%o6) [8,81,320,6,2]
(%i7) statistics_cs(apply_pa({-1,2,-3,-4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0280 seconds (0.0250 elapsed)
(%o7) [8,69,272,6,2]
(%i8) statistics_cs(apply_pa({-1,-2,3,-4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0250 elapsed)
(%o8) [8,76,288,6,1]
(%i9) statistics_cs(apply_pa({-1,-2,-3,4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0260 elapsed)
(%o9) [8,74,298,6,2]
(%i10) statistics_cs(apply_pa({-1,-2,-3,-4,5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0240 elapsed)
(%o10) [8,68,264,6,2]
(%i11) statistics_cs(apply_pa({-1,-2,-3,-4,-5,6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0260 elapsed)
(%o11) [8,81,319,6,1]
(%i12) statistics_cs(apply_pa({-1,-2,-3,-4,-5,-6,7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0280 seconds (0.0270 elapsed)
(%o12) [8,84,324,6,1]
(%i13) statistics_cs(apply_pa({-1,-2,-3,-4,-5,-6,-7,8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0250 elapsed)
(%o13) [8,73,287,6,2]
   \endverbatim
   </li>
   <li> Setting all 8 input bits (variables 1 to 8) to 0 (by adding unit 
   clauses -1 ... -8) for Sbox44CNF: 
   \verbatim
[12:53:58 - aeternus] data$ ./minisat/minisat/core/minisat test.cnf 
restarts              : 1
conflicts             : 0              (nan /sec)
decisions             : 2              (0.00 % random) (inf /sec)
propagations          : 16             (inf /sec)
conflict literals     : 0              ( nan % deleted)
CPU time              : 0 s
SATISFIABLE
   \endverbatim
   As can be seen two decisions are made (but no conflicts).
   </li>
   <li> Setting all inputs bits to 1 for Sbox44CNF :
   \verbatim
[12:55:45 - aeternus] data$ ./minisat/minisat/core/minisat test.cnf 
restarts              : 1
conflicts             : 3              (750 /sec)
decisions             : 7              (0.00 % random) (1750 /sec)
propagations          : 26             (6500 /sec)
conflict literals     : 10             (9.09 % deleted)
CPU time              : 0.004 s
SATISFIABLE
   \endverbatim
   As can be seen, in this case, 3 conflicts occur and 7 decisions are made.
   </li>
   <li> Whether or not unit clauses occur immediately after setting all Sbox
   input bits (using Sbox44ICCNF) depends on the assignment. </li>
   <li> A better Sbox CNF representation seems to be needed. </li>
   <li> However, the primary goal is for problems where the key is not known,
   not simple encryption and decryption, so further insight into where the
   Sbox is used and exactly what bits are commonly set and that we wish to
   allow easier deductions with. </li>
  </ul>


  \todo Minimisation of the Sbox
  <ul>
   <li> See "Minimisation" in 
   OKlib/Satisfiability/FiniteFunctions/plans/general.hpp . </li> 
   <li> R QCA packages 
   <ul>
    <li> We can use the QCA package, given in 
    Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp to compute
    the minimum sized CNF or DNF clause-set representation. </li>
    <li> This should be possible using the following code:
    \verbatim
######## In Maxima #######
generate_full_aes_sbox_tt() :=  
  map(
     lambda([ce],
       append(
         int2polyadic_padd(ce[1],2,8),
         int2polyadic_padd(ce[2],2,8),
         if rijn_lookup_sbox(ce[1]) = ce[2] then [1] else [0]))
     ,cartesian_product(setmn(0,255),setmn(0,255)))$

with_stdout("Sbox.tt", block(
  apply(print, endcons("O",create_list(i,i,1,16))),
  for tt_line in generate_full_aes_sbox_tt() do
    apply(print,tt_line)
  ))$

######## In R ###########

oklib_load_all()
library(QCA)

sbox_tt = read.table("Sbox.tt",header=TRUE)
eqmcc(sbox_tt, outcome="O", expl.0=TRUE)
   \endverbatim
   although currently there are issues with memory (see "Minimisation in
   OKlib/Satisfiability/FiniteFunctions/plans/general.hpp). </li>  
  </ul>
  </li>
  <li> Espresso-ab (see Logic "synthesis" in
  Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp)
  <ul>
   <li> Espresso-ab takes as input a truth table in PLA format. </li>
   <li> We can generate a truth table in PLA format for the Sbox in the
   following way:
   \verbatim
generate_full_aes_sbox_tt() :=  
  map(
     lambda([ce],
       append(
         int2polyadic_padd(ce[1],2,8),
         int2polyadic_padd(ce[2],2,8),
         if rijn_lookup_sbox_nat(ce[1]) = ce[2] then [1] else [0]))
     ,cartesian_product(setmn(0,255),setmn(0,255)))$

with_stdout("Sbox.pla", block(
  print(".i 16"),
  print(".o 1"),
  for tt_line in generate_full_aes_sbox_tt() do
    print(apply(sconcat,rest(tt_line,-1)),1-last(tt_line))
  ))$
   \endverbatim
   where the PLA file will be called "Sbox.pla", and will be represented
   as a DNF representing the negation of the Sbox (as by default Espresso
   minimises DNF formulas.
   </li>
   <li> Running espresso-ab with the standard options yielded a minimal DNF 
   representing the negation of the Sbox with 354 clauses. This is much 
   smaller than the Sbox found in XXX with 529 clauses. </li>
   <li> Espresso-ab by default uses heuristical methods however, and so it
   is not clear how close to the minimum size clause-set 354 clauses is. 
   </li>
   <li> Espresso-ab does however have additional options to allow exact
   minimisation including "exact" and "signature" options representing
   algorithms discussed in [BraytonMcGeerSanghaviVincentelli 1993], which can
   be run by using the "-Dexact" and "-Dsignature" options. </li>
   <li> Using the "exact" and "signature" algorithms, espresso-ab runs out of 
   memory on an 8GB machine (it is killed by the linux OOM killer). </li>
  </ul>
  </li>
  <li> Scherzo (see "Logic synthesis" in
  Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp)
  <ul>
   <li> Scherzo uses implicit BDD representations for sets of prime implicates
   and therefore offers the opportunity to minimise the Sbox without running 
   out of memory. </li>
   <li> See "Logic synthesis" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp for 
   issues relating to the building and running of Scherzo. 
   </li>
  </ul>
  </li>
 </ul>

*/
