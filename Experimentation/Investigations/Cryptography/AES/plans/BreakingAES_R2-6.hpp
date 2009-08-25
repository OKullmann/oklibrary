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
   <li> Additionally, the structure of the above system allows one to monitor
   the behaviour of the experiments, using the following monitor script:
   \verbatim
# Monitor experiments running on several machines

echo "MACHINE" "EXP" "NAME" "RUNNING_FOR";

cat global_experiments | while read experiment; do
    MACHINE=`echo $experiment | cut -d " " -f 1`;
    EXP_DIR=`echo $experiment | cut -d " " -f 2`;
    CURRENT_EXP=`0</dev/null ssh $MACHINE -C "head -n 1 $EXP_DIR/current_experiment"`;
    CUR_EXP_PID=`echo $CURRENT_EXP | cut -d " " -f 3`;
    CUR_EXP_NAME=`echo "$CURRENT_EXP" | cut -d " " -f 2`;
    CUR_EXP_START_TIME=`0</dev/null ssh $MACHINE -C "ps h -o etime --pid $CUR_EXP_PID"`;
    echo $MACHINE $EXP_DIR $CUR_EXP_NAME $CUR_EXP_START_TIME
done
   \endverbatim
   where this script expects a file "global_experiments" in the same directory,
   where each line has a hostname following by a directory name, and it will 
   simply connect to each host and print the machine, the directory, the 
   currently running experiment on that machine (given the names used in the 
   file "experiments" for that set of experiments) and how long that experiment
   has been running for in total. </li>
   <li> Note, this monitoring script assumes that any specific details such as
   usernames, passwords etc have been setup as keys or in the users
   .ssh/config file , otherwise one would need to edit the script. </li>
   <li> An example of this global experiments file is:
   \verbatim
cspasiphae.swan.ac.uk AES_1_Round
cssinope.swan.ac.uk AES_2_Round
cselara.swan.ac.uk AES_3_Round
csananke.swan.ac.uk AES_4_Round
csmiranda.swan.ac.uk AES_5_Round
csio.swan.ac.uk AES_6_Round
cshimalia.swan.ac.uk AES_7_Round
cslysithea.swan.ac.uk AES_8_Round
csiapetus.swan.ac.uk AES_9_Round
csamalthea.swan.ac.uk AES_10_Round
cspasiphae.swan.ac.uk Ramsey_2_5_5
cssinope.swan.ac.uk Ramsey_2_4_4
cselara.swan.ac.uk Ramsey_2_3_10
   \endverbatim
   </li>
   <li> Additionally, as results will be pooled across many machines, and these
   machines may be shut down or encounter hardware faults, the existence of such
   a "global_experiments" file for monitoring, also allows automated backup 
   using the following script (assumed to be in the same folder as 
   "global_experiments"):
   \verbatim
cat global_experiments | while read exp; do
    MACHINE=`echo $exp | cut -d " " -f 1`;
    EXP_DIR=`echo $exp | cut -d " " -f 2`;
    rsync -r --copy-links $MACHINE:$EXP_DIR .; 
done
   \endverbatim
   This script simply copies each of the experiment directories listed in
   "global_experiments", across the network, into the current directory,
   ensuring that any symlinks are resolved to their respective files before
   copying.
   </li>
   <li> The above experiment script results in the following solvers each being 
   run on the 1 round AES problems with the number of unknown key bits ranging
   from 0 to 128:
   <ul>
    <li> picosat913 </li>
    <li> minisat2 </li>
    <li> march_pl </li>
    <li> OKsolver_2002-O3-DNDEBUG </li>
    <li> OKsolver_2002-m2pp </li>
   </ul>
   </li>
   <li> The above scripts need to be explained further, polished and moved 
   somewhere more appropriate. </li>
   <li> minisat2 seems to perform reasonably well on such problems:
    <ul>
     <li> With 10 key bits unknown: 
     \verbatim
cs360678@cspasiphae:~> cat AES_1_Round/AES_R1_P0_K0_CX_KN10.cnf.result.minisat2 | grep -v ".\{1000,\}"
This is MiniSat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  32176                                                |
|  Number of clauses:    510482                                               |
|  Parsing time:         0.40         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |       0        0        0 |        0        0    nan |  0.000 % |
===============================================================================
restarts              : 1
conflicts             : 0              (0 /sec)
decisions             : 1              (0.00 % random) (1 /sec)
propagations          : 32176          (19666 /sec)
conflict literals     : 0              ( nan % deleted)
Memory used           : 33.17 MB
CPU time              : 1.6361 s

SATISFIABLE
SAT
     \endverbatim
     </li>
     <li> With 50 key bits unknown:
     \verbatim
cs360678@cspasiphae:~> cat AES_1_Round/AES_R1_P0_K0_CX_KN50.cnf.result.minisat2 | grep -v ".\{1000,\}"
This is MiniSat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  32176                                                |
|  Number of clauses:    510442                                               |
|  Parsing time:         0.41         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |   22443   367815  1230363 |   122605        0    nan |  0.000 % |
===============================================================================
restarts              : 1
conflicts             : 9              (2 /sec)
decisions             : 648            (1.85 % random) (143 /sec)
propagations          : 75870          (16755 /sec)
conflict literals     : 1075           (11.89 % deleted)
Memory used           : 37.98 MB
CPU time              : 4.52828 s

SATISFIABLE
SAT
     \endverbatim
     </li>
     <li> With 80 key bits unknown:
     \verbatim
cs360678@cspasiphae:~> cat AES_1_Round/AES_R1_P0_K0_CX_KN80.cnf.result.minisat2 | grep -v ".\{1000,\}"
This is MiniSat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  32176                                                |
|  Number of clauses:    510412                                               |
|  Parsing time:         0.40         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |   25170   411810  1378018 |   137270        0    nan |  0.000 % |
|       101 |   25170   411810  1378018 |   150997      101    158 | 19.567 % |
|       253 |   25170   411810  1378018 |   166096      253    302 | 19.567 % |
|       479 |   25170   411810  1378018 |   182706      479    458 | 19.567 % |
|       817 |   25170   411810  1378018 |   200977      817    486 | 19.567 % |
|      1323 |   25170   411810  1378018 |   221074     1323    462 | 19.567 % |
|      2083 |   25170   411810  1378018 |   243182     2083    485 | 19.567 % |
|      3223 |   25170   411810  1378018 |   267500     3223    449 | 19.567 % |
|      4932 |   25170   411810  1378018 |   294250     4932    459 | 19.567 % |
|      7506 |   25170   411810  1378018 |   323675     7506    463 | 19.567 % |
|     11351 |   25170   411810  1378018 |   356043    11351    398 | 19.567 % |
|     17118 |   25170   411810  1378018 |   391647    17118    388 | 19.567 % |
|     25769 |   25170   411810  1378018 |   430812    25769    362 | 19.567 % |
|     38744 |   25170   411810  1378018 |   473893    38744    339 | 19.567 % |
|     58206 |   25170   411810  1378018 |   521282    58206    344 | 19.567 % |
===============================================================================
restarts              : 15
conflicts             : 64225          (214 /sec)
decisions             : 139765         (1.39 % random) (467 /sec)
propagations          : 151211470      (504801 /sec)
conflict literals     : 21865300       (54.62 % deleted)
Memory used           : 118.31 MB
CPU time              : 299.547 s

SATISFIABLE
SAT
     \endverbatim
     </li>
    </ul>
   </li>
   <li> picosat seems to perform extremely well in this instance:
   \verbatim
c PicoSAT SAT Solver Version 913
c Copyright (c) 2006 - 2009 Armin Biere JKU Linz
c gcc -Wall -Wextra -m32 -static -DNDEBUG -O3 -fomit-frame-pointer -finline-limit=1000000
c
c parsing AES_R1_P0_K0_CX_KN80.cnf
c parsed header 'p cnf 32176 510412'
c initialized 32176 variables
c found 510412 non trivial clauses
c
c random number generator seed 0
c 
c  seconds     variables   original    learned     agility
c         level        used      conflicts    limit          MB
c 
c s   1.8   0.0 25880  19.6 27470     0     0     0   0.0   8.5 
c r   1.8   0.0 25880  19.6 27470     0     0     0   0.0   8.5 
c 
c initial reduction limit 17219 clauses
c 
c 1   1.8   8.5 25880  19.6 27470     0     0 17219   0.0   8.6 
c 
c  seconds     variables   original    learned     agility
c         level        used      conflicts    limit          MB
c 
s SATISFIABLE
c
c 0 iterations
c 0 restarts
c 0 failed literals
c 0 conflicts
c 16 decisions
c 6296 fixed variables
c 6012 learned literals
c 0.0% deleted literals
c 10033828 propagations
c 19.6% variables used
c 1.8 seconds in library
c 5.4 megaprops/second
c 1 simplifications
c 0 reductions
c 0.5 MB recycled
c 9.0 MB maximally allocated
c 1.8 seconds total run time
   \endverbatim
   even for the full AES (tested in a seperate experiment):
   \verbatim
[19:42:51 - aeternus] AES_10_Round$ picosat913 AES_R10_P0_K0_CX_KN128.cnf  | grep -v "^v"
c PicoSAT SAT Solver Version 913
c Copyright (c) 2006 - 2009 Armin Biere JKU Linz
c gcc -Wall -Wextra -m32 -static -DNDEBUG -O3 -fomit-frame-pointer -finline-limit=1000000
c
c parsing AES_R10_P0_K0_CX_KN128.cnf
c parsed header 'p cnf 291168 4673464'
c initialized 291168 variables
c found 4673464 non trivial clauses
c
c random number generator seed 0
c 
c  seconds     variables   original    learned     agility
c         level        used      conflicts    limit          MB
c 
c s   3.0   0.0 290e3   0.2 311e3     0     0     0   0.0  80.8 
c 
c initial reduction limit 156020 clauses
c 
c 1   3.0  64.5 290e3   0.2 311e3     0     0 156e3   0.0  82.8 
c 
c  seconds     variables   original    learned     agility
c         level        used      conflicts    limit          MB
c 
s SATISFIABLE
c
c 0 iterations
c 0 restarts
c 0 failed literals
c 0 conflicts
c 128 decisions
c 336 fixed variables
c 256 learned literals
c 0.0% deleted literals
c 10291832 propagations
c 0.2% variables used
c 3.1 seconds in library
c 3.4 megaprops/second
c 1 simplifications
c 0 reductions
c 0.0 MB recycled
c 82.8 MB maximally allocated
c 3.1 seconds total run time
   \endverbatim
   </li>
   <li> However, in the case of picosat913, it seems that the heuristics are 
   such that it happens to try an all zero key first. Trying a more random
   key, generated by:
   \verbatim
aes_num_rounds : 1$
for bits_to_remove : 0 thru 128 do block([PA],
  PA : map(set,
    union(
      aes_hex2pa("00000000000000000000000000000000", create_list(i,i,1,128)),
      aes_hex2pa("123456789ABCDEF0123456789ABCDEF0", create_list(i,i,129,256)), 
      aes_hex2pa(
        aes_encrypt_hex(
          "00000000000000000000000000000000", 
	  "123456789ABCDEF0123456789ABCDEF0"), 
          create_list(i,i,257,384)))),
  PA : subset(PA, lambda([a], 
      not(member(map(abs,a), create_list({i},i,129,129+(bits_to_remove-1)))))),
  output_fcs(
    sconcat("AES Unit Clause assignment for all zero plaintext and key ",
            "and associated ciphertext with the first ", bits_to_remove," bits",
            "left unset in the key. Rounds = ",aes_num_rounds)  , 
    cs2fcs(PA), 
    sconcat("AES_UC_P0_KX_CX_KN",bits_to_remove,".cnf"))
)$
   \endverbatim
   and then
   \verbatim
./merge_cnf.sh AES_R1.cnf AES_UC_P0_KX_CX_KN75.cnf > AES_R1_P0_KX_CX_KN75.cnf
   \endverbatim
   results in far more reasonable results:
   \verbatim
$ picosat913 AES_R1_P0_KX_CX_KN75.cnf | grep -v "^v"
c PicoSAT SAT Solver Version 913
c Copyright (c) 2006 - 2009 Armin Biere JKU Linz
c gcc -Wall -Wextra -m32 -static -DNDEBUG -O3 -fomit-frame-pointer -finline-limit=1000000
c
c parsing AES_R1_P0_KX_CX_KN75.cnf
c parsed header 'p cnf 32176 510417'
c initialized 32176 variables
c found 510417 non trivial clauses
c
c random number generator seed 0
c 
c  seconds     variables   original    learned     agility
c         level        used      conflicts    limit          MB
c 
c s   2.6   0.0 25121  22.1 27202   496     0     0   0.0   8.6 
c 
c initial reduction limit 17219 clauses
c 
c +   2.7  73.1 25121  24.0 27202   596    99 18940   5.2   8.7 
c R   2.7  73.1 25121  24.0 27202   599   102 18940   5.3   8.7 
c +   2.8 233.7 25121  28.4 27202   746   249 20834   4.3   9.0 
c R   2.9 409.1 25121  28.4 27202   901   404 20834   4.1   9.2 
c +   3.1 397.9 25121  30.0 27202   971   474 22917   4.3   9.3 
c +   3.6 324.6 25121  30.3 27202  1308   811 25208   4.1   9.8 
c R   4.2 407.2 25121  30.3 27202  1710  1212 25208   3.8  10.4 
c +   4.4 394.9 25121  34.8 27202  1813  1314 27728   2.3  10.5 
c +   5.4 393.9 25121  34.8 27202  2570  2071 30500   2.0  11.9 
c +   7.3 377.9 25121  34.8 27202  3705  3205 33550   5.5  13.6 
c R   7.4 378.0 25121  34.8 27202  3727  3227 33550   3.9  13.6 
c +   9.9 364.0 25121  45.9 27202  5407  4905 36905   4.2  15.7 
c +  12.9 358.9 25121  45.9 27202  7960  7456 40595   6.1  19.0 
c R  13.7 355.2 25121  45.9 27202  8537  8033 40595   4.6  20.0 
c s  17.0 355.8 25120  45.9 27202 10531 10018 40595   3.6  23.0 
c R  17.2 354.1 25120  45.9 27202 10632 10118 40595   5.4  23.2 
c R  17.9 352.1 25120  46.7 27202 10935 10421 40595   4.1  23.5 
c R  19.4 347.2 25120  46.7 27202 11741 11227 40595   3.7  24.6 
c +  19.5 346.4 25120  46.7 27202 11789 11275 44654   5.3  24.7 
c s  19.7 345.9 25119  46.7 27202 12017 11494 44654   3.4  25.0 
c 
c  seconds     variables   original    learned     agility
c         level        used      conflicts    limit          MB
c 
c R  19.8 345.2 25119  46.7 27202 12117 11594 44654   5.9  25.1 
c R  20.5 343.3 25119  46.7 27202 12420 11897 44654   5.4  25.5 
c s  21.1 339.5 25118  46.7 27202 12894 12361 44654   4.0  26.1 
c R  21.2 338.6 25118  46.7 27202 12994 12461 44654   2.1  26.3 
c R  21.6 337.3 25118  46.7 27202 13297 12762 44654   4.1  26.6 
c R  21.9 336.2 25117  46.7 27202 13425 12888 44654   3.8  26.8 
c R  22.4 331.5 25117  47.5 27202 13735 13193 44654   4.8  27.1 
c s  22.5 331.0 25117  47.5 27202 13744 13198 44654   4.0  27.1 
c R  23.2 333.4 25116  47.5 27202 14331 13784 44654   5.0  28.0 
c R  23.6 336.9 25116  47.5 27202 14634 14087 44654   3.7  28.4 
c s  24.0 335.7 25116  47.5 27202 14920 14364 44654   1.6  28.8 
c R  24.7 336.0 25116  47.5 27202 15447 14891 44654   3.5  29.3 
c R  28.2 328.9 25116  48.3 27202 17455 16899 44654   3.6  31.6 
c +  28.3 328.5 25116  48.3 27202 17532 16976 49119   4.1  31.6 
c R  33.9 314.9 25116  48.3 27202 22281 21723 49119   5.5  37.1 
c s  34.0 313.6 25115  48.3 27202 22390 21793 49119   2.8  37.1 
c R  34.1 313.2 25115  48.3 27202 22493 21896 49119   4.4  37.3 
c R  34.5 312.6 25115  48.3 27202 22802 22205 49119   3.6  37.6 
c R  35.1 312.6 25115  48.3 27202 23608 23011 49119   3.3  38.5 
c R  37.7 306.3 25115  48.3 27202 25619 25022 49119   4.0  40.6 
c +  38.7 303.0 25115  48.3 27202 26146 25547 54030   3.4  41.1 
c s  39.9 299.4 25114  48.3 27202 27031 26417 54030   4.3  42.1 
c 
c  seconds     variables   original    learned     agility
c         level        used      conflicts    limit          MB
c 
c R  40.0 299.6 25114  48.3 27202 27134 26520 54030   3.1  42.2 
c R  40.6 298.5 25114  48.3 27202 27434 26819 54030   4.3  42.5 
c R  41.7 295.1 25114  48.3 27202 28235 27620 54030   3.6  43.2 
c R  44.7 291.4 25114  48.3 27202 30246 29628 54030   5.7  45.2 
c 1  48.8 286.1 25114  48.4 27202 32544 31925 54030   4.4  47.1 
c 
c  seconds     variables   original    learned     agility
c         level        used      conflicts    limit          MB
c 
s SATISFIABLE
c
c 7 iterations
c 150 restarts
c 496 failed literals
c 32544 conflicts
c 241499 decisions
c 7062 fixed variables
c 9887444 learned literals
c 15.3% deleted literals
c 77729689 propagations
c 48.4% variables used
c 48.8 seconds in library
c 1.6 megaprops/second
c 8 simplifications
c 0 reductions
c 0.6 MB recycled
c 47.1 MB maximally allocated
c 48.8 seconds total run time
   \endverbatim
   </li>
   <li> OKsolver seems to perform well 
   <ul>
    <li> 
    \verbatim
cs360678@cspasiphae:~> cat AES_1_Round/AES_R1_P0_K0_CX_KN72.cnf.result.OKsolver | grep -v "^v"

Name of file, monitoring depth and number of monitoring nodes:
 AES_R1_P0_K0_CX_KN72.cnf,   20,    1048576
level: nodes processed, average nodes, predicted total nodes, time for monitoring node, average time, predicted remaining time, single nodes, autarkies, depth


s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=256 initial_number_of_variables=32176 initial_number_of_clauses=510420 initial_number_of_literal_occurrences=1500064 running_time(s)=8.7 number_of_nodes=1 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=255 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=0 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=7136 reduced_number_of_clauses=110841 reduced_number_of_literal_occurrences=326240 number_of_1-autarkies=0 number_of_initial_unit-eliminations=7136 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=374080 file_name=AES_R1_P0_K0_CX_KN72.cnf
    \endverbatim
    </li>
    <li> However, when the number of unknown key bits reaches 73 and above, 
    OKsolver seems to have considerable trouble:
    \verbatim
cs360678@cspasiphae:~> cat AES_1_Round/AES_R1_P0_K0_CX_KN73.cnf.result.OKsolver | grep -v "^v"

Name of file, monitoring depth and number of monitoring nodes:
 AES_R1_P0_K0_CX_KN73.cnf,   20,    1048576
level: nodes processed, average nodes, predicted total nodes, time for monitoring node, average time, predicted remaining time, single nodes, autarkies, depth

524288:      2      0.00  4.00E+00   261.73s     0.00s     0y   0d  0h  4m 22s     0     0    1

s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=256 initial_number_of_variables=32176 initial_number_of_clauses=510419 initial_number_of_literal_occurrences=1500063 running_time(s)=269.7 number_of_nodes=3 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=580 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=6571 reduced_number_of_clauses=101979 reduced_number_of_literal_occurrences=300149 number_of_1-autarkies=0 number_of_initial_unit-eliminations=6571 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=382261 file_name=AES_R1_P0_K0_CX_KN73.cnf
    \endverbatim
    </li>
    <li>
    \verbatim
cs360678@cspasiphae:~> cat AES_1_Round/AES_R1_P0_K0_CX_KN75.cnf.result.OKsolver | grep -v "^v"

** Monitoring removed for brevity (will be added as a summary).

s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=256 initial_number_of_variables=32176 initial_number_of_clauses=510417 initial_number_of_literal_occurrences=1500061 running_time(s)=17567.7 number_of_nodes=516 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=213261 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=13 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=6559 reduced_number_of_clauses=101889 reduced_number_of_literal_occurrences=299893 number_of_1-autarkies=0 number_of_initial_unit-eliminations=6559 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=382334 file_name=AES_R1_P0_K0_CX_KN75.cnf
    \endverbatim
    </li>
   </ul>
   </li>
   <li> Preprocessing seems to help OKsolver considerably here:
   \verbatim
cs360678@cspasiphae:~> cat AES_1_Round/AES_R1_P0_K0_CX_KN75.cnf.result.OKsolver-m2pp | grep -v "^v"
s UNKNOWN
c sat_status=2 initial_maximal_clause_length=256 initial_number_of_variables=32176 initial_number_of_clauses=510417 initial_number_of_literal_occurrences=1500061 running_time(s)=0.6 number_of_nodes=0 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=0 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=0 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=6559 reduced_number_of_clauses=101889 reduced_number_of_literal_occurrences=299893 number_of_1-autarkies=0 number_of_initial_unit-eliminations=6559 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=AES_R1_P0_K0_CX_KN75.cnf
This is MiniSat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  32176                                                |
|  Number of clauses:    510417                                               |
|  Parsing time:         0.40         s                                       |
==============================[ Writing DIMACS ]===============================
Wrote 407544 clauses...
restarts              : 0
conflicts             : 0              (0 /sec)
decisions             : 0              ( nan % random) (0 /sec)
propagations          : 6559           (1290 /sec)
conflict literals     : 0              ( nan % deleted)
Memory used           : 38.61 MB
CPU time              : 5.08432 s

Name of file, monitoring depth and number of monitoring nodes:
 AES_R1_P0_K0_CX_KN75.cnf_m2pp_17180,   20,    1048576
level: nodes processed, average nodes, predicted total nodes, time for monitoring node, average time, predicted remaining time, single nodes, autarkies, depth


s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=129 initial_number_of_variables=24912 initial_number_of_clauses=407544 initial_number_of_literal_occurrences=1363708 running_time(s)=634.9 number_of_nodes=3 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=1480 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=2 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=0 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=380039 file_name=AES_R1_P0_K0_CX_KN75.cnf_m2pp_17180
   \endverbatim
   </li>
   <li> march_pl also doesn't seem to work particularly well on such problems,
   although better than OKsolver:
   <ul>
    <li> For the number of unknown key bits being 10, march_pl takes under a 
    second. </li>
    <li> For the number of unknown key bits being 50, march_pl takes around 
    25 seconds. </li>
    <li> For the number of unknown key bits being 75, march_pl takes nearly 
    3 hours (2:53:31). </li>
    <li> Timings are given from the process monitoring which is to within 2 
    seconds. </li>
    <li> The experiments should be paused, and restarted with an updated
    "run_experiments.sh" including use of the "time" command to give accurate
    readings, especially for solvers such as march with broken timers (the timer
    overflows). </li>
   </ul>
   </li>
   <li> OKsolver* and march_pl seem to considerably slow the experiment in this
   case and so it may be reasonable to remove them for larger numbers of unknown
   key bits? </li>
   <li> Monitoring scripts in the R system should be written to read the output
   of each solver, so such data can be easily amalgamated and then properly
   analysed. </li>
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
