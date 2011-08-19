// Oliver Kullmann, 19.8.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Cryptography.hpp
  \brief Plans regarding building and making available of Cryptography software and benchmarks


  \todo ArgoSAT DES instances
  <ul>
   <li> The ArgoSAT DES benchmarks are available in
   SAT09/APPLICATIONS/crypto/desgen/ in
   http://www.cril.univ-artois.fr/SAT09/bench/appli.7z .
   </li>
   <li> We should install these in the library. </li>
   <li> We should also contact the Argo group and ask
   for the generators. </li>
  </ul>


  \todo Kreuzer DES instances
  <ul>
   <li> In [Algrebraic Attacks using SAT-solvers; Jovanovic and Kreuzer] the
   authors mention a translation to DES using the ApCoCoA system. </li>
   <li> We should contact the authors and ask if this translation is available.
   </li>
  </ul>


  \todo DES generator
  <ul>
   <li> http://disi.unitn.it/~massacci/CryptoSAT/ contains the generator
   and an update. </li>
   <li> The sources are in sources/SAT/Des. </li>
   <li> We need this for comparison (and for historical reasons). </li>
   <li> Installing the DES generator:
   \verbatim
ExternalSources/builds/SAT/> mkdir Des && cd Des
ExternalSources/builds/SAT/Des/> tar zxvf ../../sources/SAT/Des/des2fml-0.9.tgz
ExternalSources/builds/SAT/Des/> cd des2fml-0.9
ExternalSources/builds/SAT/Des/des2fml-0.9/> zcat ../../../sources/SAT/Des/des2fml-0.9-newgcc.diff.gz | patch -p1
ExternalSources/builds/SAT/Des/des2fml-0.9> make
   \endverbatim
   </li>
   <li> The plaintxt, key are available in the files plaintxt and key_des.
   </li>
   <li> By default many plaintexts are provided. All but one must be removed
   if one wants to generate an instance with a single plaintext-ciphertext
   pair. </li>
   <li> Run
   \verbatim
ExternalSources/builds/Des/des2fml-0.9> rounds=1
ExternalSources/builds/Des/des2fml-0.9> ./des -r${rounds}
   \endverbatim
   to generate the ciphertext in ciph_text from plaintxt and key_des for
   ${rounds} rounds.
   </li>
   <li> Then run
   \verbatim
ExternalSources/builds/Des/des2fml-0.9> ./des2fml -r${rounds} -p -c -f1
   \endverbatim
   to generate the file formulae. This contains a CNF but in Dimacs format.
   </li>
   <li> Converting formulae to Dimacs:
   \verbatim
./clausify formulae test.cnf
   \endverbatim
   </li>
   <li> So for example with
   \verbatim
0xe9 0xa6 0x52 0x90 0x90 0x62 0x60 0xf7
   \endverbatim
   in plaintext, and
   \verbatim
0x1b 0x6c 0x26 0xa7 0x20 0x03 0x3d 0xa2
   \endverbatim
   in key_des, we get the following:
   \verbatim
ExternalSources/builds/Des/des2fml-0.9> round=3 && ./des -r${round} && ./des2fml -r${round} -p -c -f1 && ./clausify formulae test.cnf
############# DES with 3 round/rounds #############

Key: 1b6c26a720033da2
Plaintext: e9a65290906260f7
Ciphertext (after 3 round/rounds): 72fe6cf9cfcebb57

####### Block number 1 #######
Plaintext: e9a65290906260f7
Ciphertext (after 3 round/rounds): 72fe6cf9cfcebb57

ExternalSources/builds/Des/des2fml-0.9> OKsolver-O3-DNDEBUG test.cnf
s SATISFIABLE
c sat_status                            1
c initial_maximal_clause_length         24
c initial_number_of_variables           1531
c initial_number_of_clauses             9286
c initial_number_of_literal_occurrences 25522
c number_of_initial_unit-eliminations   0
c number_of_2-clauses_after_reduction   7421
c running_time(sec)                     8.1
c number_of_nodes                       266
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                25797
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        10
c number_of_table_enlargements          0
c number_of_1-autarkies                 4
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             test.cnf
   \endverbatim
   </li>
  </ul>

*/

