// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/general.hpp
  \brief On investigations into the Data Encryption Standard key discovery


  \todo Overview
  <ul>
   <li> Solving the key discovery problem for:
    <ul>
     <li> 1-round DES, see
     Cryptography/DataEncryptionStandard/plans/KeyDiscovery/1.hpp.
     </li>
     <li> 3-round DES, see
     Cryptography/DataEncryptionStandard/plans/KeyDiscovery/3.hpp.
     </li>
     <li> 4-round DES, see
     Cryptography/DataEncryptionStandard/plans/KeyDiscovery/4.hpp.
     </li>
     <li> 5-round DES, see
     Cryptography/DataEncryptionStandard/plans/KeyDiscovery/5.hpp.
     </li>
     <li> 16-round DES with some known key bits, see
     Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp.
     </li>
    </ul>
   </li>
  </ul>


  \todo Number of solutions for DES key discovery
  <ul>
   <li> When analysing the performance of solvers on the DES
   key discovery instances, it is interesting to consider how
   many satisfying assignments each instance has. </li>
   <li> For instance, roughly speaking, if the instance has multiple
   satisfying assignments, then perhaps the solver is less "guided"
   to one single solution. </li>
   <li> Using cryptominisat to determine the number of solutions:
    <ul>
     <li> Generating the instances:
     \verbatim
shell> oklib --maxima
oklib_load_all()$
for rounds : 1 thru 4 do block(
  sbox_fcl_l : create_list(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8),
  for seed : 1 thru 20 do block(
    print(sconcat("Generating ", rounds, "-round DES with seed ", seed)),
    set_random(make_random_state(seed)),
    P_hex : lpad(int2hex(random(2**64)),"0",16),
    K_hex : lpad(int2hex(random(2**64)),"0",16),
    C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
    P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds),
    C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds),
    F : des2fcl_gen(sbox_fcl_l,rounds),
    Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])]),
    output_fcl_v(
      sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the canonical translation for the S-boxes (6-to-4)."),
      Fs[1],
      sconcat("des_6t4_canon_r",rounds,"_s",seed,".cnf"),
      Fs[2])))$
print("DONE!");
     \endverbatim
     </li>
     <li> Numbers of solutions for each rounds:
     \verbatim
# Use cryptominisat's "maxsolutions" option to compute as many solutions as possible up to 32000
> for instance in *.cnf; do echo ${instance}; cryptominisat --maxsolutions=32000 ${instance} > cryptominisat_multiple_solutions_${instance}_32000max.result 2>&1; done
# Count the number of solutions output
[1 1] 32000
[1 2] 32000
[1 3] 32000
[1 4] 32000
[1 5] 12856
[1 6] 6869
[1 7] 9518
[1 8] 13326
[1 9] 13816
[1 10] 32000
[1 11] 32000
[1 12] 7388
[1 13] 12630
[1 14] 32000
[1 15] 32000
[1 16] 32000
[1 17] 32000
[1 18] 32000
[1 19] 32000
[1 20] 32000
[2 1] 256
[2 2] 512
[2 3] 768
[2 4] 512
[2 5] 1024
[2 6] 3072
[2 7] 1536
[2 8] 512
[2 9] 512
[2 10] 512
[2 11] 512
[2 12] 768
[2 13] 256
[2 14] 2048
[2 15] 768
[2 16] 512
[2 17] 1024
[2 18] 256
[2 19] 512
[2 20] 256
[3 1] 256
[3 2] 512
[3 3] 256
[3 4] 256
[3 5] 256
[3 6] 256
[3 7] 256
[3 8] 256
[3 9] 256
[3 10] 256
[3 11] 256
[3 12] 256
[3 13] 256
[3 14] 256
[3 15] 256
[3 16] 256
[3 17] 256
[3 18] 256
[3 19] 256
[3 20] 256
[4 1] 256
[4 2] 256
[4 3] 256
[4 4] 256
[4 5] 256
[4 6] 256
[4 7] 256
[4 8] 256
[4 9] 256
[4 10] 256
[4 11] 256
[4 12] 256
[4 13] 256
[4 14] 256
[4 15] 256
[4 16] 256
[4 17] 256
[4 18] 256
[4 19] 256
[4 20] 256
     \endverbatim
     </li>
     <li> For the first round, we hit the 32000 limit, and so these instances
     are being rerun. </li>
     <li> The DES includes 8 key bits which are not used by the algorithm
     (every 8-th bit, 8, 16, 24, 32 etc). </li>
     <li> The unused key bits still occur in the translation, and so the
     number of solutions should be divided by 256 to get the number of
     "true" 56-bit keys which map the given plaintext to the given ciphertext.
     </li>
     <li> So it seems for 1 up to 3 rounds, we can see multiple keys
     which map a particle plaintext to a particular ciphertext. </li>
     <li> We should compute what keys these are, and check this is the case.
     </li>
     <li> We should also use include this data in our analysis of the
     performance of solvers on these instances. </li>
     <li> The number of solutions decreases as the number of rounds increases.
     Can we explain precisely why this is? </li>
    </li>
   </li>
  </ul>


  \todo Experiment scripts
  <ul>
   <li> The argumentation below seems dubious to OK:
    <ol>
     <li> It seems to fit into the unfortunate pattern of just writing tools
     for themselves, without interest in the experiments. </li>
     <li> Special scripts for such special purposes will only be
     understandable, will only be used by the person running the scripts. </li>
     <li> Then it follows also that they will be of weak quality. </li>
     <li> We should only use general tools; except, of course, it is a real
     method, like for computing van der Waerden numbers the various scripts,
     which implement certain algorithms. </li>
     <li> No tiny ("dirty") scripts for this and that, as a private hobby of
     a single person --- but everything must connect to the whole library.
     </li>
    </ol>
   </li>
   <li> The presentation of experimental data for DES in
   Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/
   is excessively verbose, and hides the underlying data. </li>
   <li> Experiment generation should be a single command where the key
   parameters can be easily seen, and there is no room for mistake. </li>
   <li> Experimental results should be concise. </li>
   <li> Experimental results should be extractable at any stage of the
   computation, not only once a batch of 20 instances have been run on.
   </li>
   <li> We need scripts for:
    <ul>
     <li> Generating DES data (S-box representations etc). </li>
     <li> Generating DES experiments; </li>
     <li> Running DES experiments; </li>
    </ul>
   </li>
   <li> Generating DES data:
    <ul>
     <li> We need scripts which generate CNF representations for all
     of the DES S-boxes, for each different translation. </li>
     <li> So we need the following scripts:
      <ul>
       <li> GenerateDESSboxes_Min: generates the CNF representations
       for the DES S-boxes with the smallest (conjectured) number of clauses.
       </li>
       <li> GenerateDESSboxes_1base: generates the CNF 1-base
       representations for the DES S-boxes with the smallest (conjectured)
       number of clauses. </li>
      </ul>
     </li>
     <li> Note, in all cases, the minimum sizes are *not* known, and
     so smaller representations might be found. </li>
     <li> These scripts can then be updated manually if new smaller
     representations are found. </li>
     <li> The scripts should create a directories DES_Sboxes_min_${date} and
     DES_Sboxes_1base_${date} respectively, and output the corresponding
     representations for S-box i to "Sbox_${i}.cnf" within the directory.
     </li>
     <li> ${date} is a timestamp: $(date +"%Y-%m-%d-%H%M%S"). </li>
    </ul>
   </li>
   <li> Generating DES experiments:
    <ul>
     <li> We need scripts which generate an uninstantiated DES instance
     using each of the box translations. </li>
     <li> So we need the following scripts (arguments given in parentheses):
      <ul>
       <li> GenerateDESExperiment_MinSbox(r): generates the r-round DES using
       the "minimum" translation for the S-boxes. </li>
       <li> GenerateDESExperiment_1baseSbox(r): generates the r-round DES
       using the 1-base translation for the S-boxes. </li>
       <li> GenerateDESExperiment_CanonSbox(r): generates the r-round DES
       using the canonical translation for the S-boxes. </li>
       <li> GenerateDESKeys(r,s_max): generates CNF Dimacs files
       containing unit-clauses for instantiating r-round DES with the
       plaintext-ciphertext pairs generated using seeds 1,...,s_max. </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> To run the DES experiments, we should extend the standard
   experiment running scripts, discussed in "Running experiments"
   in ExperimentSystem/SolverMonitoring/plans/general.hpp. </ul>
   </li>
   <li> All DES experiments need to be rerun using the new experiment scripts
   discussed in "Running experiments"
   in ExperimentSystem/SolverMonitoring/plans/general.hpp. OK: ???
   Why this waste of time ??? We already suffer from a serious lack of data.
   This again seems to fit into the pattern of running the experiments as end
   in itself. </li>
   <li> See also "Update experiment script" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
  </ul>


  \todo Add information on specific S-box translations to experiment instances
  <ul>
   <li> At present, if one encounters an experiment instance for a DES
   experiment, then it is not clear exactly how it was produced. </li>
   <li> The ambiguity occurs because, over time, smaller "minimum" and 1-base
   representations for the S-boxes are found. </li>
   <li> Based on these new representations, new translations are generated,
   and experiments rerun. </li>
   <li> This leaves one with several copies of the same translation file,
   and the possibility of uncertainty about which is which. </li>
   <li> All instance generation code for experiments should include
   information, via the DIMACS comments of the instance, on the S-box
   representations used. </li>
   <li> For example, the seeds given by RandomRUcpBases to generate the
   1-base for a DES S-box could be output as comment in the DIMACS FILE as:
   \verbatim
c DES S-box 1 1-base seeds: 1 (gen) 2 (base).
   \endverbatim
   </li>
   <li> Even better, tools and scripts should be written which take
   only parameters such as seeds from RandomRUcpBases or ubcsat etc and
   output the CNFs. </li>
   <li> Then the actual generation instructions could be added to the CNF
   file as comments. </li>
  </ul>


  \todo DONE Move into separate sub-module

*/
