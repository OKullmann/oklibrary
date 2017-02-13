-// Matthew Gwynne, 11.4.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Benchmarks/docus/SAT2012.hpp
  \brief Benchmarks for the SAT 2012 competitions

  See http://baldur.iti.kit.edu/SAT-Challenge-2012/ for the SAT competition
  webpage.

  The specifications of the competition formats can be found at
  http://www.satcompetition.org/2011/format-benchmarks2011.html
  (same as for SAT2011).


  <h1> Advanced Encryption Standard II benchmarks </h1>

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
OKlib/Experimentation/Benchmarks/SAT2012_AES> mkdir -p Assignments/{16,20,24,32} Benchmarks/{16,20,24,32} Formulas/{16,20,24,32}
OKlib/Experimentation/Benchmarks/SAT2012_AES> cd Formulas/32
OKlib/Experimentation/Benchmarks/SAT2012_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r :1 thru 2 do output_ss_fcl_std(r,2,2,8,false,aes_small_box,aes_mc_forward);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2012_AES> for r in 1 2; do mv ssaes_r${r}_c2_rw2_e8_f0.cnf aes_32_${r}.cnf; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> cd ../24
OKlib/Experimentation/Benchmarks/SAT2012_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r : 1 thru 4 do output_ss_fcl_std(r,3,1,8,false,aes_small_box,aes_mc_forward);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2012_AES> for r in $(seq 1 4); do mv ssaes_r${r}_c3_rw1_e8_f0.cnf aes_24_${r}.cnf; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> cd ../20
OKlib/Experimentation/Benchmarks/SAT2012_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> output_ss_fcl_std(20,5,1,4,false,aes_small_box,aes_mc_forward);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2012_AES> mv ssaes_r20_c5_rw1_e4_f0.cnf aes_20_20.cnf
OKlib/Experimentation/Benchmarks/SAT2012_AES> cd ../16
OKlib/Experimentation/Benchmarks/SAT2012_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r : 1 thru 10 do output_ss_fcl_std(r,1,2,8,false,aes_small_box,aes_mc_forward);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2012_AES> for r in $(seq 1 10); do mv ssaes_r${r}_c1_rw2_e8_f0.cnf aes_16_${r}.cnf; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> cd ../../Assignments/32
OKlib/Experimentation/Benchmarks/SAT2012_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r : 1 thru 2 do for s : 1 thru 5 do output_ss_random_pc_pair(s,r,2,2,8,false);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2012_AES> for r in 1 2; do for s in $(seq 1 5); do mv ssaes_pcpair_r${r}_c2_rw2_e8_f0_s${s}.cnf aes_ass_32_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> cd ../24
OKlib/Experimentation/Benchmarks/SAT2012_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r :1 thru 4 do for s : 1 thru 5 do output_ss_random_pc_pair(s,r,3,1,8,false);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2012_AES> for r in $(seq 1 4); do for s in $(seq 1 5); do mv ssaes_pcpair_r${r}_c3_rw1_e8_f0_s${s}.cnf aes_ass_24_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> cd ../20
OKlib/Experimentation/Benchmarks/SAT2012_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for s : 1 thru 5 do output_ss_random_pc_pair(s,20,5,1,4,false);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2012_AES> r=20; for s in $(seq 1 5); do mv ssaes_pcpair_r${r}_c5_rw1_e4_f0_s${s}.cnf aes_ass_20_${r}_keyfind_${s}.cnf; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> cd ../16
OKlib/Experimentation/Benchmarks/SAT2012_AES> oklib --maxima
maxima> oklib_load_all()$
maxima> for r : 1 thru 10 do for s : 1 thru 5 do output_ss_random_pc_pair(s,r,1,2,8,false);
maxima> quit();
OKlib/Experimentation/Benchmarks/SAT2012_AES> for r in $(seq 1 10); do for s in $(seq 1 5); do mv ssaes_pcpair_r${r}_c1_rw2_e8_f0_s${s}.cnf aes_ass_16_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> cd ../../
OKlib/Experimentation/Benchmarks/SAT2012_AES> bits=32; for r in $(seq 1 2); do for s in $(seq 1 5); do AppendDimacs-O3-DNDEBUG Formulas/${bits}/aes_${bits}_${r}.cnf Assignments/${bits}/aes_ass_${bits}_${r}_keyfind_${s}.cnf > Benchmarks/${bits}/aes_${bits}_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> bits=24; for r in 3 4; do for s in $(seq 1 5); do AppendDimacs-O3-DNDEBUG Formulas/${bits}/aes_${bits}_${r}.cnf Assignments/${bits}/aes_ass_${bits}_${r}_keyfind_${s}.cnf > Benchmarks/${bits}/aes_${bits}_${r}_keyfind_${s}.cnf; done; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> bits=20; r=20; for s in $(seq 1 5); do AppendDimacs-O3-DNDEBUG Formulas/${bits}/aes_${bits}_${r}.cnf Assignments/${bits}/aes_ass_${bits}_${r}_keyfind_${s}.cnf > Benchmarks/${bits}/aes_${bits}_${r}_keyfind_${s}.cnf; done
OKlib/Experimentation/Benchmarks/SAT2012_AES> bits=16; for r in 3 4 5 6 10; do for s in $(seq 1 5); do AppendDimacs-O3-DNDEBUG Formulas/${bits}/aes_${bits}_${r}.cnf Assignments/${bits}/aes_ass_${bits}_${r}_keyfind_${s}.cnf > Benchmarks/${bits}/aes_${bits}_${r}_keyfind_${s}.cnf; done; done
   \endverbatim
   </li>
   <li> The instances are then found in SAT2012_AES/Benchmarks/. </li>
   </li>
   <li> The Git-ID of the relevant state of the OKlibrary is
   \verbatim
e3896a3b8e00cf8c46739fded03a6835117c01cb
   \endverbatim
   </li>
   <li> The package-template is given by the directory
   <code>OKlib/Experimentation/Benchmarks/SAT2012_AES</code>. </li>
   <li> Created by
   \verbatim
OKlib/Experimentation/Benchmarks> tar -cjf SAT2012_AES.tar.bz2 SAT2012_AES
   \endverbatim
   </li>
   <li> The package has md5sum
   \verbatim
2050e06bc9ae36fa848566834727f141  SAT2012_AES.tar.bz2
   \endverbatim
   and 4,884,115 bytes. </li>
   <li> Extracting this archive (by
   <code>tar -xjf SAT2012_AES.tar.bz2</code>) yields directory
   <code>SAT2012_AES</code>. </li>
  </ul>

*/

