-// Oliver Kullmann, 26.2.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Benchmarks/docus/SAT2011.hpp
  \brief Benchmarks for the SAT 2011 competitions

  See
  <ul>
   <li> http://www.cril.univ-artois.fr/SAT11/ for the SAT competition webpage.
   </li>
   <li> http://maxsat.ia.udl.cat/ for the MaxSAT competition webpage (note
   that this updates each year). </li>
   <li> http://www.cril.univ-artois.fr/PB11/ for the pseudo-boolean
   competition webpage. </li>
  </ul>

  The specifications of each of the competition formats can be found at:
  <ul>
   <li> SAT benchmarks -
   http://www.satcompetition.org/2011/format-benchmarks2011.html . </li>
   <li> PseudoBoolean benchmarks -
   http://www.cril.univ-artois.fr/PB11/format.pdf . </li>
   <li> MaxSAT benchmarks - http://maxsat.ia.udl.cat/rules/ and
   http://maxsat.ia.udl.cat/requirements/ . </li>
  </ul>


  <h1> Van der Waerden instances </h1>

  General information:
  <ul>
   <li> Two generators:
    <ol>
     <li> <code>VanderWaerdenCNF-O3-DNDEBUG</code>
     (Satisfiability/Transformers/Generators/VanderWaerdenCNF.cpp) </li>
     <li> Creates ordinary vdW-instances, encoding "vdw_2(k1,k2) > n"
     (satisfiable iff the answer is yes). </li>
     <li> <code>PdVanderWaerdenCNF-O3-DNDEBUG</code>
     (Satisfiability/Transformers/Generators/PdVanderWaerdenCNF.cpp) </li>
     <li> Creates palindromic vdW-instances, encoding "vdw_2^pd(k1,k2) > n"
     (satisfiable iff the answer is yes). </li>
    </ol>
   </li>
   <li> These generators-executables, statically linked, are created by
   \verbatim
OKlib/Satisfiability/Transformers/Generators> oklib cleanall
OKlib/Satisfiability/Transformers/Generators> LDFLAGS="-static" oklib all
   \endverbatim
   and then copied from <code>OKplatform/system_directories/bin</code>.
   </li>
   <li> The Git-ID of the relevant state of the OKlibrary is
   \verbatim
9fc269649716c7a848878fe58edfbdadf11faff0
   \endverbatim
   </li>
   <li> The package-template is given by the directory
   <code>OKlib/Experimentation/Benchmarks/SAT2011_VanDerWaerden</code>. </li>
   <li> The "empty" package (the template-directory, without the instances,
   but with the generator-executables added) has md5sum
   \verbatim
a1714135997bc1c52300f842be4093f7  SAT2011_VanDerWaerden_Empty.tar.bz2
   \endverbatim
   and 4,556,198 bytes.
   </li>
   <li> Created by
   \verbatim
OKlib/Experimentation/Benchmarks> tar -cjf SAT2011_VanDerWaerden_Empty.tar.bz2 SAT2011_VanDerWaerden
   \endverbatim
   </li>
   <li> Extracting this archive (by
   <code>tar -xjf SAT2011_VanDerWaerden_Empty.tar.bz2</code>) yields directory
   <code>SAT2011_VanDerWaerden</code>. </li>
   <li> The "full" package (additionally with the instances) has md5sum
   \verbatim
5c6a16114a3de773b54f15c7ef711959  SAT2011_VanDerWaerden.tar.bz2
   \endverbatim
   and size 42,057,116 bytes.
   </li>
   <li> Created by
   \verbatim
OKlib/Experimentation/Benchmarks> tar -cjf SAT2011_VanDerWaerden.tar.bz2 SAT2011_VanDerWaerden
   \endverbatim
   </li>
   <li> From the "empty" package the "full" package is created by running
   <code>./Create</code> in the (extracted) directory
   <code>SAT2011_VanDerWaerden</code>.
    <ol>
     <li> This script prints "SUCCESS" and returns code 0 if no error occurred.
     </li>
     <li> This includes checking of the md5sums of the two generators and all
     the benchmark files. </li>
    </ol>
   </li>
  </ul>

  Competition results:
  <ul>
   <li> See
    http://www.cril.univ-artois.fr/SAT11/results/globalbybench.php?idev=46&idcat=63
    </li>
  </ul>


  <h1> Green-Tao instances </h1>

  General information:
  <ul>
   <li> One generator:
    <ol>
     <li> <code>GreenTaoCNF-O3-DNDEBUG</code>
     (Satisfiability/Transformers/Generators/GreenTaoCNF.cpp) </li>
     <li> Creates gt-instances, encoding "grt_2(k1,k2) > n"
     (satisfiable iff the answer is yes). </li>
     <li> <code>GTSat</code> is a wrapper script, which in general allows
     also to create problems using more than two colours, however this
     functionality is not available for this copy. </li>
    </ol>
   </li>
   <li> The generator-executable, statically linked, is created by
   \verbatim
OKlib/Satisfiability/Transformers/Generators> oklib cleanall
OKlib/Satisfiability/Transformers/Generators> LDFLAGS="-static" oklib all
   \endverbatim
   and then copied from <code>OKplatform/system_directories/bin</code>.
   </li>
   <li> The Git-ID of the relevant state of the OKlibrary is
   \verbatim
96183df1d88c240d98663666726d7d79cc34fdc0
   \endverbatim
   </li>
   <li> The package-template is given by the directory
   <code>OKlib/Experimentation/Benchmarks/SAT2011_GreenTao</code>. </li>
   <li> The "empty" package (the template-directory, without the instances,
   but with the generator-executables added) has md5sum
   \verbatim
623f44fddfa3dc002a488e71e73e2f43  SAT2011_GreenTao_Empty.tar.bz2
   \endverbatim
   and 2,373,674 bytes.
   </li>
   <li> Created by
   \verbatim
OKlib/Experimentation/Benchmarks> tar -cjf SAT2011_GreenTao_Empty.tar.bz2 SAT2011_GreenTao
   \endverbatim
   </li>
   <li> Extracting this archive (by
   <code>tar -xjf SAT2011_GreenTao_Empty.tar.bz2</code>) yields directory
   <code>SAT2011_GreenTao</code>. </li>
   <li> The "full" package (additionally with the instances) has md5sum
   \verbatim
94b7f8bfd62eab7c399a27bdf81bc652  SAT2011_GreenTao.tar.bz2
   \endverbatim
   and size 63,910,079 bytes.
   </li>
   <li> Created by
   \verbatim
OKlib/Experimentation/Benchmarks> tar -cjf SAT2011_GreenTao.tar.bz2 SAT2011_GreenTao
   \endverbatim
   after running <code>Create</code>.
   </li>
   <li> From the "empty" package the "full" package is created by running
   <code>./Create</code> in the (extracted) directory
   <code>SAT2011_GreenTao</code>.
    <ol>
     <li> This script prints "SUCCESS" and returns code 0 if no error occurred.
     </li>
     <li> This includes checking of the md5sums of the two generator-files and
     all the benchmark files. </li>
    </ol>
   </li>
  </ul>

  Competition results:
  <ul>
   <li> See
    http://www.cril.univ-artois.fr/SAT11/results/globalbybench.php?idev=46&idcat=63
    </li>
  </ul>


  <h1> Advanced Encryption Standard I benchmarks </h1>

  General information:
  <ul>
   <li> Generation occurs using "output_ss_fcl_std" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac.
   </li>
   <li> The concatenation tool, <code>AppendDimacs-O3-DNDEBUG</code>,
   statically linked, is created by
   \verbatim
OKlib/Satisfiability/Interfaces/InputOutput> oklib cleanall
OKlib/Satisfiability/Interfaces/InputOutput> LDFLAGS="-static" oklib all
   \endverbatim
   and then copied from <code>OKplatform/system_directories/bin</code>.
   </li>
   <li> The generated instances are created by:
   \verbatim
OKlib/Experimentation/Benchmarks/SAT2011_AES> mkdir -p Assignments/{128,64,32} Benchmarks/{128,64,32} Formulas/{128,64,32}
OKlib/Experimentation/Benchmarks/SAT2011_AES> cd Formulas/128
OKlib/Experimentation/Benchmarks/SAT2011_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r :1 thru 3 do output_ss_fcl_std(r,4,4,8,false,aes_small_box,aes_mc_forward);
maxima> output_ss_fcl_std(10,4,4,8,true,aes_small_box,aes_mc_forward);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2011_AES> for r in 1 2 3; do mv ssaes_r${r}_c4_rw4_e8_f0.cnf aes_128_${r}.cnf; done
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv ssaes_r10_c4_rw4_e8_f1.cnf aes_128_10.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> cd ../64
OKlib/Experimentation/Benchmarks/SAT2011_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r :1 thru 4 do output_ss_fcl_std(r,4,4,4,false,aes_small_box,aes_mc_forward);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2011_AES> for r in $(seq 1 4); do mv ssaes_r${r}_c4_rw4_e4_f0.cnf aes_64_${r}.cnf; done
OKlib/Experimentation/Benchmarks/SAT2011_AES> cd ../32
OKlib/Experimentation/Benchmarks/SAT2011_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r :1 thru 5 do output_ss_fcl_std(r,4,2,4,false,aes_small_box,aes_mc_forward);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2011_AES> for r in $(seq 1 5); do mv ssaes_r${r}_c4_rw2_e4_f0.cnf aes_32_${r}.cnf; done
OKlib/Experimentation/Benchmarks/SAT2011_AES> cd ../../Assignments/128
OKlib/Experimentation/Benchmarks/SAT2011_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r :1 thru 3 do for s in [1,2] do output_ss_random_pc_pair(s,r,4,4,8,false);
maxima> for s in [1,2] do output_ss_random_pc_pair(s,10,4,4,8,true);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2011_AES> for r in 1 2 3; do for s in 1 2; do mv ssaes_pcpair_r${r}_c4_rw4_e8_f0_s${s}.cnf aes_ass_128_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2011_AES> for s in 1 2; do mv ssaes_pcpair_r10_c4_rw4_e8_f1_s${s}.cnf aes_ass_128_10_keyfind_${s}.cnf; done
OKlib/Experimentation/Benchmarks/SAT2011_AES> cd ../64
OKlib/Experimentation/Benchmarks/SAT2011_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r :1 thru 4 do for s in [1,2] do output_ss_random_pc_pair(s,r,4,4,4,false);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2011_AES> for r in $(seq 1 4); do for s in 1 2; do mv ssaes_pcpair_r${r}_c4_rw4_e4_f0_s${s}.cnf aes_ass_64_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2011_AES> cd ../32
OKlib/Experimentation/Benchmarks/SAT2011_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r :1 thru 5 do for s in [1,2] do output_ss_random_pc_pair(s,r,4,2,4,false);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2011_AES> for r in $(seq 1 5); do for s in 1 2; do mv ssaes_pcpair_r${r}_c4_rw2_e4_f0_s${s}.cnf aes_ass_32_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2011_AES> cd ../../
OKlib/Experimentation/Benchmarks/SAT2011_AES> bits=128; for r in 1 2 3 10; do for s in 1 2; do AppendDimacs-O3-DNDEBUG Formulas/${bits}/aes_${bits}_${r}.cnf Assignments/${bits}/aes_ass_${bits}_${r}_keyfind_${s}.cnf > Benchmarks/${bits}/aes_${bits}_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2011_AES> bits=64; for r in 1 2 3 4; do for s in 1 2; do AppendDimacs-O3-DNDEBUG Formulas/${bits}/aes_${bits}_${r}.cnf Assignments/${bits}/aes_ass_${bits}_${r}_keyfind_${s}.cnf > Benchmarks/${bits}/aes_${bits}_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2011_AES> bits=32; for r in 1 2 3 4 5; do for s in 1 2; do AppendDimacs-O3-DNDEBUG Formulas/${bits}/aes_${bits}_${r}.cnf Assignments/${bits}/aes_ass_${bits}_${r}_keyfind_${s}.cnf > Benchmarks/${bits}/aes_${bits}_${r}_keyfind_${s}.cnf; done; done
   \endverbatim
   </li>
   <li> The instances are then found in SAT2011_AES/Benchmarks/. </li>
   </li>
   <li> The Git-ID of the relevant state of the OKlibrary is
   \verbatim
0293c1751342d05f76180be93a23c3771d9c0362
   \endverbatim
   </li>
   <li> The package-template is given by the directory
   <code>OKlib/Experimentation/Benchmarks/SAT2011_AES</code>. </li>
   <li> Created by
   \verbatim
OKlib/Experimentation/Benchmarks> tar -cjf SAT2011_AES.tar.bz2 SAT2011_AES
   \endverbatim
   </li>
   <li> The package has md5sum
   \verbatim
2a1f18ddf5a9faf4fcc445704034c4b4  SAT2011_AES.tar.bz2
   \endverbatim
   and 4,884,115 bytes. </li>
   <li> Extracting this archive (by
   <code>tar -xjf SAT2011_AES.tar.bz2</code>) yields directory
   <code>SAT2011_AES</code>. </li>
  </ul>


  <h1> The AES Challenge </h1>

  General information:
  <ul>
   <li> Generation occurs using "output_ss_fcl_std" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac.
   </li>
   <li> The concatenation tool, <code>AppendDimacs-O3-DNDEBUG</code>,
   statically linked, is created by
   \verbatim
OKlib/Satisfiability/Interfaces/InputOutput> oklib cleanall
OKlib/Satisfiability/Interfaces/InputOutput> LDFLAGS="-static" oklib all
   \endverbatim
   and then copied from <code>OKplatform/system_directories/bin</code>.
   </li>
   <li> The generated instances are created by:
   \verbatim
OKlib/Experimentation/Benchmarks/SAT2011_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> output_ss_fcl_std(10,4,4,8,true,aes_small_box,aes_mc_forward);
maxima> plaintext_lits : ss_matrix2pa(ss_hex2matrix(int2hex(275930429712199798024509060124983156862), 2,8,ss_polynomial_2_8,4), create_list(i,i,1,128),2,8,ss_polynomial_2_8)$
maxima> ciphertext_lits : ss_matrix2pa(ss_hex2matrix(int2hex(82288044290978544244364916044704454498), 2,8,ss_polynomial_2_8,4), create_list(i,i,257,384),2,8,ss_polynomial_2_8)$
maxima> comment : sconcat(
      "P :  CF964488F8FD93A6F30E9B4EDE0FE07E ",
      "C : 3DE819D1BB624DDA35E4445D12C31762")$
maxima> pc_phi : append(plaintext_lits, ciphertext_lits)$
maxima> pc_unit_fcs : [
  setify(create_list(i,i,1, 384)),setify(map(set, pc_phi))]$
maxima> output_fcs(comment, pc_unit_fcs, "aes_ass_challenge.cnf")$
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv ssaes_r10_c4_rw4_e8_f1.cnf aes_128_10.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> AppendDimacs-O3-DNDEBUG aes_128_10.cnf aes_ass_challenge.cnf > aes_challenge.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> rm aes_128_10.cnf aes_ass_challenge.cnf
   \endverbatim
   </li>
   <li> We also remove by hand the generated comments at the beginning of the
   file and replace with the following:
   \verbatim
c The AES Challenge
c
c Derive the 128-bit key which encrypts the plain-text
c
c   CF964488F8FD93A6F30E9B4EDE0FE07E
c
c to the cipher-text
c
c   3DE819D1BB624DDA35E4445D12C31762.
c
   \endverbatim
   </li>
   <li> The AES challenge instance is <code>aes_challenge.cnf</code>. </li>
   </li>
   <li> The Git-ID of the relevant state of the OKlibrary is
   \verbatim
0293c1751342d05f76180be93a23c3771d9c0362
   \endverbatim
   </li>
   <li> The package-template is given by the directory
   <code>OKlib/Experimentation/Benchmarks/SAT2011_AES-Challenge</code>. </li>
   <li> Created by
   \verbatim
OKlib/Experimentation/Benchmarks> tar -cjf SAT2011_AES-Challenge_OKlibrary.tar.bz2 SAT2011_AES-Challenge
   \endverbatim
   </li>
   <li> The package has md5sum
   \verbatim
45b5b7e532cea6bb7861f2a5d0d476aa  SAT2011_AES-Challenge_OKlibrary.tar.bz2
   \endverbatim
   and 492,465 bytes. </li>
   <li> Extracting this archive (by
   <code>tar -xjf SAT2011_AES-Challenge_OKlibrary.tar.bz2</code>) yields
   directory <code>SAT2011_AES-Challenge</code>. </li>
  </ul>


  <h1> Advanced Encryption Standard component benchmarks (MaxSAT)
  </h1>

  General information:
  <ul>
   <li> Generation of full CNFs for boolean functions for the
    <ul>
     <li> AES Sbox using "output_ss_sbox_fullcnf_stdname" in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac.
     </li>
     <li> Field multiplications using "output_ssmult_fullcnf_stdname" in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac.
     </li>
    </ul>
   </li>
   <li> The generated instances are created by:
   \verbatim
maxima> output_ss_sbox_fullcnf_stdname(2,4,ss_polynomial_2_4);
maxima> output_ss_sbox_fullcnf_stdname(2,8,ss_polynomial_2_8);
maxima> for e in [3,9,11,13,14] do output_ssmult_fullcnf_stdname(e,2,8,ss_polynomial_2_8);
shell> mkdir Benchmarks
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_sbox_2_4_full.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > Benchmarks/sbox_4.wcnf
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_sbox_2_8_full.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > Benchmarks/sbox_8.wcnf
shell> for x in 3 9 11 13 14; do QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG ss_byte2_8_field_mul_full_${x}.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > Benchmarks/mul_8_${x}.wcnf; done
shell> rm *_primes*
   \endverbatim
   </li>
   <li> The instances are then found in SAT2011-MaxSAT_AES/Benchmarks/. </li>
   </li>
   <li> The Git-ID of the relevant state of the OKlibrary is
   \verbatim
5981af725f2d993da932ab82cf9f3748a61faddb
   \endverbatim
   </li>
   <li> The package-template is given by the directory
   <code>OKlib/Experimentation/Benchmarks/SAT2011-MaxSAT_AES</code>. </li>
   <li> Created by
   \verbatim
OKlib/Experimentation/Benchmarks> tar -cjf SAT2011-MaxSAT_AES.tar.bz2 SAT2011-MaxSAT_AES
   \endverbatim
   </li>
   <li> The package has md5sum
   \verbatim
184741b55a8359796f34593db4f32537  SAT2011-MaxSAT_AES.tar.bz2
   \endverbatim
   and 48,194,818 bytes. </li>
   <li> Extracting this archive (by
   <code>tar -xjf SAT2011-MaxSAT_AES.tar.bz2</code>) yields directory
   <code>SAT2011-MaxSAT_AES</code>. </li>
  </ul>


  <h1> Advanced Encryption Standard component benchmarks (pseudo-boolean)
  </h1>

  General information:
  <ul>
   <li> Generation of full CNFs for boolean functions for the
    <ul>
     <li> AES Sbox using "output_ss_sbox_fullcnf_stdname" in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac.
     </li>
     <li> Field multiplications using "output_ssmult_fullcnf_stdname" in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac.
     </li>
    </ul>
   </li>
   <li> The generated instances are created by:
   \verbatim
maxima> output_ss_sbox_fullcnf_stdname(2,4,ss_polynomial_2_4);
maxima> output_ss_sbox_fullcnf_stdname(2,8,ss_polynomial_2_8);
maxima> for e in [3,9,11,13,14] do output_ssmult_fullcnf_stdname(e,2,8,ss_polynomial_2_8);
shell> mkdir Benchmarks
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_sbox_2_4_full.cnf | MinOnes2PseudoBoolean-O3-DNDEBUG > Benchmarks/sbox_4_shg.opb
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_sbox_2_8_full.cnf | MinOnes2PseudoBoolean-O3-DNDEBUG > Benchmarks/sbox_8_shg.opb
shell> for x in 3 9 11 13 14; do QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG ss_byte2_8_field_mul_full_${x}.cnf | MinOnes2PseudoBoolean-O3-DNDEBUG > Benchmarks/mul_8_${x}_shg.opb; done
shell> rm *_primes*
   \endverbatim
   </li>
   <li> The instances are then found in SAT2011-PB_AES/Benchmarks/. </li>
   </li>
   <li> The Git-ID of the relevant state of the OKlibrary is
   \verbatim
0293c1751342d05f76180be93a23c3771d9c0362
   \endverbatim
   </li>
   <li> The package-template is given by the directory
   <code>OKlib/Experimentation/Benchmarks/SAT2011-PB_AES</code>. </li>
   <li> Created by
   \verbatim
OKlib/Experimentation/Benchmarks> tar -cjf SAT2011-PB_AES.tar.bz2 SAT2011-PB_AES
   \endverbatim
   </li>
   <li> The package has md5sum
   \verbatim
3dcddd0dfb9400fbdaa35f6dcf2e39e5  SAT2011-PB_AES.tar.bz2
   \endverbatim
   and 56,365,764 bytes. </li>
   <li> Extracting this archive (by
   <code>tar -xjf SAT2011-PB_AES.tar.bz2</code>) yields directory
   <code>SAT2011-PB_AES</code>. </li>
  </ul>

*/

