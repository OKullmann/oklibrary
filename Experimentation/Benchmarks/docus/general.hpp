// Oliver Kullmann, 13.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Benchmarks/docus/general.hpp
  \brief Central docus-file regarding benchmark packages


  <h1> SAT 2011 </h1>

  <h2> Van der Waerden instances </h2>

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
ab24cceb1074bc2a0c4486922b415de60bd3cb07
   \endverbatim
   </li>
   <li> The package-template is given by the directory
   <code>OKlib/Experimentation/Benchmarks/SAT2011_VanDerWaerden</code>. </li>
   <li> The "empty" package (the template-directory, without the instances,
   but with the generator-executables added) has md5sum
   \verbatim
5b355957671e20f033bd130af4d633b8  SAT2011_VanDerWaerden_Empty.tar.bz2
   \endverbatim
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
eec7580151a41b53d263845c2c2ebdb7  SAT2011_VanDerWaerden.tar.bz2
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


  <h2> Green-Tao instances </h2>

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
ab24cceb1074bc2a0c4486922b415de60bd3cb07
   \endverbatim
   </li>
   <li> The package-template is given by the directory
   <code>OKlib/Experimentation/Benchmarks/SAT2011_GreenTao</code>. </li>
   <li> The "empty" package (the template-directory, without the instances,
   but with the generator-executables added) has md5sum
   \verbatim
0bbbef7b9581bd15135fa2aea1576e7c  SAT2011_GreenTao_Empty.tar.bz2
   \endverbatim
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
f78b135ae128e679aaf7fc7cd50dfd71  SAT2011_GreenTao.tar.bz2
   \endverbatim
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


  <h2> AES instances </h2>

  General information:
  <ul>
   <li> Portable generators not available. Generation occurs using
   "output_ss_fcl_std" in 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac.
   </li>
   <li> The generated instances are created by:
   \verbatim
OKlib/Experimentation/Benchmarks/SAT2011_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> output_ss_fcl_std(10,4,4,8,true,aes_small_box,aes_mc_forward);
maxima> output_ss_fcl_std(2,4,4,8,true,aes_small_box,aes_mc_bidirectional);
maxima> output_ss_fcl_std(2,4,4,4,true,aes_small_box,aes_mc_bidirectional);
maxima> output_ss_random_pk_pair(1,10,4,4,8,true);
maxima> output_ss_random_kc_pair(1,10,4,4,8,true);
maxima> output_ss_random_pc_pair(1,2,4,4,8,true);
maxima> output_ss_random_pc_pair(1,2,4,4,4,true);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv ssaes_r10_c4_rw4_e8_f1.cnf aes_128.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv ssaes_r2_c4_rw4_e8_f1.cnf aes_128_2.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv ssaes_r2_c4_rw4_e4_f1.cnf aes_64_2.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv ssaes_pkpair_r10_c4_rw4_e8_f1_s1.cnf aes_ass_128_encrypt.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv ssaes_kcpair_r10_c4_rw4_e8_f1_s1.cnf aes_ass_128_decrypt.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv ssaes_pcpair_r2_c4_rw4_e8_f1_s1.cnf aes_ass_128_2_keyfind.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv ssaes_pcpair_r2_c4_rw4_e4_f1_s1.cnf aes_ass_64_2_keyfind.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> AppendDimacs-O3-DNDEBUG aes_128.cnf aes_ass_128_encrypt.cnf > aes_128_encrypt.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> AppendDimacs-O3-DNDEBUG aes_128.cnf aes_ass_128_decrypt.cnf > aes_128_decrypt.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> AppendDimacs-O3-DNDEBUG aes_128_2.cnf aes_ass_128_2_keyfind.cnf > aes_128_2_keyfind.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> AppendDimacs-O3-DNDEBUG aes_64_2.cnf aes_ass_64_2_keyfind.cnf > aes_64_2_keyfind.cnf
OKlib/Experimentation/Benchmarks/SAT2011_AES> mkdir -p EncryptionDecryption KeyDiscovery/128 KeyDiscovery/64
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv aes_128.cnf aes_128_encrypt.cnf aes_128_decrypt.cnf aes_ass_128_encrypt.cnf aes_ass_128_decrypt.cnf EncryptionDecryption/.
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv aes_128_2.cnf aes_128_2_keyfind.cnf aes_ass_128_2_keyfind.cnf KeyDiscovery/128/.
OKlib/Experimentation/Benchmarks/SAT2011_AES> mv aes_64_2.cnf aes_64_2_keyfind.cnf aes_ass_64_2_keyfind.cnf KeyDiscovery/64/.
   \endverbatim
   </li>
   <li> The main files of interest are then:
   <ul>
    <li> EncryptionDecryption/aes_128_encrypt.cnf 
    (128-bit 10 round AES encryption). </li>
    <li> EncryptionDecryption/aes_128_decrypt.cnf 
    (128-bit 10 round AES decryption). </li>
    <li> KeyDiscovery/128/aes_128_2_keyfind.cnf 
    (128-bit 2 round AES key discovery). </li>
    <li> KeyDiscovery/64/aes_64_2_keyfind.cnf 
    (64-bit 2 round small scale AES key discovery). 
    </li>
   </ul>
   </li>
   <li> The Git-ID of the relevant state of the OKlibrary is
   \verbatim
54afbadfb72018a4cb9ec5fc77b2d47252095009
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
1443b70e365a0dd3b3363600d4542971  SAT2011_AES.tar.bz2
   \endverbatim
   </li>
   <li> Extracting this archive (by
   <code>tar -xjf SAT2011_AES.tar.bz2</code>) yields directory
   <code>SAT2011_AES</code>. </li>
  </ul>

*/

