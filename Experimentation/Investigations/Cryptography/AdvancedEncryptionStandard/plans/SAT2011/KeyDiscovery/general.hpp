// Matthew Gwynne, 23.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/general.hpp
  \brief Investigations into AES key discovery

  The aim of these experiments is, using translations of the AES and small 
  scale AES cryptosystems with the translation scheme given in 
  ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac, to 
  "break" AES by discovering the key given a plaintext and ciphertext pair.

  The key aspects of the investigation are
   - the representations for the various "boxes" of the AES (see 
  ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac)
  - the reconfiguration of the computational network, to make the boolean
  analysis more powerful (see
  ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac).
  

  \todo Links
  <ul>
   <li> For investigations into the various component/box representations 
   (CNF and otherwise) of the AES boxes (Sbox, field multiplications etc) see
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/general.hpp .
   </li>
   <li> See 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/general.hpp
   for the more general investigations.
   </li>   
  </ul>


  \todo Explanations
  <ul>
   <li> DONE For every such file, we need explanations what a "round" etc. is,
   in elementary terms, using terms "addition, sub-bytes, shift-rows,
   mix-columns, round-key". </li>
   <li> In general, all discussions should be based on the general notions
   as introduced in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/IteratedBlockCipher.mac.
   </li>
   <li> Also the file-name needs to be explained (in each file). </li>
  </ul>


  \todo Fast generation of AES translations
  <ul>
   <li> At current, the AES translations are incredibly slow, mainly due to 
   the fact that very large variable names are introduced, which are then 
   later standardised. </li>
   <li> The maxima implementation of hash tables is slow, and so as discussed
   in "Using the Maxima "associative arrays" in 
   ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp, one can get quite a
   large speed up in some circumstances by use associative arrays. </li>
   <li> The following functions can be redefined after calling
   "oklib_load_all" to achieve a speed up:
   \verbatim
rename_fcl(FF,VL) := block([count : -1], local(h),
  for V in map("[", FF[1], VL) do h[V[1]] : V[2],
  return([create_list(i,i,1,length(FF[1])),
   map(
     lambda([C], (  
       if oklib_monitor then 
           (count : count + 1, 
            if mod(count,1000) = 0 then 
              print("Renaming ", count, "/",length(FF[2]))),
       map(lambda([L], if L > 0 then h[L] else -h[-L]), C))), FF[2])]))$

gen_count : 384;
generate_ss_constraint_vars(n,m,namespace, id) := 
  create_list(
          if integerp(gen_h[namespace(ss_var(i,id))]) then gen_h[namespace(ss_var(i,id))]
          else gen_h[namespace(ss_var(i,id))] : (gen_count : gen_count +1),
          i,n,m)$
   \endverbatim
   </li>
   <li> Note that although the standardised translation will be the same,
   the file output will be slightly different as the variable mappings will
   not be maintained. Therefore, if knowing precisely what variables are what
   (excluding the plaintext, key and ciphertext variables, which are always
   at 1-3*num_rows*num_columns*exp) is important, then the standard
   translation, without these faster functions, should be used. </li>
   <li> Also note that "gen_h" is global in this example as we want
   generate_ss_constraint_vars to yield the same result across calls. </li>
   <li> Therefore, a typical AES translation might be:
   \verbatim
maxima> oklib_load_all()$
maxima> oklib_monitor : true$
maxima> rename_fcl(FF,VL) := block([count : -1], local(h),
  for V in map("[", FF[1], VL) do h[V[1]] : V[2],
  return([create_list(i,i,1,length(FF[1])),
   map(
     lambda([C], (  
       if oklib_monitor then 
           (count : count + 1, 
            if mod(count,1000) = 0 then 
              print("Renaming ", count, "/",length(FF[2]))),
       map(lambda([L], if L > 0 then h[L] else -h[-L]), C))), FF[2])]))$

gen_count : 384;
generate_ss_constraint_vars(n,m,namespace, id) := 
  create_list(
          if integerp(gen_h[namespace(ss_var(i,id))]) then gen_h[namespace(ss_var(i,id))]
          else gen_h[namespace(ss_var(i,id))] : (gen_count : gen_count +1),
          i,n,m)$
maxima> output_ss_fcl_std(1,4,4,8,0,aes_ts_box, aes_mc_bidirectional);
   \endverbatim
   </li>
  </ul>


  \todo Add todos

*/
