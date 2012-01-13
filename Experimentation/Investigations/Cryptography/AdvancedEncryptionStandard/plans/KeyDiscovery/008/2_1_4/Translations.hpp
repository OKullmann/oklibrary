// Matthew Gwynne, 21.12.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/008/2_1_4/Translations.hpp
  \brief Listing the translations for the small-scale AES key discovery for AES with a 2x1 plaintext matrix and 4-bit field elements


  \todo Standard "box" translations
  <ul>
   <li> We consider the default decomposition into small boolean
   functions for aes(r,2,1,4) discussed in "Problem specification"
   AdvancedEncryptionStandard/plans/KeyDiscovery/016/2_1_8/general.hpp.
   </li>
   <li> Splitting aes(r,2,1,4) into boolean functions ("boxes") in the
   "standard"/"default" way:
    <ul>
     <li> The AES is decomposed into Subbytes, MixColumns and
     the Key schedule. </li>
     <li> The Subbytes is decomposed into S-boxes. </li>
     <li> The MixColumns operation is decomposed into its field
     multiplications (02 and 03) and addition operations. </li>
     <li> The Key schedule is decomposed into S-boxes and
     additions using new variables to store intermediate results.
     </li>
     <li> We treat S-boxes, field multiplications and additions as the
     "boxes". </li>
     <li> The "boxes" are then translated using (see below):
      <ul>
       <li> "The canonical box translation". </li>
       <li> "The 1-base box translation". </li>
       <li> 'The "minimum" box translation". </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> The MixColumns operation is translated by translating both
   the MixColumns operation and its inverse (it is self-inverse). </li>
   <li> Size of boolean functions:
    <ul>
     <li> The S-box and field multiplications are considered as a 8x1
     boolean functions. </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
    </ul>
   </li>
   <li> In any standard "box" translation, for r rounds, we have:
    <ul>
     <li> r full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 4*r Sboxes:
      <ul>
       <li> 2*r from SubBytes = 2 byte * r rounds; </li>
       <li> 2*r from key schedule = 2 row * 1 byte * r rounds. </li>
      </ul>
     </li>
     <li> 4*r multiplications by 02: 2 rows * 1 multiplication * 1 columns *
     r rounds * 2 directions (forward + inverse). </li>
     <li> 4*r multiplications by 03: 2 rows * 1 multiplication * 1 columns *
     r rounds * 2 directions (forward + inverse). </li>
     <li> 32*r + 8 additions of arity 2:
      <ul>
       <li> 8*r from key additions = 8 bits * r round; </li>
       <li> 8 from final key addition = 8 bits; </li>
       <li> 4*r from the key schedule = 1 rows * 4 bits * r round. </li>
       <li> 8*r from forward MixColumns = 2 rows * 1 column * 4 bits *
       r rounds; </li>
       <li> 8*r from inverse MixColumns = 2 rows * 1 column * 4 bits * r
       rounds. </li>
      </ul>
     </li>
     <li> 4*r bits for the constant in the key schedule = 4 bits * r rounds.
     </li>
    </ul>
   </li>
   <li> Note that as this variant has only one column, the key schedule
   applies Sbox(K_i) + C rather than Sbox(K_i) + K_j + C where K_i and
   K_j are key words from the previous round key. </li>
  </ul>


  \todo The canonical box translation
  <ul>
   <li> Using the 'Standard "box" translation' (see above). </li>
   <li> The Sboxes and multiplications boxes are translated using the
   canonical translation, which has the following number of clauses of
   each length:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instance has 6 * r boxes = 2*r S-boxes + 4*r multiplications. </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 8*r = key schedule constant * 1; </li>
     <li> 2 : 1536*r = 12*r boxes * 128; </li>
     <li> 3 : 128*r + 32 = 32*r+8 additions (arity 2) * 4; </li>
     <li> 9 : 96*r = 6*r boxes * 16; </li>
     <li> 16 : 12*r = 6*r boxes * 1. </li>
    </ul>
   </li>
  </ul>


  \todo The 1-base box translation
  <ul>
   <li> The Sboxes and multiplications use 1-base translations,
   which have the following number of clauses of each length:
   \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,4));
[[3,12],[4,15]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,2]));
[[2,6],[3,4]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,3]));
[[3,16],[4,8]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 8*r = key schedule constant * 1; </li>
     <li> 2 : 4*r = 4*r multiplications by 02 * 10; </li>
     <li> 3 : 368*r + 64 = 4*r multiplications by 02 * 12 + 4*r
     multiplications by 03 * 24 + 56*r+16 additions (arity 2) * 4; </li>
     <li> 4 : 96*r = 4*r multiplications by 03 * 24; </li>
     <li> 5 : 132 = 4*r S-boxes * 1 + 4 multiplications by 03 * 32; </li>
     <li> 6 : 4748*r = 4*r S-boxes * 1187; </li>
     <li> 7 : 10812*r = 4*r S-boxes * 2703; </li>
     <li> 8 : 2012*r = 4*r S-boxes * 503; </li>
     <li> 9 : 16*r = 4*r S-boxes * 4. </li>
    </ul>
   </li>
  </ul>


  \todo The "minimum" box translation:
  <ul>
   <li> There are currently active investigations attempting to find the
   minimum representations for each of the boxes, discussed in
    <ul>
     <li> AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp.
     </li>
     <li> AdvancedEncryptionStandard/plans/Representations/Mul_2_8.hpp.
     </li>
     <li> AdvancedEncryptionStandard/plans/Representations/Mul_3_8.hpp.
     </li>
    </ul>
   </li>
   <li> The smallest "minimum" representations for the S-box (as of 05/09/2011)
   and multiplications by 02 and 03 from
   AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp,
   AdvancedEncryptionStandard/plans/Representations/Mul_2_8.hpp
   and
   AdvancedEncryptionStandard/plans/Representations/Mul_3_8.hpp :
   \verbatim
/* Multiplication by 02: */
maxima> FieldMul2CNF : [{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},{{-9,2},{-2,9},{-10,3},{-3,10},{-11,4},{-4,11},{-12,-5,-1},{-12,1,5},{-5,1,12},{-1,5,12},{-13,-6,-1},{-1,6,13},{-14,7},{-7,14},{-15,1,8},{-8,1,15},{-16,-15,-8},{-16,8,15},{-13,6,16},{-6,13,16}}]$
set_hm(ss_field_cnfs,[8,2], FieldMul2CNF));
/* Multiplication by 03: */
maxima> FieldMul3CNF :
 [[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16],
  [{-9,-2,-1},{-2,1,9},{-10,2,3},{-10,-9,-3,1},{-10,-3,-1,9},{-3,2,10},{-9,1,3,10},{-1,3,9,10},{-11,-4,-3},{-11,3,4},{-4,3,11},{-3,4,11},{-12,-5,-4,1},{-12,-4,-1,5},{-5,1,4,12},{-1,4,5,12},{-13,-5,-1,6},{-13,1,5,6},{-13,-12,-6,4},{-13,-6,-4,12},{-6,-5,-1,13},{-6,1,5,13},
   {-12,4,6,13},{-4,6,12,13},{-14,-7,-6},{-14,6,7},{-7,6,14},{-6,7,14},{-16,-8,-1},{-16,1,8},{-16,-15,-7},{-16,7,15},{-8,1,16},{-1,8,16},{-15,7,16},{-7,15,16}]]$
set_hm(ss_field_cnfs,[8,2], FieldMul3CNF));
/* Sbox: */
maxima> output_rijnsbox_fullcnf_stdname();
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_shg.cnf
shell> cat AES_Sbox_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_Sbox_shg.wcnf
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 40000000 -wtarget 294 -solve 1 -seed 3213901809 -i AES_Sbox_shg.wcnf -r model AES_Sbox_s294.ass;
shell> cat  AES_Sbox_full.cnf_primes | FilterDimacs AES_Sbox_s294.ass > AES_Sbox_s294.cnf
maxima> SboxMinCNF : read_fcl_f("AES_Sbox_s294.cnf");
maxima> set_hm(ss_sbox_cnfs,8, SboxMinCNF));
   \endverbatim
   </li>
   <li> The Sboxes and multiplications use the "minimum" translations,
   which have the following number of clauses of each length:
   \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_cnfs,8));
[[6,143],[7,127],[8,24]]
maxima> ncl_list_fcs(ev_hm(ss_field_cnfs,[8,2]))
[[2,8],[3,12]]
maxima> ncl_list_fcs(ev_hm(ss_field_cnfs,[8,3]))
[[3,20],[4,16]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 8*r = key schedule constant * 1; </li>
     <li> 2 : 32*r = 4*r multiplications by 02 * 8; </li>
     <li> 3 : 352*r + 64 = 4*r multiplications by 02 * 12 + 4*r
     multiplications by 03 * 20 + 56*r+16 additions (arity 2) * 4; </li>
     <li> 4 : 64*r = 4*r multiplications by 04 * 16; </li>
     <li> 6 : 572*r = 4*r S-boxes * 143; </li>
     <li> 7 : 508*r = 4*r S-boxes * 127; </li>
     <li> 8 : 96*r = 4*r S-boxes * 24. </li>
    </ul>
   </li>
  </ul>

*/
