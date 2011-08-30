// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
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


  \todo Experiment scripts
  <ul>
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
   in ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
   <li> See also "Update experiment script" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp.
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
