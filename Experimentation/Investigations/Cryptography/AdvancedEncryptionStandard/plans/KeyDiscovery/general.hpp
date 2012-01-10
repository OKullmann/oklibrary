// Matthew Gwynne, 23.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/general.hpp
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


  \todo Patterns
  <ul>
   <li> We list here any patterns noticed in the behaviour of solvers on
   key discovery instances, and suggest further avenues of investigation.
   </li>
   <li> 4-bit (field size) instances (so far):
    <ul>
     <li> All translations perform comparably; there is no clear "best"
     translation. </li>
    </ul>
   </li>
   <li> 8-bit (field size) instances (so far):
    <ul>
     <li> The 1-soft translations perform better by a considerable
     margin (e.g., 35 times faster; 1000 times less conflicts). </li>
     <li> Comparing "time to solve" (or "conflicts") vs
     "number of rounds":
      <ul>
       <li> 1-soft translations: (best) fit (equally well) by either a
       quadratic function, or exponential t ~ e^(0.2*r). </li>
       <li> minimum translations: (best) fit (equally well) by either a
       quintic function, or exponential t ~ e^(0.6*r). </li>
      </ul>
     That is, the minimum translation gets "harder, faster" as the
     number of rounds increases. </li>
     <li> We have experiments on instances of the form
     AES(r,1,1,8), AES(r,2,1,8) and AES(r,1,2,8). </li>
     <li> We should consider AES(r,1,3,8), AES(4,1,8) and AES(2,2,8) next
     to see if this pattern persists. </li>
     <li> We also need to almalgamate the data from all other
     experiments into this todo. </li>
    </ul>
   </li>
   <li> Most experiments so far have been with minisat-2.2.0.
   We should run other solvers and see if these patterns persist. </li>
  </ul>



  \todo Using multiple plaintext-ciphertext pairs
  <ul>
   <li> Investigating where providing multiple plaintext
   ciphertext pairs, for the *same* key, for AES key discovery
   instances improves solver performance on these instances. </li>
   <li> A key question here is also whether performance is improved
   more drastically for 1-soft translations compared to harder
   (i.e. minimum) translations. </li>
   <li> To compute an AES instance which uses k random plaintext-ciphertext
   pairs encrypted using key with seed s, we can:
    <ol>
     <li> Compute k instances of the AES instance in question; </li>
     <li> Instantiate each instance with the corresponding
     plaintext-ciphertext assignment; </li>
     <li> Apply renamings to all instances such that the only shared
     variables are the key variables; </li>
     <li> Append all renamed instances together, yielding the final
     instance. </li>
    </ol>
   </li>
   <li> This is a very naive implementation, and in fact we could
   do more, by combining the whole key schedule for the AES across
   instances. However, this is more complicated. </li>
   <li> We must now implement this idea and begin testing whether
   using multiple plaintext-ciphertext pairs improves solver
   performance. </li>
   <li> Generating 16 plaintext-ciphertext pairs for AES(20,2,2,4)
   using the canonical translation and key seeds 1 to 20:
   \verbatim
oklib_monitor : true$ num_pairs : 4$ rounds : 20$ num_columns : 2$ num_rows : 2$ exp : 4$ final_round_b : false$ box_tran : aes_ts_box$ mc_tran: aes_mc_bidirectional$
/* Generate the AES instances */
output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$
/* Generate the plaintext-ciphertext pairs */
for seed : 1 thru 20 do
  output_ss_random_pc_pairs(seed, num_pairs, rounds, num_columns, num_rows,exp, final_round_b)$
# Compute combined instances using ${num_pairs} plaintext-ciphertext pairs
k=4; s=1; r=20; rw=2; col=2; e=4; final_round=0;
AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${rw}_e${e}_f${final_round}.cnf ssaes_pcpair_r${r}_c${col}_rw${rw}_e${e}_f${final_round}_s${s}_k1.cnf  > r${r}_s${s}.cnf
let block_size=${rw}*${col}*${e}; max_index=$(cat r${r}_s${s}.cnf | awk ' /^p/ { print $3 }'); cur_index=${max_index};
cp r${r}_s${s}.cnf r${r}_s${s}_k1_cumul.cnf;
for i in $(seq 2 ${k}); do
  (seq $(expr ${cur_index} + 1) $(expr ${cur_index} + ${block_size});
   seq $(expr ${block_size} + 1) $(expr 2 '*' ${block_size});
   seq $(expr ${cur_index} + ${block_size} + 1) $(expr ${cur_index} + ${max_index})) > renaming_k${i};
   AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${rw}_e${e}_f${final_round}.cnf ssaes_pcpair_r${r}_c${col}_rw${rw}_e${e}_f${final_round}_s${s}_k${i}.cnf | RenameDimacs-O3-DNDEBUG renaming_k${i} > renamed_r${r}_s${s}_k${i}.cnf;
   AppendDimacs-O3-DNDEBUG r${r}_s${s}_k$(expr ${i} - 1)_cumul.cnf renamed_r${r}_s${s}_k${i}.cnf > r${r}_s${s}_k${i}_cumul.cnf;
   cur_index=$(expr ${cur_index} + ${cur_index});
done
   \endverbatim
   This should be moved to a script with application tests. </li>
   <li> We should now investigate the effect of increasing the number of
   pairs on how solvers perform on these instances. </li>
  </ul>


  \todo Explanations
  <ul>
   <li> For every set of AES parameters (rows, columns and field size), we
   need the following:
    <ul>
     <li> Problem specification:
      <ul>
       <li> We need explanations what a "round" etc. is, in elementary terms,
       using terms "addition, sub-bytes, shift-rows, mix-columns, round-key".
       </li>
       <li> This should go to the general.hpp file in the appropriate
       directory (e.g., KeyDiscovery/016/2_1_8/general.hpp). </li>
      </ul>
     </li>
     <li> Translations:
      <ul>
       <li> We need explanation of how to generate each instance, including
       which boxes are being used in each case. </li>
       <li> This can go to Translations.hpp (e.g.,
       KeyDiscovery/016/2_1_8/Translations.hpp). </li>
      </ul>
     </li>
     <li> Statistics:
      <ul>
       <li> We need statistics and explanations of them, for all
       instances. </li>
       <li> This can go to Statistics.hpp (e.g.,
       KeyDiscovery/016/2_1_8/Statistics.hpp). </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> The reason for splitting each of these are that it means that:
    <ul>
     <li> each file doesn't become too large and hard to navigate; </li>
     <li> statistics and so on can be provided for all rounds in one file,
     allowing easy comparison; </li>
     <li> information is not needlessly replicated. </li>
    </ul>
   </li>
   <li> In general, all discussions should be based on the general notions
   as introduced in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/IteratedBlockCipher.mac.
   </li>
   <li> Also the file-name needs to be explained (in each file). </li>
  </ul>


  \todo Naming of translations
  <ul>
   <li> For each experiment on some AES variant with some specific translation
   we have a todo. </li>
   <li> One must someone specify in the title and content of the todo which
   translation is meant. See "Investigating dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> For example "Using the canonical box translation" in
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/032/4_2_4/2_13.hpp
   doesn't provide enough information on its own to understand which
   translation is meant. </li>
   <li> The first thought is for the title of the todo to fully specify the
   translation. So for example, one would have "Using the whole Sbox and
   encryption-only Mixcolumns with "multiplication-based" decompositions
   translated using the canonical translation and Key schedule using the
   shared sub-expression translation.". </li>
   <li> The full specification is hard to read, however, and isn't appropriate
   for a todo heading. </li>
   <li> Another solution would be to specify defaults for each aspect of
   the translation, and then only specify the non-standard parts of the
   translation. The above example would then be
   "Using standard translation but with encryption-only MixColumns.". </li>
   <li> This is much more succinct but one must constantly refer to
   the "standard" translation. </li>
   <li> A balance between these two approaches is needed. </li>
   <li> One weapon in tackling the large number of dimensions of the AES
   translation is to derive overarching named concepts which categorise
   several dimensions at once. </li>
  </ul>


  \todo Links
  <ul>
   <li> For investigations into the various component/box representations
   (CNF and otherwise) of the AES boxes (Sbox, field multiplications etc) see
   Cryptography/AdvancedEncryptionStandard/plans/Representations/general.hpp .
   </li>
   <li> See
   Cryptography/AdvancedEncryptionStandard/plans/general.hpp
   for the more general investigations.
   </li>
  </ul>


  \todo Boundaries
  <ul>
   <li> This todo is out of date and must be updated. </li>
   <li> At the same time, the order should be changed to
   "N rows, N columns"; rows come first in a matrix specification.
   </li>
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
      computation with all solvers XXX. </li>
      <li> 16 column, 1 row, 8 bits, up to <em> 2 + 1/3 </em> rounds in
      <em>40s</em> with minisat-2.2.0 in 128/1_16_8/2_13.hpp. </li>
     </ul>
    </li>
    <li> 64 bit key:
     <ul>
      <li> 16 column, 1 row, 4 bits, up to <em>5 + 1/3</em> rounds in
      <em> 26 </em> seconds with minisat-2.2.0 in 064/1_16_4/4_13.hpp. </li>
      <li> 4 column, 4 row, 4 bits up to <em>1+1/3</em> rounds in
      <em> 21 </em> seconds with minisat-2.2.0 in 064/4_4_4/1_13.hpp.
      </li>
     </ul>
    </li>
    <li> 32 bit key:
     <ul>
      <li> 2 column, 4 row, 4 bits up to <em>2 + 1/3</em> rounds in
      <em> 2718 </em> seconds with glucose in 032/4_2_4/2_13.hpp. </li>
      <li> 4 column, 2 row, 4 bits up to <em>5 + 1/3</em> rounds in
      <em> 903 </em> seconds with picosat in 032/2_4_4/5_13.hpp. </li>
     </ul>
    </li>
    <li> 16 bit key:
     <ul>
      <li> 1 column, 2 row, 8 bits up to <em>20 + 1/3</em> rounds in
      <em> 268s </em> seconds with minisat-2.2.0 in 016/2_1_8/general.hpp. </li>
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
      <em> 0.0 </em> seconds, and  with <em> 1 </em> decision, with OKsolver
      in 004/1_1_4/20_13.hpp. </li>
     </ul>
    </li>
   </ul>
   </li>
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

*/
