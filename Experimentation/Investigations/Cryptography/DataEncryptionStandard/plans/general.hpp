// Oliver Kullmann, 8.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/general.hpp
  \brief On investigations into the Data Encryption Standard


  \todo Understanding DES
  <ul>
   <li> First implementations of encryption/decryption at the Maxima-level
   are needed. </li>
   <li> At least in two versions, as follows:
    <ol>
     <li> Following the specific descriptions of the various permutations
     and S-boxes. </li>
     <li> Emphasising the abstract flow information (abstracting away of the
     specific nature of the special functions involved). </li>
    </ol>
   </li>
   <li> We also need to think about different organisations of the round.
   Perhaps the key-involvement could happen at the beginning or end of a round,
   instead of in the middle. </li>
   <li> See 
   Experimentation/Investigations/Cryptography/DataEncryptionStandard/plans/Representations/Sboxes.hpp
   for investigations into the DES Sboxes. </li>
  </ul>


  \todo Basic translation
  <ul>
   <li> Just following the definitions, of course "ignoring" the permutations,
   that is, they just are interpreted as "rewirings". </li>
   <li> Since the key schedule thus now is basically trivial (only input key
   bits are used), all what remains are the S-boxes and binary additions. </li>
   <li> The S-boxes have 6 input and 4 output bits, and thus yield 10-bit
   boolean functions. </li>
   <li> These can be analysed completely, and we can study the various
   representations (at least minimum, canonical, r_0- and r_1-bases). </li>
   <li> Do we need also to include decryption? Can the basic scheme encrypt
   and decrypt by just unit-clause propagation? </li>
   <li> We should obtain a considerably smaller formula than what Massacci and
   Marraro obtained. To be comparable, for the minimum translation we also need
   two versions: one treating an S-box as one 6-to-4 bit function, and one
   treating it as 4 6-bit functions. </li>
   <li> The canonical translation for the 6-to-4 bit Sbox contains
   64 new variables and 705 clauses:
   \verbatim
maxima> ncl_list_full_dualts(10,64);
[[2,640],[11,64],[64,1]]
nvar_full_dualts(10,64) - 10;
64
   \endverbatim
   </li>
   <li> Using the canonical box translation and treating the Sboxes as 6-to-4
   bit functions, the full 16 round DES will contain:
   <ul>
    <li> 64+56+9984=10104 variables: 
     <ol>
      <li> 64 variables for the input plaintext. </li>
      <li> 56 variables for the key. </li>
      <li> 16*(48+512+32+32)=9984 variables from 16 rounds consisting of:
      <ol>
       <li> 48 variables for the output of the key addition. </li>
       <li> 8*64=512 variables for the S-box representation. </li>
       <li> 32 variables for the output of Sbox substitutions. </li>
       <li> 32 variables for the output of the final addition. </li>
      </ol>
      </li>
     </ol>
    </li>
    <li> 81920+5120+8192+128=95360 clauses:
    <ol>
     <li> 16*8*640=81920 clauses of size 2
     (16 rounds * 8 Sboxes * 640 clauses = 81,920). </li>
     <li> 16*(48+32)*4=5120 clauses of size 4
     (16 rounds * (48-bit addition + 32-bit addition) * 4 clauses = 5120). 
     </li>
     <li> 16*8*64=8192 clauses of size 11
     (16 rounds * 8 Sboxes * 64 clauses = 8,192). </li>
     <li> 16*8=128 clauses of size 64
     (16 rounds * 8 Sboxes * 1 clause = 128). </li>
    </ol>
    </li>
   </ul>
   </li>
   <li> In comparison to Massaci and Marraro, they have 61,935 clauses and
   10,336 variables. </li>
   <li> Using a minimum-representation for the S-boxes, we get:
    <ol>
     <li> 64+56+16*(48+32+32)=1912 variables. </li>
     <li> If a minimum representation of an S-box (a 10-bit boolean function)
     needs P clauses (assuming that the eight different S-boxes share the same
     size), then 16*((48+32)*4+8*P)= 5120 + 128*P clauses are used. </li>
     <li> We should have P <= 100, and so less than 20000 clauses are needed
     (for this smallest representation). </li>
     <li> Alternatively every S-box is represented by 4 1-bit-output functions.
     </li>
     <li> If such a (6-bit) function uses Q clauses, then
     16*((48+32)*4+8*4*Q)= 5120 + 512*Q clauses are used. </li>
     <li> Likely Q <= 30, and thus around 20000 clauses are needed. </li>
    </ol>
   </li>
  </ul>


  \todo Analysing the S-boxes
  <ul>
   <li> These considerations need a dedicated sub-module. </li>
   <li> Most urgent is to use all our instruments to analyse the 8 S-boxes.
   </li>
   <li> Of course, starting with defining them at Maxima-level. </li>
   <li> Considering them as one 10-bit boolean function, or as 4 6-bit
   boolean functions. Likely better the first view, but we need to consider
   all possibilities. </li>
   <li> Determining the number of prime implicates, data on the subsumption
   hypergraph, and minimum CNFs, and r_0-,r_1-bases. </li>
   <li> Here also minimum DNF-representations are of interest! </li>
   <li> They can be used to yield pseudo-canonical translations. </li>
   <li> Analysis of the prime clauses:
    <ol>
     <li> Creating the CNF/DNF files:
     \verbatim
OKplatform> mkdir EXP_DES
OKplatform> cd EXP_DES/
EXP_DES> oklib --maxima

oklib_load("OKlib/ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/Sboxes.mac");
for i : 1 thru 8 do (output_dessbox_fulldnf_stdname(i),output_dessbox_fullcnf_stdname(i));
     \endverbatim
     </li>
     <li> Prime-clause statistics:
     \verbatim
EXP_DES> for F in *.cnf; do QuineMcCluskeySubsumptionHypergraphFullStatistics-n16-O3-DNDEBUG ${F}; done

EXP_DES> for F in DES_Sbox_?_fullDNF.cnf_primes_stats; do cat ${F}; done

EXP_DES> for F in DES_Sbox_?_fullCNF.cnf_primes_stats; do cat ${F}; done

 n non_taut_c red_l taut_c orig_l comment_count finished_bool
10 1624 9554 0 9554 0 1
 length count
5 243
6 1328
7 53

10 1844 10967 0 10967 0 1
5 154
6 1633
7 57

10 1767 10458 0 10458 0 1
5 219
6 1473
7 75

10 1881 11197 0 11197 0 1
5 153
6 1664
7 64

10 1812 10768 0 10768 0 1
5 174
6 1568
7 70

10 1705 10115 0 10115 0 1
5 204
6 1412
7 89

10 1673 9891 0 9891 0 1
5 228
6 1364
7 81

10 2047 12227 0 12227 0 1
5 106
6 1890
7 51
     \endverbatim
     </li>
     <li> Subsumption hypergraph statistics:
     \verbatim
> sbox1_df = read.table(paste("DES_Sbox_",1,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox1_df)
     length          count      
 Min.   :10.00   Min.   : 0.00  
 1st Qu.:22.25   1st Qu.: 5.00  
 Median :34.50   Median :19.00  
 Mean   :34.50   Mean   :19.20  
 3rd Qu.:46.75   3rd Qu.:31.75  
 Max.   :59.00   Max.   :48.00
> sbox2_df = read.table(paste("DES_Sbox_",2,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox2_df)
     length          count      
 Min.   :11.00   Min.   : 0.00  
 1st Qu.:23.75   1st Qu.: 3.75  
 Median :36.50   Median :19.50  
 Mean   :36.50   Mean   :18.46  
 3rd Qu.:49.25   3rd Qu.:30.00  
 Max.   :62.00   Max.   :56.00
> sbox3_df = read.table(paste("DES_Sbox_",3,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox3_df)
     length         count      
 Min.   : 7.0   Min.   : 0.00  
 1st Qu.:20.5   1st Qu.: 3.00  
 Median :34.0   Median :17.00  
 Mean   :34.0   Mean   :17.45  
 3rd Qu.:47.5   3rd Qu.:29.50  
 Max.   :61.0   Max.   :49.00
> sbox4_df = read.table(paste("DES_Sbox_",4,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox4_df)
     length          count   
 Min.   :12.00   Min.   : 0  
 1st Qu.:23.75   1st Qu.: 7  
 Median :35.50   Median :21  
 Mean   :35.50   Mean   :20  
 3rd Qu.:47.25   3rd Qu.:32  
 Max.   :59.00   Max.   :52
> sbox5_df = read.table(paste("DES_Sbox_",5,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox5_df)
     length         count      
 Min.   :11.0   Min.   : 0.00  
 1st Qu.:23.5   1st Qu.: 5.50  
 Median :36.0   Median :19.00  
 Mean   :36.0   Mean   :18.82  
 3rd Qu.:48.5   3rd Qu.:29.00  
 Max.   :61.0   Max.   :43.00
> sbox6_df = read.table(paste("DES_Sbox_",6,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox6_df)
     length          count      
 Min.   :11.00   Min.   : 0.00  
 1st Qu.:23.75   1st Qu.: 3.75  
 Median :36.50   Median :15.00  
 Mean   :36.50   Mean   :18.46  
 3rd Qu.:49.25   3rd Qu.:31.50  
 Max.   :62.00   Max.   :56.00
> sbox7_df = read.table(paste("DES_Sbox_",7,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox7_df)
     length       count      
 Min.   :11   Min.   : 0.00  
 1st Qu.:23   1st Qu.: 4.00  
 Median :35   Median :21.00  
 Mean   :35   Mean   :19.59  
 3rd Qu.:47   3rd Qu.:30.00  
 Max.   :59   Max.   :53.00
> sbox8_df = read.table(paste("DES_Sbox_",8,"_fullCNF.cnf_shg_stats",sep=""), head=TRUE, skip=2)
> summary(sbox8_df)
     length         count      
 Min.   :12.0   Min.   : 1.00  
 1st Qu.:24.5   1st Qu.: 5.50  
 Median :37.0   Median :17.00  
 Mean   :37.0   Mean   :18.82  
 3rd Qu.:49.5   3rd Qu.:28.00  
 Max.   :62.0   Max.   :45.00
     \endverbatim
     </li>
     <li> All curves k -> nr (clause-length to number of occurrences) look
     like nice relatively symmetric curves. They have maximums around 30. 
     Some (1,2,4) look slightly concave and some look slightly convex
     (3,5,6,7,8). </li>
     <li> The DNF-output shows that no resolutions are possible, and thus these
     boolean functions have unique DNF. </li>
     <li> Quite some differences regarding the prime implicates. S-box number
     1 is the easiest, number 8 is the hardest. </li>
     <li> This is also confirmed by r_1-bases: Box 1 has an r_1-base with 126
     clauses, while for box 8 only one with 157 clauses was found. </li>
     <li> Also minimum representations need to be studied. </li>
     <li> As a model one can study random 6 x 4 boolean functions. </li>
    </ol>
   </li>
  </ul>



  \todo r_2-bases of a round
  <ul>
   <li> It seems not effective, and also not needed for obtaining an r_1-base,
   to integrate the addition of 4 (selected) bits to the S-box output,
   yielding a 14-bit boolean function: this should create a lot of clauses,
   which are also obtained, as r_1-bases, by just translating the additions.
   </li>
   <li> However on the input-side of an S-box more interesting things happen:
   4 (selected) bits yield 6 bits by partial repetition, they are added with
   6 key bits, and yield the input of the S-box. This yields also a 14-bit
   boolean function, whose use should improve the above basic translation.
   </li>
   <li> Again we consider at least minimum, canonical and r_0-, r_1-based
   representations. </li>
   <li> We should obtain r_2-based representations of a round. </li>
  </ul>


  \todo Understanding the Massacci-Marraro translation
  <ul>
   <li> The reference is [SAT 2000, editors Gent, van Maaren, Walsh, pages
   343-375]. </li>
   <li> Their benchmarks should become part of the OKlibrary (ExternalSources).
   </li>
   <li> The article speaks about the "commercial version" ? Likely this means
   just full DES? </li>
   <li> It seems superfluous variables are used, since the "superstructure",
   the permutations and the whole structure, is not trivialised, but is
   represented by "logical" formulas. </li>
   <li> On pages 356-361 one finds accordingly strange considerations regarding
   various circuits. </li>
   <li> The S-boxes:
    <ol>
     <li> On page 351, last paragraph it says that just by unit-clause
     propagation encryption can be performed. </li>
     <li> However on pages 356-357 Espresso and minimisation is mentioned,
     in two versions, considering an S-box as a 6-to-4 bit functions and
     as 4 6-bit functions. </li>
     <li> Apparently they go for a minimum representation? </li>
     <li> If they use the 6-to-4 bit representation, then the output bit
     can't be inferred. </li>
     <li> But using the 4 6-bit functions, the output bit is trivially inferred
     since it's the only variable left. </li>
     <li> So that seems to be it. </li>
     <li> However in Section 4.3 they speak of "PLAs" ("Programmable Logic
     Arrays") and of minterms (constituting the canonical DNF, consisting of
     all full DNF-clauses). </li>
     <li> We have some information on PLAs in
     Buildsystem/ExternalSources/SpecialBuilds/docus/Espresso.hpp. We need
     to have full information there. </li>
     <li> Then Espresso is used to minimise some PLAs. They speak actually
     again of "minterms", where they could "obtain a further improvement
     in terms of the number of minterms associated with each output"?
     When using (only) minterms, there is no choice! </li>
     <li> Since in Figure 3 in the article a disjunction of minterms is used,
     perhaps they use roughly the canonical translation for the full DNF? </li>
    </ol>
   </li>
   <li> Overall one sees with that article the fundamental problem of "logic
   cryptoanalysis": Careless use of all sorts of extended boolean formulas,
   which are then *somehow* transformed into some "clausal form", which is
   not considered on its own, but is just treated as some sort of "file
   format". On the contrary, in the OKlibrary full attention is given to
   clauses (as one would with graphs). </li>
  </ul>


  \todo More advanced treatments
  <ul>
   <li> It seems complicated to "break" the barriers between the rounds, but
   we should try. </li>
   <li> The point is that even if we have a very good representation of a
   round, the sixteen rounds together likely still have enough confusion
   power to make the problem very hard for a SAT solver. </li>
   <li> Definitely xor-constraint can be merged. </li>
  </ul>

*/
