// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/128/4_4_8/0_23_13.hpp
  \brief Investigations into AES key discovery for 1 round AES without MixColumns (0+2/3+1/3)


  \todo Show and explain sizes of minimum-translations
  <ul>
   <li> To show the "minimum-size" translation, its sizes need to be discussed
   everywhere. </li>
   <li> This must include explanations of the numbers of different boxes and
   their contributions to the overall size (explained, in each case). </li>
  </ul>


  \todo Problem specification
  <ul>
   <li> We investigate the 0 + 2/3 + 1/3 round AES with 4 row,
   4 column, using the 8-bit field size. </li>
   <li> We denote this AES instance by aes(0+2/3,4,4,8). </li>
   <li> aes(0+2/3,4,4,8) takes a 128-bit plaintext and 128-bit key and
   outputs a 128-bit ciphertext. </li>
   <li> The MixColumns is the identity in this instance . </li>
   <li> The final round - the only round - is "special"; we only
   perform 2/3 of the round (key addition + SubBytes). </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/128/4_4_8/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
   <li> This instance has:
    <ul>
     <li> 1 special round (Key Addition, SubBytes, and ShiftRows).
     </li>
     <li> 16 Sboxes in the SubBytes operation (4 rows * 4 columns = 16). </li>
     <li> 256 additions within the round and key additions, coming from:
      <ul>
       <li> 256 additions from key additions
       (2 round keys * 128-bit additions = 256). </li>
      </ul>
     </li>
     <li> 4 Sboxes in the AES key schedule (4 rows). </li>
     <li> 128 additions in the key schedule:
     <ul>
      <li> 8 additions of arity three
      (1 row * 1 column * 8 bits = 8). </li>
      <li> 120 additions of arity two
      ((3 rows * 4 columns + 1 rows * 3 columns) * 8 bits = 120). </li>
     </ul>
     </li>
     <li> 8 bits for the constant in the key schedule. </li>
    </ul>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(0+2/3,4,4,8):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> The S-box is considered as a 16x1-bit boolean function;
     translated using the canonical translation; see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions (XORs) of arity k are considered bit-wise as (k+1)-bit
     boolean functions; translated using their 2^k prime implicates. </li>
    </ul>
   </li>
   <li> Generating instantiated key-discovery instances for key seeds 1 to 20:
    <ul>
     <li> Generating uninstantiated AES-instance for 0 + 2/3 + 1/3 round:
     \verbatim
num_rounds : 1;
num_rows : 4;
num_columns : 4;
exp : 8;
final_round_b : true;
box_tran : aes_ts_box;
mc_tran : aes_mc_bidirectional;
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran);
for seed : 1 thru 20 do
  output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);

shell> cat ssaes_r1_c4_rw4_e8_f1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
5928 88636 260776 0 260776 5929 1
 length count
1 8
2 81920
3 1504
4 64
17 5120
256 20

shell> for seed in $(seq 1 20); do AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e8_f1.cnf ssaes_pcpair_r1_c4_rw4_e8_f1_s${seed}.cnf > r1_k${seed}.cnf; done
     \endverbatim
     </li>
     <li> The number of clauses of each length in the uninstantiated
     translation before UCP (ssaes_r1_c4_rw4_e8_f1.cnf):
     \verbatim
maxima> ncl_list_ss(1,4,4,8,true,aes_ts_box,aes_mc_bidirectional);
[[1,8],[2,81920],[3,1504],[4,64],[17,5120],[256,20]]
maxima> ncl_list_ss_gen(1,4,4,8,ss_mixcolumns_matrix(2,8,4),[[2,'s2],[9,'s9],[16,'s16]],[],true,aes_mc_bidirectional);
[[1,8],[2,20*s2],[3,1504],[4,64],[9,20*s9],[16,20*s16]]
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
     \endverbatim
     comprised of:
      <ul>
       <li> 8 unit-clauses for the 8-bit constant in the key expansion. </li>
       <li> 81920 binary clauses, coming from 20 Sboxes
       (20 * 4096 = 81920). </li>
       <li> 1504 ternary clauses, coming from 376 additions of arity two
       (376 * 4 = 1504). </li>
       <li> 64 clauses of length four, coming from 8 additions of arity three
       (8 * 8 = 64). </li>
       <li> 5120 clauses of length seventeen, coming from 20 Sboxes
       (20 * 256 = 5120). </li>
       <li> 20 clauses of length 256, coming from from 20 Sboxes
       (20 * 1 = 20). </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Solving for key seed = 1:
    <ul>
     <li> Most solvers solve the problem in < 6m. </li>
     <li> Using the OKsolver:
     \verbatim
shell> OKsolver_2002-O3-DNDEBUG -D15 -M r1_k1.cnf
c running_time(sec)                     362.9
c number_of_nodes                       646
c number_of_single_nodes                1
c number_of_2-reductions                120548
     \endverbatim
     </li>
     <li> Using precosat236:
     \verbatim
shell> precosat236 r1_k1.cnf
     \endverbatim
     returns the correct result in 5.2 seconds.
     </li>
     <li> Glucose finishes in 131.55 seconds:
     \verbatim
shell> precosat236 r1_k1.cnf
c restarts              : 721
c conflicts             : 402591         (3060 /sec)
c decisions             : 1562100        (1.58 % random) (11875 /sec)
c propagations          : 57405015       (436374 /sec)
c CPU time              : 131.55 s
     \endverbatim
     </li>
     <li> MiniSAT-2.2.0 finishes in 60s:
     \verbatim
shell> minisat-2.2.0 test_keyfind.cnf
<snip>
restarts              : 382
conflicts             : 143515         (2413 /sec)
decisions             : 801345         (0.00 % random) (13473 /sec)
propagations          : 25615653       (430660 /sec)
CPU time              : 59.48 s

shell> minisat2 r1_k1.cnf
restarts              : 13
conflicts             : 33963          (346 /sec)
decisions             : 214258         (1.39 % random) (2181 /sec)
propagations          : 5270395        (53637 /sec)
CPU time              : 98.26 s
    \endverbatim
    </li>
    <li> march_pl solves it in 19.92s:
    \verbatim
shell> march_pl r1_k1.cnf
c main():: nodeCount: 5
c main():: dead ends in main: 0
c main():: lookAheadCount: 133012
c main():: unitResolveCount: 5664
c main():: time=19.920000
     \endverbatim
     It apparently does this in only 5 nodes??!?
     </li>
     <li> Picosat solves it in 18 seconds:
     \verbatim
shell> picosat913 r1_k1.cnf
c 29 iterations
c 439 restarts
c 94325 conflicts
c 783798 decisions
c 35170202 propagations
c 17.7 seconds total run time
     \endverbatim
     </li>
     <li> survey_propagation doesn't converge:
     \verbatim
shell> survey_propagation r1_k1.cnf
<snip>
fixed 1 biased var (+49 ucp)
.:-)
<bias>:-nan
fixed 1 biased var (+17 ucp)
.:-)
<bias>:-nan
contradiction
     \endverbatim
     </li>
     <li> Grasp says "RESOURCES EXCEEDED":
     \verbatim
shell> sat-grasp r1_k1.cnf
Registering switches
	Done creating structures. Elapsed time: 0.12
	Done searching.... RESOURCES EXCEEDED. Elapsed time: 2256.6
     \endverbatim
     </li>
     <li> With "seed : 2$" and "seed : 3$", these runtimes and statistics
     seem stable. </li>
    </ul>
   </li>
   <li> DONE (merge_cnf.sh replaced with AppendDimacs)
   Finally we merge the assignment with the basic instance:
   \verbatim
shell> $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh ssaes_r1_c4_rw4_e8_f1.cnf ssaes_pcpair_r1_c4_rw4_e8_f1_s1.cnf > r1_k1.cnf

> cat r1_k1.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
terminate called after throwing an instance of 'OKlib::InputOutput::ClauseInputError'
  what():  OKlib::InputOutput::StandardDIMACSInput::read_clauses:
  literal 5929 has variable index larger than the specified upper bound 5928
line 94566, column 2, total characters read 21231687
   \endverbatim
   </li>
  </ul>

*/
