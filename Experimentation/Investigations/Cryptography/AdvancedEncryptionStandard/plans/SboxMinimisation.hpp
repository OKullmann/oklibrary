// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SboxMinimisation.hpp
  \brief On investigations into the AES Sbox


  \todo Connections
  <ul>
   <li> See Investigations/BooleanFunctions/plans/Permutations.hpp for
   general investigations on permutations of {0,1}^n. </li>
  </ul>


  \todo Minimum size small scale AES operations
  <ul>
   <li> See ss_sbox_fullcnf_fcs() in
   OKlib/ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/testobjects/SboxAnalysis.mac . </li>
   <li> In the small scale AES, the size of the field and the size of the 
   block may vary and therefore, rather than simplying having the standard 8 
   bit "byte" field in AES, the "word" (a generalisation of byte) field
   may now be of arbitrary size. </li>
   <li> In reality, in the literature (see [Small Scale Variants of the AES; 
   Cid, Murphy and Robshaw]), only one field of size 4 is considered. </li>
   <li> So we are considering the following minisation problems
   (given as maxima code to generate their minimum CNF representations) :
   \verbatim
min_sbox_cnfs : all_minequiv_bvs_cs(ss_sbox_fullcnf_fcs(2,4,ss_polynomial_2_4)[2]);
   \endverbatim
   </li>
  </ul>


  \todo Minimisation of the Sbox
  <ul>
   <li> See "Minimisation" in 
   OKlib/Satisfiability/FiniteFunctions/plans/general.hpp . </li> 
   <li> R QCA packages 
   <ul>
    <li> We can use the QCA package, given in 
    Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp to compute
    the minimum sized CNF or DNF clause-set representation. </li>
    <li> This should be possible using the following code:
    \verbatim
######## In Maxima #######
generate_full_aes_sbox_tt() :=  
  map(
     lambda([ce],
       append(
         int2polyadic_padd(ce[1],2,8),
         int2polyadic_padd(ce[2],2,8),
         if rijn_lookup_sbox(ce[1]) = ce[2] then [1] else [0]))
     ,cartesian_product(setmn(0,255),setmn(0,255)))$

with_stdout("Sbox.tt", block(
  apply(print, endcons("O",create_list(i,i,1,16))),
  for tt_line in generate_full_aes_sbox_tt() do
    apply(print,tt_line)
  ))$

######## In R ###########

oklib_load_all()
library(QCA)

sbox_tt = read.table("Sbox.tt",header=TRUE)
eqmcc(sbox_tt, outcome="O", expl.0=TRUE)
   \endverbatim
   although currently there are issues with memory (see "Minimisation in
   OKlib/Satisfiability/FiniteFunctions/plans/general.hpp). </li>  
  </ul>
  </li>
  <li> Espresso-ab (see Logic "synthesis" in
  Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp)
  <ul>
   <li> Espresso-ab takes as input a truth table in PLA format. </li>
   <li> We can generate a truth table in PLA format for the Sbox in the
   following way:
   \verbatim
generate_full_aes_sbox_tt() :=  
  map(
     lambda([ce],
       append(
         int2polyadic_padd(ce[1],2,8),
         int2polyadic_padd(ce[2],2,8),
         if rijn_lookup_sbox_nat(ce[1]) = ce[2] then [1] else [0]))
     ,cartesian_product(setmn(0,255),setmn(0,255)))$

with_stdout("Sbox.pla", block(
  print(".i 16"),
  print(".o 1"),
  for tt_line in generate_full_aes_sbox_tt() do
    print(apply(sconcat,rest(tt_line,-1)),1-last(tt_line))
  ))$
   \endverbatim
   where the PLA file will be called "Sbox.pla", and will be represented
   as a DNF representing the negation of the Sbox (as by default Espresso
   minimises DNF formulas.
   </li>
   <li> Running espresso-ab with the standard options yielded a minimal DNF 
   representing the negation of the Sbox with 354 clauses. This is much 
   smaller than the Sbox found in XXX with 529 clauses. </li>
   <li> Espresso-ab by default uses heuristical methods however, and so it
   is not clear how close to the minimum size clause-set 354 clauses is. 
   </li>
   <li> Espresso-ab does however have additional options to allow exact
   minimisation including "exact" and "signature" options representing
   algorithms discussed in [BraytonMcGeerSanghaviVincentelli 1993], which can
   be run by using the "-Dexact" and "-Dsignature" options. </li>
   <li> Using the "exact" and "signature" algorithms, espresso-ab runs out of 
   memory on an 8GB machine (it is killed by the linux OOM killer). </li>
  </ul>
  </li>
  <li> Scherzo (see "Logic synthesis" in
  Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp)
  <ul>
   <li> Scherzo uses implicit BDD representations for sets of prime implicates
   and therefore offers the opportunity to minimise the Sbox without running 
   out of memory. </li>
   <li> See "Logic synthesis" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp for 
   issues relating to the building and running of Scherzo. 
   </li>
  </ul>
  </li>
  <li> Using prime generation and subsumption hypergraph generators in the 
  OKlibrary (see OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp
  and Structures/Sets/SetAlgorithms/plans/SubsumptionHypergraph.hpp)
  <ol>
   <li> One can generate the necessary files for the Sbox using the following
   in Maxima, using the scripts ("add_dimacs_p_line.sh" and "merge_cnfs.sh")
   and then performing some commands in a shell.
   \verbatim
/* Maxima */
output_rijnsbox_fullcnf_stdname()$
   \endverbatim
   and
   \verbatim
#!/bin/bash 
#
# add_dimacs_p_line.sh
# 
# Takes the path to a dimacs file without a p line
# and updates the file to include a valid p line.
#

OKSOLVER_NUM_VARS_LINE='initial_number_of_variables'
OKSOLVER_NUM_CLAUSES_LINE='initial_number_of_clauses'

if [[ $# -eq 1 ]]; then
    OKSOLVER_OUTPUT=`OKsolver_2002-O3-DNDEBUG -P $1 | grep "\(${OKSOLVER_NUM_VARS_LINE}\|${OKSOLVER_NUM_CLAUSES_LINE}\)"`;
    NUM_VARS=`echo "${OKSOLVER_OUTPUT}" | grep "${OKSOLVER_NUM_VARS_LINE}" | awk '{ print \$NF}'`;
    NUM_CLAUSES=`echo "${OKSOLVER_OUTPUT}" | grep "${OKSOLVER_NUM_CLAUSES_LINE}" | awk '{ print \$NF}'`;
    sed -i "1s/^/p cnf ${NUM_VARS} ${NUM_CLAUSES}\n/" $1;
else
    echo "Usage: ./add_dimacs_p_line.sh file
  Takes the path (file) to a dimacs file without a p line
  and updates the file to include a valid p line.
";
fi
   \endverbatim
and 
   \verbatim
#!/bin/bash 
#
# merge_cnfs.sh
#
# Takes the paths to three dimacs files, the first two
# m and n are input files and the third is the output.
# 
# The output file is simply the clauses of the first
# input file, followed by the clauses of the second file
# where the very first line of output is a DIMACS "p"
# line with the correct number of variables and clauses
# for the file.
#
# Note all comments will be removed.
# 

OKSOLVER_NUM_VARS_LINE='initial_number_of_variables'
OKSOLVER_NUM_CLAUSES_LINE='initial_number_of_clauses'

if [[ $# -eq 3 ]]; then
    cat $1 $2 | grep -v '^[pc]' > $3;
    ./add_dimacs_p_line.sh $3
else
    echo "Usage: ./merge_cnf.sh input1 input2 output
  Takes the paths to three dimacs files, the first two
  m and n are input files and the third is the output.

  The output file is simply the clauses of the first
  input file, followed by the clauses of the second file
  where the very first line of output is a DIMACS "p"
  line with the correct number of variables and clauses
  for the file.

  Note all comments will be removed.
";
fi
   \endverbatim
   and then we run the following commands to generate the final
   DIMACS file
   \verbatim
M=528;
QuineMcCluskey-n16-O3-DNDEBUG AES_Sbox_full.cnf > sbox_pi.cnf;
./add_dimacs_p_line.sh sbox_pi.cnf;
SubsumptionHypergraph-O3-DNDEBUG sbox_pi.cnf AES_Sbox_full.cnf > sbox_shg.cnf;
LinInequal-O3-DNDEBUG 136253 $M "<=" | sed 's/V//g' > LinInEq558.ecnf;
./merge_cnfs.sh sbox_shg.cnf LinInEq558.ecnf sbox_hgt.ecnf;
ExtendedToStrictDimacs-O3-DNDEBUG < sbox_hgt.ecnf > sbox_hgt.cnf;
   \endverbatim
   </li>
   <li> Note here that $M is the size of the clause-set we would like to generate,
   and then we iteratively reduce this value to find the size of the minimum
   CNF representation (until the whole thing is unsatisfiable). </li>
   <li> The resultant clause-set has 726038 variables and 2995245 clauses and
   the DIMACS file takes up 421MB. </li>
   <li> Note that the subsumption hypergraph for the Sbox prime implicates
   doesn't contain any unit hyperedges and aside from basic subsumption 
   elimination on the subsumption hypergraph, there are no simple reductions 
   such as those performed in the %QuineMcCluskey algorithm. </li>
   <li> MG is currently running experiments using the SAT solvers available in
   the OKlibrary. </li>
   <li> Running the standard "run_ubcsat" on the sbox_hgt.cnf instance with
   M=600 yields the following :
   \verbatim
sbox_eval = run_ubcsat("sbox_hgt.cnf")
sbox_mean_eval = aggregate(list(avg_beststep=sbox_eval$beststep,avg_cputime_mean=sbox_eval$CPUTime_Mean, avg_falsified = sbox_eval$best), list(alg=sbox_eval$alg), mean)
sbox_mean_eval[order(sbox_mean_eval$avg_falsified),]

                   alg avg_beststep avg_cputime_mean avg_falsified
3          gsat_simple    100000.00          85.9505      109121.2
2                 gsat    100000.00           6.5574      109139.0
13                rots    100000.00         158.2928      109145.1
16                saps    100000.00         346.6715      109150.9
6                 hsat    100000.00          92.1264      109151.1
15                samd    100000.00         114.0423      109164.1
14               rsaps    100000.00         327.8084      109188.2
8                irots    100000.00         199.0409      109202.0
4            gsat_tabu    100000.00         115.6767      109202.9
17              sapsnr    100000.00         282.5468      109594.9
7                hwsat     99999.83          79.7637      117324.8
1        adaptnoveltyp     99999.33           0.1970      160964.4
19        walksat_tabu     99999.43           0.1898      172123.7
20 walksat_tabu_nonull     99999.50           0.1900      172223.0
12           rnoveltyp     99999.48           0.1976      172608.5
11            rnovelty     99999.06           0.1969      172637.7
5                gwsat     99999.06          52.7133      178657.5
18             walksat     99998.96           0.1841      180616.5
9              novelty     99999.31           0.1975      180622.4
10            noveltyp     99999.57           0.1970      181339.2
   \endverbatim
   MG is currently running experiments for other values of M, and using gsat
   with large cutoffs (4000000).
   </li>
   <li> With 46/100 runs with UBCSAT using "gsat" and a cutoff of 4000000,
   UBCSAT seem to yield on average a minimum of 21500 falsified clauses
   with little difference (around 100-200) between these minimum numbers and
   every run reaching the maximum cutoff. This suggests we need to increase 
   the cutoff, but we are currently unable to do so due to UBCSATs 32 bit 
   limit on the cutoff. </li>
   <li> Using ubcsat 1.1.0 along with gsat, a cutoff of 40000000, and 
   translating the subsumption hypergraph for the Sbox from a MinSAT problem 
   to a weighted MaxSAT problem, yields the following small (294 clauses) CNF 
   \verbatim
Sbox294CNFF : [{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},
        {{-16,-15,-14,-10,-6,-5,8},{-16,-15,-14,-8,-2,13},{-16,-15,-14,4,7,9,13},
         {-16,-15,-11,-10,-9,1,2,6},{-16,-15,-11,-8,-7,-1,6},{-16,-15,-9,-6,5,12},
         {-16,-15,-8,-7,-6,1,2},{-16,-15,-5,-2,7,11,13},{-16,-15,-3,-1,12,14},
         {-16,-14,-12,-8,-5,-4,11},{-16,-14,-12,-5,4,6,13},{-16,-14,-12,-3,-2,6,7},
         {-16,-14,-12,-1,4,5,15},{-16,-14,-11,-6,-4,3},{-16,-14,-9,-8,2,3,6,15},
         {-16,-14,-7,-6,-1,2,8},{-16,-14,-6,4,8,15},{-16,-14,-3,2,4,5,12},{-16,-14,1,2,7,9,15},
         {-16,-13,-12,-7,5,8},{-16,-13,-12,-2,7,9,15},{-16,-13,-11,-9,-5,-2,1,3},
         {-16,-13,-11,-1,6,7},{-16,-13,-10,-5,-4,9},{-16,-13,-9,-6,-5,7,11},
         {-16,-13,-8,-7,-6,10,11},{-16,-13,-8,4,6,11,14},{-16,-13,-4,5,6,7},
         {-16,-12,-11,6,9,13,14},{-16,-12,-10,-9,6,8},{-16,-12,-9,-3,-2,4,13},
         {-16,-12,-8,-6,-5,-2,14},{-16,-12,-8,-3,1,7,13},{-16,-12,-6,-4,2,8},
         {-16,-12,-6,-3,5,9,15},{-16,-12,-5,1,11,14},{-16,-12,6,7,10,13,14},
         {-16,-11,-10,-7,-4,6,13},{-16,-11,-10,-4,5,14},{-16,-11,-6,-5,-3,-2,15},
         {-16,-11,-1,2,3,8},{-16,-11,2,3,14,15},{-16,-10,-8,-5,9,12},{-16,-10,-6,-4,5,12,15},
         {-16,-10,-2,3,4,5},{-16,-10,2,3,6,11,13},{-16,-10,2,6,7,14,15},{-16,-9,-7,2,8,10,15},
         {-16,-9,-6,-5,1,15},{-16,-9,-4,-3,1,8,10},{-16,-9,-3,1,4,5,7,14},{-16,-8,-7,-5,-1,11,15},
         {-16,-8,-7,2,6,10,13},{-16,-8,-6,-4,10,13,15},{-16,-8,-6,4,10,14},{-16,-7,-5,3,6,8,10},
         {-16,-7,1,3,10,11},{-16,-7,1,4,5,6,12},{-16,-6,3,4,8,13},{-16,-4,-2,5,6,11,12,13},
         {-16,-4,-2,8,9,10,13},{-16,-4,7,8,10,14},{-16,-3,-2,8,9,10,11},{-16,-3,2,6,11,15},
         {-16,-1,2,7,9,10},{-16,1,2,5,7,10,11},{-16,1,8,9,10,11,13,14},{-15,-14,-13,-9,3,5},
         {-15,-14,-13,-5,-2,-1,8,10},{-15,-14,-13,-5,-2,3,7},{-15,-14,-13,6,7,8},
         {-15,-14,-12,-10,-6,11,13,16},{-15,-14,-11,-10,3,7,13},{-15,-14,-11,-8,2,3,5},
         {-15,-14,-11,-5,-3,2,6},{-15,-14,-10,-9,-7,-3,-2,1},{-15,-14,2,3,10,16},
         {-15,-14,3,5,8,10},{-15,-13,-10,-7,-4,3,16},{-15,-13,-10,-1,3,9,12,16},
         {-15,-13,-7,-5,9,14,16},{-15,-13,-6,-4,5,7,11},{-15,-13,-5,-3,2,11},{-15,-13,-3,-2,5,9},
         {-15,-12,-10,-5,-1,4,9,11},{-15,-12,-10,-3,8,11,13},{-15,-12,-9,2,6,16},
         {-15,-12,-8,-4,3,13},{-15,-12,-4,3,10,14},{-15,-12,-3,-1,5,11},
         {-15,-11,-10,-9,-5,-4,8,16},{-15,-11,-10,-8,6,7,14},{-15,-11,-10,-7,-2,3,4},
         {-15,-11,-10,-3,5,9},{-15,-11,-6,-5,-3,4},{-15,-11,1,3,5,10},{-15,-9,-8,-3,4,16},
         {-15,-9,-7,-4,-1,11,12},{-15,-9,-7,3,5,14},{-15,-8,-7,-3,10,13,14},{-15,-8,-5,-3,-2,6,14},
         {-15,-7,-6,10,12,13},{-15,-7,2,11,13,16},{-15,-6,-4,-3,1,8,9},{-15,-6,3,8,11,13,14},
         {-15,-5,-4,-1,13,14},{-15,-5,-3,2,4,10},{-15,-5,-1,2,3,4,11},{-15,-5,1,2,3,7,10},
         {-15,-4,-3,5,6,16},{-15,-2,1,3,4,8,12,14},{-15,-1,3,4,7,9,10},{-15,-1,3,6,12,14},
         {-15,5,6,8,13,16},{-14,-13,-12,-10,1,11},{-14,-13,-11,-2,-1,5,15},
         {-14,-13,-10,-9,-4,-3,5},{-14,-13,-9,-7,-2,10,15},{-14,-13,-8,-5,4,12},
         {-14,-13,-3,2,5,10,16},{-14,-12,-11,-10,-8,-1},{-14,-12,-11,-8,-7,-2,10},
         {-14,-12,-10,-6,8,9,16},{-14,-12,-9,-6,-5,-3,16},{-14,-12,-7,1,5,10},{-14,-12,-1,4,5,6},
         {-14,-12,3,6,7,8},{-14,-11,-10,-1,6,12,13,15},{-14,-11,-9,-8,-4,-3,2},
         {-14,-11,-9,-7,-2,12,13},{-14,-11,-9,-3,5,8,16},{-14,-11,-8,-7,-6,-1},
         {-14,-11,-8,-7,1,4,15},{-14,-11,-4,-1,2,10,15},{-14,-11,-4,5,10,12},{-14,-11,-3,-2,4,8},
         {-14,-10,-2,3,8,12},{-14,-9,-8,3,6,11,12},{-14,-9,-7,2,3,8},{-14,-9,-7,2,10,13},
         {-14,-9,-6,-3,4,5,7},{-14,-9,-5,6,7,10,11},{-14,-8,-2,1,9,13},{-14,-8,-1,12,13,15},
         {-14,-7,-6,-4,15,16},{-14,-7,-3,2,6,11},{-14,-7,5,9,10,11,15,16},{-14,-6,-5,4,8,10,16},
         {-14,-5,-4,1,2,9,16},{-14,-4,-3,-2,8,13},{-14,-4,-1,5,11,12,13},{-14,-3,-1,7,8,11,13},
         {-14,-3,4,5,6,11},{-14,1,3,5,7,11},{-14,1,4,9,11,16},{-13,-12,-11,-8,-5,1,2},
         {-13,-12,-11,-6,3,9},{-13,-12,-11,-5,1,2,10},{-13,-12,-10,-2,7,8},
         {-13,-12,-8,-6,-2,-1,5,16},{-13,-12,-6,-4,3,10,14},{-13,-12,-6,2,4,5,14},
         {-13,-12,-3,6,9,15},{-13,-12,1,4,6,8},{-13,-11,-10,-3,1,16},{-13,-11,-8,-3,6,10},
         {-13,-11,1,2,12,16},{-13,-11,2,5,7,8,9},{-13,-10,-9,-7,-6,1,14},{-13,-10,-9,-5,-4,15},
         {-13,-10,-7,-1,8,15},{-13,-10,-5,2,4,12},{-13,-10,6,7,9,11},{-13,-9,-8,-6,-4,2,7},
         {-13,-9,-3,2,8,14,16},{-13,-9,-2,1,7,14,16},{-13,-8,-7,-4,6,11,14},{-13,-7,3,4,5,11,12},
         {-13,-6,-2,1,10,11,16},{-13,-6,-2,5,10,12,15},{-13,-5,-4,-3,-2,-1,16},{-13,-5,-4,-3,1,9},
         {-13,-4,1,5,9,12},{-13,-4,3,6,10,11},{-13,-3,-1,6,8,12,14,16},{-13,-3,1,4,9,14},
         {-12,-11,-9,-3,5,8,14},{-12,-11,-8,-6,-4,10},{-12,-11,-2,1,3,9,16},{-12,-11,5,7,10,15},
         {-12,-10,-9,-6,-5,-3,2,13},{-12,-10,-7,-3,4,9,13},{-12,-10,-7,5,11,13},
         {-12,-10,-5,-4,-3,14,15},{-12,-9,-8,3,4,16},{-12,-9,-7,-5,-1,4,6},
         {-12,-9,-7,-2,1,10,14,16},{-12,-9,-6,-2,-1,8,11},{-12,-9,6,10,13,14,15},
         {-12,-8,-7,11,14,15,16},{-12,-8,-6,7,9,10,11},{-12,-7,-5,-3,6,10},{-12,-7,-5,4,10,16},
         {-12,-7,-4,-2,1,5,6},{-12,-6,-5,3,13,15},{-12,-6,-3,-1,2,4,7,9},{-12,-6,-3,-1,8,9},
         {-12,-5,-1,3,7,14},{-12,-4,-2,-1,5,11},{-12,-4,2,7,8,13},{-12,-1,2,7,15,16},
         {-11,-10,-9,-6,-1,14},{-11,-10,-6,5,9,13},{-11,-10,-2,1,13,16},{-11,-10,1,7,12,15},
         {-11,-9,-6,-1,2,7},{-11,-9,-2,1,7,13,15},{-11,-8,-4,6,15,16},{-11,-8,-3,-1,4,5,12},
         {-11,-7,-6,-5,2,9,12},{-11,-7,-6,-4,1,12},{-11,-7,-5,-2,1,4,9,14},{-11,-7,5,6,14,15},
         {-11,-6,-5,-2,3,16},{-11,-6,-2,-1,3,14},{-11,-5,-3,6,8,14},{-11,-5,4,12,13,14,15},
         {-11,-3,1,7,9,10,14},{-11,-2,3,5,6,7,12},{-11,2,3,5,10,15},{-10,-9,-8,2,3,5,7},
         {-10,-9,-7,-6,-5,3,4},{-10,-9,-7,-6,-1,5,15},{-10,-8,-1,2,3,5,9},{-10,-8,-1,9,11,12},
         {-10,-8,1,9,14,16},{-10,-7,-5,3,6,14},{-10,-7,-3,4,8,11},{-10,-7,-2,1,4,15},
         {-10,-7,2,3,6,13,16},{-10,-6,-3,7,9,12,16},{-10,-5,2,3,7,14},{-10,-3,1,2,8,11},
         {-10,-3,4,6,7,9,14},{-10,1,2,8,15,16},{-10,3,6,9,11,15},{-9,-8,-5,2,10,14},
         {-9,-8,-2,7,11,16},{-9,-6,8,12,14,16},{-9,-5,-4,-3,-2,12},{-9,-5,-4,-1,3,6,13},
         {-9,-5,1,7,8,13},{-9,-4,1,5,11,15,16},{-9,-2,1,8,11,12},{-9,3,4,5,8,11,14},
         {-9,3,5,7,8,15,16},{-9,4,11,12,14,15},{-8,-5,-4,3,9,10,12},{-8,-5,-3,11,12,13},
         {-8,-5,-1,4,15,16},{-8,-3,1,6,15,16},{-8,1,4,7,13,16},{-8,1,5,9,11,13},{-8,3,9,13,14,16},
         {-7,-6,-3,13,14,15,16},{-7,-6,2,5,9,14},{-7,-4,-2,-1,3,15,16},{-7,-2,-1,3,9,14},
         {-7,-1,12,13,14,15},{-7,2,3,4,14,16},{-7,2,6,8,9,15},{-6,-5,-4,2,3,9,12,16},
         {-6,-5,8,11,12,15},{-6,-3,2,5,13,14},{-6,-2,1,5,11,14,15},{-6,1,5,11,12,13,16},
         {-6,2,3,7,11,12},{-5,-4,-2,-1,3,9,16},{-5,-4,-2,1,3,13},{-5,-4,1,6,8,9,13,14},
         {-5,-4,3,8,12,15,16},{-5,-2,3,6,10,12},{-4,-3,-1,7,9,10},{-4,-3,2,7,12,13,14},
         {-4,-3,5,10,12,14,16},{-4,5,7,11,13,14,15,16},{-3,-2,-1,4,7,10,15},{-3,-2,-1,6,13,16},
         {-2,-1,4,11,12,15},{-2,4,5,9,13,16},{1,4,5,8,10,13,15},{2,4,7,8,10,12},
         {4,6,9,12,14,15,16},{4,7,10,11,14,16}}]$
   \endverbatim
   where
   \verbatim
> statistics_fcs(Sbox294CNFF);
[16,294,1939,8,6]
   \endverbatim
   and we can check that this is indeed a CNF representation of the Sbox using
   the following
   \verbatim
> is(rijnsbox_fulldnf_fcs()[2] = all_sat_fcs(Sbox294CNFF));
true
   \endverbatim
   </li>
  </ul>
  </li>
 </ul>


  \todo Understanding prime implicates after any partial assignment
  <ul>
   <li> To consider the AES Sbox as an "active clause", we want to first be 
   able, given a partial assignment, to infer as many forced assignments
   as possible. This can be done simply with the DNF representation. </li>
   <li> However, secondly one needs, given a partial assignment, to be able to
   determine various measures for heuristics. </li>
   <li> Therefore, investigating several statistics (most notably the number of
   clauses for a given variable) of the prime implicates of the clause-set, 
   formed after taking the Sbox and applying each partial assignment, is 
   necessary to try and discern a pattern. </li>
   <li> If such patterns can be deduced for particular clause-set measures,
   then the active clause can use this pattern, given a partial assignment, 
   to return reasonable values for these measures which can be used for 
   statistics. </li>
   <li> A C++ implementation of such a system whereby the set of prime 
   implicates is taken as input, and each partial assignment along with
   the relevant statistics is returned is necessary. </li>
   <li> Such a C++ implementation would need to be able to apply a partial
   assignment to a clause-set and then compute various statistics on the 
   result. This would need to be done for every partial assignment. </li>
   <li> After applying the partial assignment, to gain the prime implicates
   of the new boolean function, one must simply apply subsumption elimination
   to the new clause-set (which is just result of applying a partial assignment
   to the prime implicates of the original function). This can be done using 
   functionality already in the library (MG: Where?). </li>
  </ul>


  \todo DONE (Small representations are not r1-based, see
  "Investigate CNF representations" for links to r1-based discussions)
  Investigating Sbox given 8 bit input
  <ul>
   <li> Given 8 bits input to the Sbox, the output should be immediately 
   determined without any decisions needed. Is this actually the case? </li>
   <li> This question is motivated by the fact that the full AES translation
   using a representation of the sbox and field multiplications without new 
   variables seems to make some decisions when given all the input bits it 
   needs, and this should not be the case. </li>
   <li> The "default" Sbox representation with no new variables currently used 
   can be generated in the following way:
   \verbatim
h2_aes : rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit_tb(4,4))$
p2_aes : replace_by_prime_implicates_hitting(h2_aes)$
Sbox44ICCNF : first_irr_fcs(cs_to_fcs(p2_aes), dll_simplest_trivial2)$
   \endverbatim
   In the following, we shall call this Sbox representation Sbox44ICCNF.
   </li>
   <li> Looking at this in maxima, with all input bits set to 0:
   \verbatim
(%i7) apply_pa({-1,-2,-3,-4,-5,-6,-7,-8}, Sbox44ICCNF[2]);
Evaluation took 0.0200 seconds (0.0200 elapsed)
(%o7) {{-16,-15,-14,-13,11,12},{-16,-14,-12,11,13},{-16,-14,-11,15},{-16,-13,11},{-16,-12,15},
       {-16,-11,-9},{-16,-10,11,14},{-16,9,10},{-16,13,15},{-15,-14,-13,11,16},{-15,-14,11,13},
       {-15,-13,-12,11,16},{-15,-13,9,16},{-15,-13,10,16},{-15,-12,-10,11,13},{-15,-12,-9,11,14},
       {-15,-12,11,13,16},{-15,-11,10},{-15,-10,12,13,14,16},{-15,-9,10,11,12},{-15,9,10,14},
       {-15,9,11,14},{-15,10,12,16},{-14,-13,9,11},{-14,-12,-11,10},{-14,-12,-9,13,15},
       {-14,-12,9,10},{-14,-12,9,11,15},{-14,-12,9,11,16},{-14,-12,11},{-14,-11,-9,12,16},
       {-14,-9,10,11,16},{-14,9},{-14,9,10},{-14,9,15},{-14,10,16},{-14,11,16},{-14,12,16},
       {-14,13,16},{-13,-12,-11,14,16},{-13,-12,-9,14},{-13,-12,9},{-13,9,10,12,15},{-13,9,11,14},
       {-13,9,14},{-13,10,16},{-13,12,15,16},{-12,-11,10,13,14,16},{-12,9,10,11,15},{-12,9,11,15},
       {-12,9,13,14},{-11,-9,10,16},{-11,-9,14,15,16},{-11,9,10,14,15},{-11,10,14,15},{-11,14,16},
       {-10,-9,11,14},{-10,9,14,16},{-10,16},{-9,10,11},{-9,10,12,13},{-9,11,12,15},{-9,14,15},
       {-9,14,15,16},{-9,15,16},{9,10,12,13,16},{9,10,12,14},{9,11,12,13,14,15},{9,11,12,15,16},
       {9,11,13},{9,12,14,16},{9,12,16},{9,13,14,16},{10,11,12,14},{10,11,13,14,15,16},{10,12,15},
       {10,13,16},{10,14},{11,12,16},{11,13,14},{11,15,16}}
(%i8) statistics_cs(apply_pa({-1,-2,-3,-4,-5,-6,-7,-8}, Sbox44ICCNF[2]));
Evaluation took 0.0280 seconds (0.0270 elapsed)
(%o8) [8,81,320,6,2]
   \endverbatim
   </li>
   <li> With several different assignments in maxima: 
   \verbatim
(%i5) statistics_cs(apply_pa({-1,-2,-3,-4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0270 elapsed)
(%o5) [8,81,320,6,2]
(%i6) statistics_cs(apply_pa({1,-2,-3,-4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0280 seconds (0.0260 elapsed)
(%o6) [8,81,320,6,2]
(%i7) statistics_cs(apply_pa({-1,2,-3,-4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0280 seconds (0.0250 elapsed)
(%o7) [8,69,272,6,2]
(%i8) statistics_cs(apply_pa({-1,-2,3,-4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0250 elapsed)
(%o8) [8,76,288,6,1]
(%i9) statistics_cs(apply_pa({-1,-2,-3,4,-5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0260 elapsed)
(%o9) [8,74,298,6,2]
(%i10) statistics_cs(apply_pa({-1,-2,-3,-4,5,-6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0240 elapsed)
(%o10) [8,68,264,6,2]
(%i11) statistics_cs(apply_pa({-1,-2,-3,-4,-5,6,-7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0260 elapsed)
(%o11) [8,81,319,6,1]
(%i12) statistics_cs(apply_pa({-1,-2,-3,-4,-5,-6,7,-8},Sbox44ICCNF[2]));
Evaluation took 0.0280 seconds (0.0270 elapsed)
(%o12) [8,84,324,6,1]
(%i13) statistics_cs(apply_pa({-1,-2,-3,-4,-5,-6,-7,8},Sbox44ICCNF[2]));
Evaluation took 0.0240 seconds (0.0250 elapsed)
(%o13) [8,73,287,6,2]
   \endverbatim
   </li>
   <li> Setting all 8 input bits (variables 1 to 8) to 0 (by adding unit 
   clauses -1 ... -8) for Sbox44CNF: 
   \verbatim
[12:53:58 - aeternus] data$ ./minisat/minisat/core/minisat test.cnf 
restarts              : 1
conflicts             : 0              (nan /sec)
decisions             : 2              (0.00 % random) (inf /sec)
propagations          : 16             (inf /sec)
conflict literals     : 0              ( nan % deleted)
CPU time              : 0 s
SATISFIABLE
   \endverbatim
   As can be seen two decisions are made (but no conflicts).
   </li>
   <li> Setting all inputs bits to 1 for Sbox44CNF :
   \verbatim
[12:55:45 - aeternus] data$ ./minisat/minisat/core/minisat test.cnf 
restarts              : 1
conflicts             : 3              (750 /sec)
decisions             : 7              (0.00 % random) (1750 /sec)
propagations          : 26             (6500 /sec)
conflict literals     : 10             (9.09 % deleted)
CPU time              : 0.004 s
SATISFIABLE
   \endverbatim
   As can be seen, in this case, 3 conflicts occur and 7 decisions are made.
   </li>
   <li> Whether or not unit clauses occur immediately after setting all Sbox
   input bits (using Sbox44ICCNF) depends on the assignment. </li>
   <li> A better Sbox CNF representation seems to be needed. </li>
   <li> However, the primary goal is for problems where the key is not known,
   not simple encryption and decryption, so further insight into where the
   Sbox is used and exactly what bits are commonly set and that we wish to
   allow easier deductions with. </li>
  </ul>

*/
