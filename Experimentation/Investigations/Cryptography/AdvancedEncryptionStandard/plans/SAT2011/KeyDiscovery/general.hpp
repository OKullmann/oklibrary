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
   - the representations for the various "boxes" of the AES
  - the reconfiguration of the computational network, to make the boolean
    analysis more powerful.

  The investigations are arranged into subdirectories based on the bit
  size of the key (4,16,32,64,128 and so on), and then by the number of
  columns, rows and field size (e.g. 4_4_8 for standard AES). At the
  lowest level in this directory structure, we then have files for
  experiments on each number of rounds, where we classify rounds as
  follows:
     
  <ul>
   <li> A number n of rounds with no further qualification means that
   the AES consists of n AES rounds (key addition, then SubBytes, then 
   Shiftrows, then MixColumns) with no final key addition. </li>
   <li> A number n of rounds with the addition of 1/3 (e.g. n + 1/3) means 
   that the AES consists of n AES rounds (key addition, then SubBytes, then 
   Shiftrows, then MixColumns) with a final key addition (the first third of 
   the round, ignoring the Shiftrows). </li>
   <li> A number n of rounds with the addition of 2/3 (e.g. n + 2/3) means
   that the AES consists of n AES rounds (key addition, then SubBytes, then 
   Shiftrows, then MixColumns) and then another special final round with just
   key addition, SubBytes and ShiftRows (i.e. two thirds of a round, ignoring 
   Shiftrows). </li>
   <li> A number n of rounds with the addition of 2/3 and 1/3 (n + 2/3 + 1/3) 
   means that the AES consists of n AES rounds (key addition, then SubBytes,
   then Shiftrows, then MixColumns) and then another special final round with 
   just key addition, SubBytes and ShiftRows (i.e. two thirds of a round, 
   ignoring Shiftrows). After this final round, we also have an additional key
   addition (the first third of the round, ignoring the Shiftrows). </li>
  </ul>

  Experiments run over n, n + 1/3, n + 2/3 and n + 2/3 + 1/3 AES rounds are 
  then in files named n.hpp, n_13.hpp, n_23.hpp and n_23_13.hpp respectively.

  As an example, experiments on the standard one round AES (no final round,
  with a final key addition), are in 128/4_4_8/1_13.hpp.


  \todo Add milestones
  <ul>
   <li> We urgently need milestones at this level. </li>
  </ul>


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


  \todo Boundaries
  <ul>
   <li> We need to have a good understanding of the boundaries of the
   parameters and sizes of AES key discovery instances that we can 
   solve in a reasonable time (in less than a day or two). </li>
   <li> So far, we can solve the following in the listed times for
   the n + 1/3 round variants:
   <ul>
    <li> 128 bit key:
     <ul>
      <li> Can break standard AES for 2/3 + 1/3 rounds in 18s with picosat913.
      </li>
      <li> So far can't break standard AES for 1 + 1/3 round after two days 
      computation with all solvers. </li>
      <li> 16 column, 1 row, 8 bits, up to <em> 2 + 1/3 </em> rounds in
      <em>40s</em> with minisat-2.2.0 in 128/16_1_8/2_13.hpp. </li>
     </ul>
    </li>
    <li> 64 bit key:
     <ul>
      <li> 16 column, 1 row, 4 bits, up to <em>5 + 1/3</em> rounds in
      <em> 26 </em> seconds with minisat-2.2.0 in 064/16_1_4/4_13.hpp. </li>
      <li> 4 column, 4 row, 4 bits up to <em>1+1/3</em> rounds in 
      <em> 21 </em> seconds with minisat-2.2.0 in 064/4_4_4/1_13.hpp.
      </li>
     </ul>
    </li>
    <li> 32 bit key:
     <ul>
      <li> 2 column, 4 row, 4 bits up to <em>2 + 1/3</em> rounds in 
      <em> 2718 </em> seconds with glucose in 032/2_4_4/2_13.hpp. </li>
      <li> 4 column, 2 row, 4 bits up to <em>5 + 1/3</em> rounds in 
      <em> 903 </em> seconds with picosat in 032/4_2_4/5_13.hpp. </li>
     </ul>
    </li>
    <li> 16 bit key:
     <ul>
      <li> 2 column, 2 row, 4 bits up to <em>20 + 1/3</em> rounds in 
      <em> 40s </em> seconds with minisat-2.2.0 in 016/2_2_4/20_13.hpp. </li>
     </ul>
    </li>
    <li> 8 bit key:
     <ul>
      <li> 1 column, 1 row, 8 bits up to <em>20 + 1/3</em> rounds in 
      <em> 0.5 </em> seconds with precosat236 in 008/1_1_8/20_13.hpp. </li>
     </ul>
    </li>
    <li> 4 bit key:
     <ul>
      <li> 1 column, 1 row, 4 bits up to <em>20 + 1/3</em> rounds in 
      <em> 0.0 </em> seconds with OKsolver in 004/1_1_4/20_13.hpp. </li>
     </ul>
    </li>
   </ul>
   </li>
  </ul>


  \todo Explanations
  <ul>
   <li> For every such file, we need explanations what a "round" etc. is,
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
   at 1-3*num_rows*num_columns*exp) is important, then one should exclude
   the replacement of "generate_ss_constraint_vars", i.e., only use the
   following:
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
   \endverbatim. 
   </li>
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


  \todo DONE Better organisation
  <ul>
   <li> The number of rounds is clearly sub-ordinated, and thus we should have
   directories, where within we have files just for the various numbers of
   rounds considered. </li>
   <li> The directory names are then the names of the various investigations-
   files (with the round-component of the name removed). </li>
   <li> There should be no "aes" in the name, since it does not serve as
   distinction. </li>
   <li> The "f0" etc. are also to be removed from the name, since it is just
   a variation on the (fractional) number of rounds (and thus to be placed
   inside the directories). </li>
   <li> No need for the letters "c" etc. </li>
   <li> So for example "AES_r1_c1_rw4_e8_f1" becomes "1_4_8". </li>
   <li> Should one treat "1_4_8" the same as "2_2_8", since it results in
   the same overall sizes? </li>
   <li> We always have the same number of bits for plain/cipher text and for
   keys. Perhaps this number is most important? </li>
   <li> So the directory name should perhaps be just one number, the number
   of bits in the key (plain/cipher text)? </li>
   <li> So currently we would have directories "4, 32, 64, 128" (for the
   number of key-bits, which is the relevant aspect here). </li>
   <li> Inside these directories we then have directories like "1_4_8" as
   above. </li>
  </ul>


  \todo DONE Better directory names (regarding their order)
  <ul>
   <li> Directory names should be "004", "008", "032" etc., so that a file
   browser lists them automatically in the right order. </li>
   <li> Just to mention: moving files should always be done in *two* steps:
    <ol>
     <li> First move the files with "git mv", without change of content. </li>
     <li> Then update content to reflect new filenames. </li>
    </ol>
   </li>
  </ul>


  \todo DONE (see brief of 
  Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/general.hpp
  and other plans headers)
  Improve explanations of what a round is
  <ul>
   <li> Round always contain MixColumns. </li>
   <li> So this shouldn't be mentioned. </li>
   <li> Only the special cases *without* MixColumns need to be mentioned. </li>
   <li> A full round has always key-addition, SubBytes and MixColumns. </li>
   <li> We need nice notions when either key-addition or key-addition and
   SubBytes are added. </li>
   <li> Perhaps 1/3 rounds or 2/3 rounds. </li>
   <li> So we would speak, e.g., of 1+2/3 or 4+1/3 rounds. </li>
   <li> The last round could be notated by "2/3+1/3". </li>
   <li> AES then had 9+2/3+1/3 rounds (MixColumns replaced by key-addition).
   </li>
  </ul>

*/
