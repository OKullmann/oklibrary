// Matthew Gwynne, 13.2.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/KeeLoq/plans/general.hpp
  \brief On investigations into KeeLoq cipher


  \todo Overview
  <ul>
   <li> KeeLoq is a 528-round iterated shift-register cipher with
   32-bit plaintext, and ciphertext, and 64-bit keys. </li>
   <li> The KeeLoq encryption cipher consists of the following:
   </li>
   <li> A key point here is that the KeeLoq round function is
   the combination of a small XXX and the XOR of X more bits,
   meaning that the entire round function is a XX bit boolean
   function. </li>
   <li> Algebraic attacks using SAT solvers are discussed in
   [Algebraic and Slide Attacks on KeeLoq; Bard, Courtois, Wagner 2008],
   theoretically breaking the cipher in time equivalent to 2^53
   encryptions, compared to 2^64 for brute-force. </li>
  </ul>


  \todo Analysing the KeeLoq round function
  <ul>
   <li> The KeeLoq round function as a boolean function:
   \verbatim
/* The 5-bit (input) non-linear feedback function */
keeloq_nlf(a,b,c,d,e) := mod(d + e + a * c + a * e + b * c + b * e + c *d + d * e + a * d * e + a * c * e + a*b*d + a * b * c, 2)$

/* The 9x1-bit boolean function for the KeeLoq round: */
keeloq_round_bf(V) := [mod(V[1] + V[2] + V[3] + keeloq_nlf(V[4],V[5],V[6],V[7],V[8]) + V[9] + 1,2)]$
   \endverbatim
   </li>
   <li> Computing statistics:
   \verbatim
maxima> Round_DNF : bf2fulldnf_fcl(keeloq_round_bf,9)$
maxima> statistics_fcs(Round_DNF);
 [9,256,2304,9,9]

maxima> Round_CNF : bf2fullcnf_fcl(keeloq_round_bf,9)$
maxima> statistics_fcs(Round_CNF);
 [9,256,2304,9,9]

maxima> output_fcs("KeeLoq Round Function", Round_CNF, "KeeLoq_Round_full.cnf")$

shell> QuineMcCluskeySubsumptionHypergraphWithFullStatistics-n16-O3-DNDEBUG KeeLoq_Round_full.cnf > KeeLoq_Round_full.cnf_shg
shell> cat KeeLoq_Round_full.cnf_primes_stats
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      9     224      9      9     224     1760     NA     NA     224     1760     0
 length   count
      7      32
      8     192
shell> cat KeeLoq_Round_full.cnf_shg_stats
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    224     256    224    224     256      512     NA     NA     256      512     0
 length   count
      2     256

shell> ${OKplatform}/OKlib/Satisfiability/Optimisation/minimise_cnf_cryptominisat KeeLoq_Round_full.cnf > KeeLoq_Round_min.cnf
shell> cat KeeLoq_Round_min.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      9      96      9      9      96      736     NA     NA      96      736     1
 length   count
      7      32
      8      64

maxima> Round_primes : read_fcl_f("KeeLoq_Round_full.cnf_primes")$
maxima> Round_min : read_fcl_f("KeeLoq_Round_min.cnf")$
maxima> hardness_wpi_cs(setify(Round_min[2]), setify(Round_primes[2]));
 1
   \endverbatim
   So the minimum representation has hardness 1! </li>
   <li> Investigating the ANF to CNF encoding for the KeeLoq round used
   in [Algebraic and Slide Attacks on KeeLoq; Bard, Courtois, Wagner 2008]:
   \verbatim
 /* Generating the encoding: */
maxima> Round_anf : lappend(map(lambda([v,C], cons(adjoin(v,map("-",C)),create_list({-v,l},l,listify(C)))),[10,11,12,13,14,15,16,17,18,19,20],[{4,6},{4,8},{5,6},{5,8},{6,7},{7,8},{11,7},{11,6},{20,7},{20,6},{4,5}]))$
maxima> Round_par_anf : append(
              boolm_mul2cnf_wv_fcl(matrix([1,1,1,1]),[1,2,3,7,21])[2],
              boolm_mul2cnf_wv_fcl(matrix([1,1,1,1]),[8,10,11,12,22])[2],
              boolm_mul2cnf_wv_fcl(matrix([1,1,1,1]),[13,14,15,16,23])[2],
              boolm_mul2cnf_wv_fcl(matrix([1,1,1]),[17,18,19,24])[2],
              boolm_mul2cnf_wv_fcl(matrix([1,1,1,1]),[21,22,23,24,9])[2]);
maxima> Round_anf : append(Round_anf,Round_par_anf);
maxima> statistics_cs(Round_anf);
 [24,105,429,5,2]
/* Checking we compute the right function: */
maxima> for C in Round_CNF[2] do assert(ucp_0_cs(apply_pa_cs(comp_sl(C),setify(Round_anf))) = {{}});
 done
maxima> for C in Round_DNF[2] do assert(ucp_0_cs(apply_pa_cs(C,setify(Round_anf))) = {});
 done
 /* Computing a lower-bound on the hardness, the hardness w.r.t the prime
    implicates on the original variables: */
 maxima> hardness_wpi_cs(setify(NLF_nv), setify(Round_primes[2]));
 2
   \endverbatim
   So the ANF representation used has a higher hardness, and is also a larger
   representation. The minimum 1-base above should be a much better representation
   of the KeeLoq round function. </li>
   <li> KeeLoq should be implemented at the Maxima level, and then translated,
   so that we can compare these translations. </li>
  </ul>

*/
