// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AES/plans/BreakingAES_R2-6.hpp
  \brief On investigations into breaking reduced round variants of the AES


  \todo Breaking 1 round AES using the canonical translation for Sbox and Mul
  <ul>
   <li> The aim here is to monitor the performance of a variety of solvers on
   the 1 round variant of the "canonical" AES translation, providing all 
   plaintext and ciphertext bits but leaving n key bits unknown where n ranges
   from 0 to 128. </li>
   <li> For the sake of simple experimental, the plaintext and key are all
   zero and the ciphertext is the corresponding ciphertext given 1-round AES. 
   Further experimentation can be done using random keys if a pattern emerges.
   </li>
   <li> Note, in the context of the problem, the full key is known (all zeros)
   but how much of this key is provided in the SAT problem to the SAT solver
   is then restricted, and the SAT problem is then to derive this key. </li>
   <li> To setup this experiment a directory is created with the following: 
    <ul>
     <li> "AES_R1.cnf" - The AES translation using new variables restricted
     to 1 round. </li>
     <li> "AES_UC_P0_K0_CX_KN$N.cnf" - The generated unit clauses specifying the
     plaintext, ciphertext and key bits (variables) to be set, where $N is the
     number of key bits being specified in the problem. </li>
     <li> "run_experiment.sh" - A shell script called to run the experiment 
     (see below). </li>
     <li> "generate_aes_exp.sh" - A shell script called to generate the list
     of experiments/commands to run (outputs to file "experiments" - see below).
     </li>
     <li> "merge_cnf.sh" - A shell script which takes 2 Dimacs filenames as 
     input and outputs to STDOUT a new Dimacs file which has all the clauses 
     from both input files, and an updated p-line to account for the sum of the 
     two files. Note: It is assumed the variable set of the first Dimacs file 
     encompasses the second. </li>
    </ul>
   </li>
   <li>
   </li>
   <li> Generating "AES_R1.cnf":
   \verbatim
:lisp (ext:set-limit 'ext:heap-size 3000000000)
:lisp (ext:set-limit 'ext:frame-stack 10000)
:lisp (ext:set-limit 'ext:c-stack 200000)
:lisp (ext:set-limit 'ext:lisp-stack 200000)
oklib_load_all()$
oklib_monitoring : true$
aes_num_rounds : 1$
block([  
  aes_sbox_cp : aes_sbox_ts_cp,
  aes_mul2_cp : aes_mul2_ts_cp,
  aes_mul3_cp : aes_mul3_ts_cp,
  aes_mul9_cp : aes_mul9_ts_cp,
  aes_mul11_cp : aes_mul11_ts_cp,
  aes_mul13_cp : aes_mul13_ts_cp,
  aes_mul14_cp : aes_mul14_ts_cp], 
  output_aes_cnf_fcs_stdname())$
   \endverbatim
Note, the ":lisp" commands are necessary to ensure the Maxima/lisp stack and 
heap are large enough to store the translation.
   </li>
   <li> The unit clauses "AES_UC_P0_K0_CX_KN$N.cnf" can be generated
   in the following way:
   \verbatim
aes_num_rounds : 1$
for bits_to_remove : 0 thru 128 do block([PA],
  PA : map(set,
    union(
      aes_hex2pa("00000000000000000000000000000000", create_list(i,i,1,128)),
      aes_hex2pa("00000000000000000000000000000000", create_list(i,i,129,256)), 
      aes_hex2pa(il2hex(
        aes_encrypt_l(
          create_list(0,i,1,16), create_list(0,i,1,16))), 
          create_list(i,i,257,384)))),
  PA : subset(PA, lambda([a], 
      not(member(map(abs,a), create_list({i},i,129,129+(bits_to_remove-1)))))),
  output_fcs(
    sconcat("AES Unit Clause assignment for all zero plaintext and key ",
            "and associated ciphertext with the first ", bits_to_remove," bits",
            "left unset in the key. Rounds = ",aes_num_rounds)  , 
    cs2fcs(PA), 
    sconcat("AES_UC_P0_K0_CX_KN",bits_to_remove,".cnf"))
)$
   \endverbatim
   </li>
   <li> An experiment can then be run using the following tools:
   <ul>
    <li> "run_experiment.sh" :
    \verbatim
# Reads and outputs to several files:
# 
# input:
#   experiments
#     This has lines of the form "name command".
# output:
#   $command.watch
#     memory/cpu profile of the given run
#   current_experiment
#      Simply contains a line number for experiments
#      followed by the name of the experiment and the
#      process id. 
#

# Renice current shell so we don't annoy anyone
# Process run from this will then pick this up
renice 19 -p $$;


EXP_START_LINE_NO=0
# Restart the experiment from where we left off
if [ -f current_experiment ]; then
    EXP_START_LINE_NO=`head -n 1 current_experiment | cut -d " " -f 1`
fi

# Go through the experiment file line by line and execute the experiments
EXP_LINE_NO=0
cat experiments | while read EXP; do
    EXP_LINE_NO=`expr $EXP_LINE_NO + 1`;
    if [ $EXP_LINE_NO -ge $EXP_START_LINE_NO ]; then
	# Work out name etc
	EXP_NAME=`echo "$EXP" | cut -d " " -f 1`;
	EXP_COMMAND=`echo "$EXP" | cut -d " " -f 1 --complement`;
	echo "Running" $EXP_NAME;
	# Run command, and work out it's PID
	eval $EXP_COMMAND &
	EXP_PID=$!;
	SESSION_ID=`ps h -o sid --pid $EXP_PID`;
	# Keep track of memory usage etc
	echo "PID CPU VMEM PMEM CPUTIME ETIME CMD" > $EXP_NAME.watch;
	watch --no-title "ps h -s $SESSION_ID -o pid,%cpu,vsz,rss,cputime,etime,comm | grep -v ' \(watch\|sh\|bash\|ps\|grep\) *$' >> $EXP_NAME.watch" > $EXPNAME.log 2>&1 &
	WATCH_PID=$!;
	# Keep track of current experiment and then wait until 
	# it's finished before cleaning up
	echo $EXP_LINE_NO $EXP_NAME $EXP_PID > current_experiment;
	wait $EXP_PID > $EXPNAME.log 2>&1;
	kill -9 $WATCH_PID;
    fi
done
    \endverbatim
    </li>
    <li> "generate_aes_exp.sh":
    \verbatim
# Generates AES experiments for 1 round AES.

RN=1

for k in `seq 0 128`; do
    echo "PICOSAT_"$k "./merge_cnf.sh AES_R${RN}.cnf AES_UC_P0_K0_CX_KN$k.cnf > AES_R${RN}_P0_K0_CX_KN$k.cnf && ./solvers/picosat913 -v AES_R${RN}_P0_K0_CX_KN$k.cnf > AES_R${RN}_P0_K0_CX_KN$k.cnf.result.picosat 2>&1; rm AES_R${RN}_P0_K0_CX_KN$k.cnf" >> experiments;
    echo "MINISAT2_"$k "./merge_cnf.sh AES_R${RN}.cnf AES_UC_P0_K0_CX_KN$k.cnf > AES_R${RN}_P0_K0_CX_KN$k.cnf && ./solvers/minisat2 AES_R${RN}_P0_K0_CX_KN$k.cnf minisat-temp > AES_R${RN}_P0_K0_CX_KN$k.cnf.result.minisat2 2>&1; cat minisat-temp >> AES_R${RN}_P0_K0_CX_KN$k.cnf.result.minisat2;rm AES_R${RN}_P0_K0_CX_KN$k.cnf" >> experiments;
    echo "MARCH_PL_"$k "./merge_cnf.sh AES_R${RN}.cnf AES_UC_P0_K0_CX_KN$k.cnf > AES_R${RN}_P0_K0_CX_KN$k.cnf && ./solvers/march_pl AES_R${RN}_P0_K0_CX_KN$k.cnf > AES_R${RN}_P0_K0_CX_KN$k.cnf.result.march_pl 2>&1;rm AES_R${RN}_P0_K0_CX_KN$k.cnf" >> experiments;
    echo "OKSOLVER_"$k "./merge_cnf.sh AES_R${RN}.cnf AES_UC_P0_K0_CX_KN$k.cnf > AES_R${RN}_P0_K0_CX_KN$k.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F AES_R${RN}_P0_K0_CX_KN$k.cnf > AES_R${RN}_P0_K0_CX_KN$k.cnf.result.OKsolver 2>&1; mv AES_R${RN}_P0_K0_CX_KN$k.cnf.mo AES_R${RN}_P0_K0_CX_KN$k.cnf.mo.OKsolver;mv AES_R${RN}_P0_K0_CX_KN$k.cnf.pa AES_R${RN}_P0_K0_CX_KN$k.cnf.pa.OKsolver;rm AES_R${RN}_P0_K0_CX_KN$k.cnf" >> experiments;
    echo "OKSOLVERM2PP_"$k "./merge_cnf.sh AES_R${RN}.cnf AES_UC_P0_K0_CX_KN$k.cnf > AES_R${RN}_P0_K0_CX_KN$k.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F AES_R${RN}_P0_K0_CX_KN$k.cnf > AES_R${RN}_P0_K0_CX_KN$k.cnf.result.OKsolver-m2pp 2>&1; mv AES_R${RN}_P0_K0_CX_KN$k.cnf_m2pp_*.mo AES_R${RN}_P0_K0_CX_KN$k.cnf.mo.OKsolver-m2pp;mv AES_R${RN}_P0_K0_CX_KN$k.cnf_m2pp_*.pa AES_R${RN}_P0_K0_CX_KN$k.cnf.pa.OKsolver-m2pp;rm AES_R${RN}_P0_K0_CX_KN$k.cnf" >> experiments;
done
    \endverbatim
    </li>
    <li> "merge_cnf.sh":
    \verbatim
#!/bin/bash

# Grab the number of clauses from each
CL1=`grep "^p" $1  | cut -d " " -f "4"`
CL2=`grep "^p" $2  | cut -d " " -f "4"`

NewCL=`expr $CL1 + $CL2`
cat $1 | sed -e "s/p \+\([a-zA-Z]\+\) \+\([0-9]\+\).*$/p \1 \2 $NewCL/"
cat $2 | grep -v "^c" | grep -v "^p"
    \endverbatim
    </li>
   </ul>
   </li>
   <li> To run the experiment, one can then simply run:
   \verbatim
./generate_aes_exp.sh
./run_experiments.sh
   \endverbatim
   </li>
   <li> The above experiment script results in the following solvers each being 
   run on the 1 round AES problems with the number of unknown key bits ranging
   from 0 to 128:
   <ul>
    <li> picosat915 </li>
    <li> minisat2 </li>
    <li> march_pl </li>
    <li> OKsolver_2002-O3-DNDEBUG </li>
    <li> OKsolver_2002-m2pp </li>
   </ul>
   </li>
   <li> The above scripts need to be explained further, polished and moved 
   somewhere more appropriate. </li>
  </ul>


  \todo Breaking 2 round AES using the canonical translation for Sbox and Mul
  <ul>
    <li> A CNF representing a 2 round AES variant (1 round and the final round)
    can be generated in the following way:
    \verbatim
:lisp (ext:set-limit 'ext:heap-size 3000000000)
:lisp (ext:set-limit 'ext:frame-stack 10000)
:lisp (ext:set-limit 'ext:c-stack 200000)
:lisp (ext:set-limit 'ext:lisp-stack 200000)


oklib_load_all()$

aes_sbox_cp : aes_sbox_ts_cp$
aes_mul3_cp : aes_mul3_ts_cp$

aes_mul2_cp : aes_mul2_ts_cp$
aes_mul9_cp : aes_mul9_ts_cp$
aes_mul11_cp : aes_mul11_ts_cp$
aes_mul13_cp : aes_mul13_ts_cp$
aes_mul14_cp : aes_mul14_ts_cp$

aes_num_rounds : 2$

block([oklib_monitoring : true], F : aes_cnf_fcs())$
output_fcs("AES - 2 Rounds - Using canonical translations for Sbox etc.", F, "AES_r2_ts.cnf")$
    \endverbatim
    </li>
  </ul>
  
  \todo Breaking 2 round AES using Sbox and Mul translations with no new 
  variables
  <ul>
   <li> A CNF representing a 2 round AES variant (1 round and the final round)
   can be generated in the following way:
   \verbatim
:lisp (ext:set-limit 'ext:heap-size 3000000000)
:lisp (ext:set-limit 'ext:frame-stack 10000)
:lisp (ext:set-limit 'ext:c-stack 200000)
:lisp (ext:set-limit 'ext:lisp-stack 200000)


oklib_load_all()$
aes_num_rounds : 2$

block([oklib_monitoring : true], CS : aes_cnf_fcs())$
output_fcs("AES - 2 Rounds - No new variables", F, "AES_r2_nnv.cnf")$
   \endverbatim
   </li>
  </ul>

*/
