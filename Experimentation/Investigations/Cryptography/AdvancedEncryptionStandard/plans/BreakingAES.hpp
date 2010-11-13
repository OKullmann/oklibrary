// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/BreakingAES.hpp
  \brief On investigations on breaking AES


  \todo Breaking AES
  <ul>
   <li> What do we mean by "Breaking AES"?
   <ul>
    <li> We take the standard notion of "breaking" a symmetric block cipher, 
    namely that if given a set of plaintext/ciphertext pairs, encrypted using 
    the same key, can we then deduce the key faster than a brute force search? 
    </li>
    <li> The size of the set of plaintext/ciphertext pairs needed, and how
    much faster than brute force one can find the key are then matters of 
    practicality. </li>
    <li> Given a single plaintext/ciphertext pair, there may be multiple keys
    for which the AES algorithm maps that plaintext block to that ciphertext
    block given that key. Therefore, for the purposes of "breaking" AES in
    earnest, one may require providing multiple AES translations, each 
    considering different plaintext/ciphertext pairs, but using the same key
    variables (see "Generate translation that allows multiple 
    plaintext/ciphertext pairs" in 
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp). 
    </li>
    <li> Therefore, experiments detailed within this plans file consider taking
    various AES SAT translations, setting all plaintext variables, all 
    ciphertext variables (to the appropriate values given the plaintext and the
    AES algorithm) and then setting "n" key bits where "n" ranges from 1 to the
    total number of key bits (128 in most cases). </li>
    <li> To "break" AES, the idea is obviously to leave all key variables
    unset. </li>
    <li> Setting some key variables, rather than leaving all key variables
    unset can be done to produce easier to solve SAT problems that can be used
    to determine the behaviour of different solvers as the number of unknown
    key variables increases. </li>
    </li>
   </ul>
   </li>
   <li> For such experiments, when considering experiments where some key bits
   are specified, there is the question of which key bits to specify. To avoid
   situations where knowing some key bits might be more advantageous than 
   others (and so the experimental results not being representative of the 
   AES as a whole), there is the need to randomise which key bits are
   specified. (See "Randomly choosing 'unknown' block bits" in 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp.) 
   </li>
   <li> The main question considered here is:  Given fully specified 128-bit 
    plaintext and the associated ciphertext and a number of rounds r (1-10), 
    how many key bits may be left unspecified before the SAT problem becomes 
    infeasible? </li>
    <li> See 
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/BreakingAES_R1-5.hpp
    for experiments investigating this question for reduced round variants of 
    AES with less than 6 rounds. </li>
    <li> See 
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/BreakingAES_R6-10.hpp
    for experiments investigating this question for reduced round variants of 
    AES with 6 rounds or more. </li>
  </ul>


  \todo General experiment system
  <ul>
   <li> This todo should be moved to a higher level. Perhaps 
   Experimentation/ExperimentSystem/plans/general.hpp ? </li>
   <li> The aim here is to provide a simple template for an "experiment
   directory", where several scripts are provided for running and monitoring
   of a series of experiments on a given machine. </li>
   <li> To create a set of experiments to be run in sequence on a 
   given machine, the following directory structure and set of scripts
   are suggested:
    <ul>
     <li> "run_experiment.sh" - A shell script called to run the experiment 
     (see below). </li>
     <li> "generate_exp.sh" - A shell script called to generate the list
     of experiments/commands to run (outputs to file "experiments" - see below).
     </li>
     <li> "merge_cnf.sh" - A shell script which takes 2 Dimacs filenames as 
     input and outputs to STDOUT a new Dimacs file which has all the clauses 
     from both input files, and an updated p-line to account for the sum of the
     two files. Note: It is assumed the variable set of the first Dimacs file 
     encompasses the second. </li>
     <li> "timeout" - A utility (available as part of the GNU coreutils - 
     http://www.gnu.org/software/coreutils/), which allows a command to be run
     with a given timeout. </li>
    </ul>
   </li>
   <li> Some initial implementations of the mentioned scripts are provided:
   <ul>
    <li> "run_experiment.sh":
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

DEFAULT_TIMEOUT=7200

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
	(./timeout $DEFAULT_TIMEOUT /usr/bin/time -p bash -c "$EXP_COMMAND" 2>$EXP_NAME.time) &
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
    <li> Note here, any individual experiment instance runs for at most 
    DEFAULT_TIMEOUT seconds (so 2 hours in this particular case), and the time
    each experiment takes is measured using the standard unix time utility, as
    some solvers etc will not display correct time information. </li>
    <li> An example of a "generate_exp.sh" is provided in "Generating 
    experiments". 
     <ul>
      <li>This file should simply be a script written for each
      set of experiments, which generates a file "experiments". </li>
      <li> Each line of "experiments" must be made up of 2 parts:
       <ol>
        <li> An experiment name, which can be anything but must contain no 
        spaces and should uniquely identify each individual experiment in
	the set. </li>
        <li> A shell command, which is separated from the experiment name by
	one or more spaces, and takes up the remainder of the line. </li>
       </ol>
      </li>
      <li> An example line in "experiments" is:
      \verbatim
EXP_1 ./solvers/picosat913 Example.cnf
      \endverbatim
      </li>
      <li> The "run_experiment.sh" script will then read this line, use
      "EXP_1" to identify the experiment (this is used in filenames etc for
      any monitoring output), and run the given command. </li>
     </ul>
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
   <li> Upon calling "run_experiment.sh", the experiment system then generates
   several files:
    <ul>
     <li> EXP_NAME.watch - A file for the given experiment (in the set of
     experiments) containing a snapshot of the state of the experiment
     every 2 seconds, with the following columns (separated by spaces,
     and containing a header row):
     <ol>
      <li> PID of the running processes in the experiment </li>
      <li> CPU percentage usage of the running processes </li>
      <li> Virtual memory usage in bytes </li>
      <li> Physical memory usage in bytes </li>
      <li> Total (actual) CPU time in HH:MM:SS format of the running 
      processes</li>
      <li> Total (elapsed) time in HH:MM:SS format of the running 
      processes </li>
     </ol>
     For example:
     \verbatim
PID CPU VMEM PMEM CPUTIME ETIME CMD
30451  0.0   5052   696 00:00:00       00:19 cat
30941  0.0  14536  1568 00:00:00       00:00 merge_cnf.sh
30950  0.0   5052   696 00:00:00       00:00 cat
30951  0.0   5164   896 00:00:00       00:00 sed
30451  0.0   5052   696 00:00:00       00:21 cat
30959  0.0   4348  3824 00:00:00       00:00 picosat913
     \endverbatim
     </li>
     <li> EXP_NAME.time - The output of the unix "time" utility run 
     with the command as the argument. </li>
     <li> current_experiment - A file containing 3 fields separated by spaces:
      <ol>
       <li> The line number of the currently running experiment in the 
       "experiments" file generated by "generate_exp.sh". </li>
       <li> The name of the currently running experiment from "experiments", 
       that is, the first argument on the above line number. </li>
       <li> The process ID of the subshell running the listed experiment.
       </li>
      </ol>
     </li>
     <li> Any files generated by the command for EXP_NAME given in the 
     "experiments" file. These (depending on the experiment/command) will
     in most cases include the output from a particular solver etc. </li>
    </ul>
   </li>
   <li> Note that the above mentioned system does not have any particular 
   functionality available for specifying PRE and POST activities for 
   experiments. </li>
   <li> For example, when running a set of experiments which involves running a
   SAT solver on a variety of CNFs, one would do the following:
    <ol>
     <li> Generate the CNF based on a particular set of parameters. </li>
     <li> Run the solver on the generated CNF. </li>
     <li> Clean up the directory afterwards, for example, deleting the generated
     CNF to save space. </li>
    </ol>
   However, such PRE and POST conditions can simply be represented themselves as
   experiments in a given set of experiments, annotating the names of such 
   experiments to illustrate this fact. For example:
   \verbatim
EXP1_PRE ./generate_example_cnf.sh
EXP1 ./solvers/minisat2 Example.cnf
EXP1_POST rm Example.cnf
   \endverbatim
   This has the added bonus that such PRE and POST activities are timed and 
   monitored separately, in addition to the main focus of the experiment.
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
   machines may be shut down or encounter hardware faults, the existence of
   such a "global_experiments" file for monitoring, also allows automated
   backup using the following script (assumed to be in the same folder as 
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
   <li> The above scripts need to be explained further, polished and moved 
   somewhere more appropriate. </li>
  </ul>

  
  \todo Generating experiments
  <ul>
   <li> For simple experiments we can simply use trivial plaintext and key 
   blocks such as all zero plaintext and key blocks, and then consider the 
   ciphertext which is generated by AES for such blocks. </li>
   <li> To experiment properly however, one needs truly random plaintext and 
   key blocks, randomly distributed in a well-defined and understood manner,
   and such randomisation much be reproducible (so we must store seeds etc
   and the algorithm must be well-defined). </li>
   <li> See "Generating randomised AES parameters" in 
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/AdvancedEncryptionStandard.hpp
   for random block generation. </li>
   <li> See also "Randomly choosing 'unknown' block bits" in 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp 
   for random choosing of "unknown" block bits. </li>
   <li> However, for initial experiments, running experiments with a fixed
   plaintext and key block (for instance all 0 values for each) is reasonable.
   If patterns occur, then these can then investigated using a variety of
   randomly chosen parameters. </li>
   <li> To generate experiments which consider an AES variant with a particular
   number of rounds, and then specifying all plaintext and key bits, while
   varying the number of unknown key bits, one may use the following 
   "generate_exp.sh" script (see "General experiment system"): 
   \verbatim
# Generates AES experiments for one round AES.

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
   <li> Additionally, any such experiment system will need the following 
   files:
    <ul>
     <li> "AES_R$R.cnf" - The AES translation using new variables restricted
     to $R rounds. </li>
     <li> "AES_UC_P0_K0_CX_KN$N.cnf" - The generated unit clauses specifying the
     plaintext, ciphertext and key bits (variables) to be set, where $N is the
     number of key bits being specified in the problem. </li>
    </ul>
   </li>
   <li> Generating "AES_R$N.cnf" (where $N=1 in this example):
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
plaintext : "00000000000000000000000000000000"$
key : "00000000000000000000000000000000"$
for bits_to_remove : 0 thru 128 do block([PA],
  PA : map(set,
    union(
      aes_hex2pa(plaintext, create_list(i,i,1,128)),
      aes_hex2pa(key, create_list(i,i,129,256)), 
      aes_hex2pa(il2hex(
        aes_encrypt_l(
          hex2il(plaintext), hex2il(key))), 
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
   <li> The above experiment script results in the following solvers each being
   run on the $R round AES problems with the number of unknown key bits ranging
   from 0 to 128:
   <ul>
    <li> picosat913 </li>
    <li> minisat2 </li>
    <li> march_pl </li>
    <li> OKsolver_2002-O3-DNDEBUG </li>
    <li> OKsolver_2002-m2pp </li>
   </ul>
   </li>
  </ul>


  \todo Computing AES ciphertext given full 128-bit key and plaintext
  <ul>
   <li> Question here is: Given the current translation, how well can a SAT
   solver perform a simple AES computation? </li>
   <li> It should be that through unit clause propagation etc, the SAT solver
   should be very quick? </li>
   <li> See 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/EncryptionDecryption.hpp . </li>
  </ul>


  \todo Determining a "good" local search algorithm to use on AES instances
  <ul>
   <li> Question: what is a good algorithm to use, considering the algorithms
   tested by "run_ubcsat" (see 
   Experimentation/ExperimentSystem/ControllingLocalSearch/Evaluation.R) </li>
   <li> Presumably local search will not do well on AES instances compared to 
   DPLL/CDCL solvers, as often local search solvers perform
   poorly on industrial and crafted instances, which the AES translations
   are likely similar to, considering the large number of new variables 
   involved. </li>
   <li> What sort of metrics to use to determine a good algorithm once the
   experiment has been run?
   <ul>
    <li> Sorting first by the average number of falsified clauses and then
    by the number of steps seems reasonable, as we wish to minimise the number
    of falsified clauses as much as possible. </li>
   </ul>
   </li>
  </ul>

*/
