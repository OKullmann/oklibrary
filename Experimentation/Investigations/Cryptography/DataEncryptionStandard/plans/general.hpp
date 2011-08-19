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
   Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp
   for investigations into the DES Sboxes. </li>
  </ul>


  \todo DES benchmarks
  <ul>
   <li> There exist the following translations of DES into SAT:
    <ul>
     <li> Massaci and Marraro's translation from [Logical Cryptanalysis of the
     Data Encryption Standard; Marraro and Massacci]. Available at
     http://disi.unitn.it/~massacci/CryptoSAT/. </li>
     <li> ArgoSAT DES benchmarks, see SAT09/APPLICATIONS/crypto/desgen/ in
     http://www.cril.univ-artois.fr/SAT09/bench/appli.7z. </li>
     <li> From [Algrebraic Attacks using SAT-solvers; Jovanovic and Kreuzer]
     using the computer algebra system ApCoCoA. No website available. </li>
    </ul>
   </li>
   <li> We should e-mail Martin Kreuzer to ask for the generator/benchmarks.
   </li>
   <li> We should install these all in the OKlibrary; see
   "DES generators", "ArgoSAT DES instances" and "Kreuzer DES instances" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Cryptography.hpp. </li>
   <li> See also "Understanding the Massacci-Marraro translation". </li>
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
   <li> DONE (No, both follow by UCP; see "Encryption and decryption")
   Do we need also to include decryption? Can the basic scheme encrypt
   and decrypt by just unit-clause propagation? </li>
  </ul>


  \todo Encryption and decryption
  <ul>
   <li> We consider encryption and decryption instances of the 16 round
   DES. </li>
   <li> These instances are the full 16 round DES translation. </li>
   <li> Unit-clauses are added for the plaintext and key, or ciphertext and
   key. This is for encryption and decryption respectively. </li>
   <li> See "Basic translation" for statistics for these instances. </li>
   <li> The task of the solver is then to derive the ciphertext or plaintext
   variables for encryption and decryption respectively. </li>
   <li> Using the canonical translation for the Sboxes:
   <ul>
    <li> Encryption and decryption of DES follows by unit-clause propagation.
    </li>
    <li> Encryption:
    <ul>
     <li> Generating an encryption instance for 16 rounds using ArgoSAT test
     vectors (see
     Cryptography/DataEncryptionStandard/plans/KeyDiscovery/general.hpp):
     \verbatim
sbox_fcl_l : create_list(dualtsplus_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
F : des2fcl(sbox_fcl_l)$
P : des_plain2fcl(hexstr2binv("038E596D4841D03B"))$
K : des_key2fcl(hexstr2binv("15FBC08D31B0D521"))$
F_std : standardise_fcs([F[1],append(F[2],P[2],K[2])])$
output_fcs_v(sconcat("DES ArgoSat comparison over 16 rounds with the first ", unknown_bits, " key bits undefined."), F_std[1] , sconcat("des_argocomp_encrypt.cnf"), F_std[2]);
     \endverbatim
     </li>
     <li> precosat236 and OKsolver_2002 need 0 decisions to solve this
     instance:
     \verbatim
shell> precosat236 des_argocomp_encrypt.cnf
*snip*
c 0 conflicts, 0 decisions, 0 random
*snip*
shell> OKsolver_2002-O3-DNDEBUG des_argocomp_encrypt.cnf
*snip*
c number_of_nodes                       0
*snip*
     \endverbatim
     </li>
    </ul>
    </li>
    <li> Decryption:
    <ul>
     <li> Generating a decryption instance for 16 rounds and ArgoSAT test
     vectors (see
     Cryptography/DataEncryptionStandard/plans/KeyDiscovery/general.hpp):
     \verbatim
sbox_fcl_l : create_list(dualtsplus_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
F : des2fcl(sbox_fcl_l)$
C : des_cipher2fcl(hexstr2binv("A2FB6032638EC79D"))$
K : des_key2fcl(hexstr2binv("15FBC08D31B0D521"))$
F_std : standardise_fcs([F[1],append(F[2],C[2],K[2])])$
output_fcs_v(sconcat("DES ArgoSat comparison over 16 rounds with the first ", unknown_bits, " key bits undefined."), F_std[1] , sconcat("des_argocomp_decrypt.cnf"), F_std[2]);
     \endverbatim
     </li>
     <li> precosat236 and OKsolver_2002 need 0 decisions to solve this
     instance:
     \verbatim
shell> precosat236 des_argocomp_decrypt.cnf
*snip*
c 0 conflicts, 0 decisions, 0 random
*snip*
shell> OKsolver_2002-O3-DNDEBUG des_argocomp_decrypt.cnf
*snip*
c number_of_nodes                       0
*snip*
     \endverbatim
     </li>
    </ul>
    </li>
   </ul>
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
   <li> Do we have an r_1-base of the operation which applies all Sboxes
   to the 48-bit input?
   <ul>
    <li> This is complicated by the fact that the Sboxes share input
    bits due to repetition. </li>
   </ul>
   </li>
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
