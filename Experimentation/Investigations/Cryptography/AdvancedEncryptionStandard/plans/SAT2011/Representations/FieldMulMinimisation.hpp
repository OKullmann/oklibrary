// Matthew Gwynne, 6.8.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/FieldMulMinimisation.hpp
  \brief On investigations into the AES field multiplications


  \todo Connections
  <ul>
   <li> See Investigations/BooleanFunctions/plans/Permutations.hpp for
   general investigations on permutations of {0,1}^n. </li>
  </ul>


  \todo Minimum size small scale AES operations
  <ul>
   <li> See ssmult_fullcnf_fcs() in
   OKlib/ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/testobjects/FieldOperationsAnalysis.mac . </li>
   <li> In the small scale AES, the size of the field and the size of the 
   block may vary and therefore, rather than simplying having the standard 8 
   bit "byte" field in AES, the "word" (a generalisation of byte) field
   may now be of arbitrary size. </li>
   <li> In reality, in the literature (see [Small Scale Variants of the AES; 
   Cid, Murphy and Robshaw]), only one field of size 4 is considered and
   in every field, only multiplications by 01, 02, and 03 are considered 
   (in each respective field). </li>
   <li> Therefore, other than those given for the standard AES, we must
   also consider the minimisation of these 01,02 and 03 multiplications
   over the 4 bit field. </li>
   <li> So we are considering the following minisation problems
   (given as maxima code to generate their minimum CNF representations) :
   <ul>
    <li> Multiplication by 01 in any field is the trivial identity, so we have
    the trivial equivalence translation as the minimum representation. </li>
    </li>
    <li> Multiplication by 02 in GF(2^4) with x^4+x+1 as the modulo polynomial:
    \verbatim
min_2_cnfs : all_minequiv_bvs_cs(ssmult_fullcnf_fcs(2,2,4,ss_polynomial_2_4)[2]);
    \endverbatim
    </li>
    <li> Multiplication by 03 in GF(2^4) with x^4+x+1 as the modulo polynomial:
    \verbatim
min_3_cnfs : all_minequiv_bvs_cs(ssmult_fullcnf_fcs(3,2,4,ss_polynomial_2_4)[2]);
    \endverbatim
    </li>
    </ul>
   </li>
  </ul>

 
  \todo Minimisation of the representations of field operations
  <ul>
   <li> See "Minimisation" in 
   OKlib/Satisfiability/FiniteFunctions/plans/general.hpp . </li>
   <li> R QCA Packages
   <ul>
    <li> We can use the QCA package, given in 
    Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp to compute
    the minimum sized CNF or DNF clause-set representation. </li>
    <li> This should be possible using the following code:
     \verbatim
######## In Maxima #######
generate_full_byteop_tt(byteop) :=  
  map(
     lambda([ce],
       append(
         int2polyadic_padd(ce[1],2,8),
         int2polyadic_padd(ce[2],2,8),
         if byteop(ce[1]) = ce[2] then [1] else [0]))
     ,cartesian_product(setmn(0,255),setmn(0,255)))$

rijn_lookup_mul : lambda([b],
  buildq([b],lambda([a], aes_field_mul_data[b,a])))$

mulConstant : 2;
with_stdout(sconcat("RijndaelMul",mulConstant,".tt"), block(
  apply(print, endcons("O",create_list(i,i,1,16))),
  for tt_line in generate_full_byteop_tt(rijn_lookup_mul(mulConstant)) do
    apply(print,tt_line)
  ))$


######## In R ###########

oklib_load_all()
library(QCA)

mulConstant = 2
mul_tt = read.table(paste("RijndaelMul",mulConstant,".tt",sep=""),header=TRUE)
eqmcc(mul_tt, outcome="O", expl.0=TRUE)
    \endverbatim
    where mulConstant can be set in each case to one of 2,3 for the 
    multiplication in the encryption direction, and 9,11,13 or 14 for
    the multiplications used when the decryption of MixColumn is
    included in the translation. </li>  
    <li> Even with multiplication by 02, the R/QCA system still runs out of
    memory (see "Minimisation" in 
    OKlib/Satisfiability/FiniteFunctions/plans/general.hpp for details). </li>
    <li> Another possibility is to minimise the field multiplications by 02
    using the minimum transversal functions present in the Maxima subsystem. 
    Assuming the prime implicates for multiplication by 02 have been generated
    like so:
    \verbatim
output_rijnmult_fullcnf_stdname(2);
    \endverbatim
    in Maxima, and then from the shell
    \verbatim
QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_2.cnf > AES_Mul2_PI.cnf
    \endverbatim
    the following, in Maxima, should produce a set of all minimum 
    representations
    \verbatim 
oklib_plain_include("stringproc")$

read_fcs_f(n) := block([fh, line, ll, cs : [], l,b,c],
  fh : openr(n),
  while stringp(line : readline(fh)) do (
    ll : tokens(line),
    if length(ll) >= 1 then
      if not(first(ll) = "c" or first(ll) = "p") then
        cs : cons(setify(rest(map(parse_string,ll),-1)), cs)
  ),
  cs : setify(cs),
  return(cs_to_fcs(cs))
)$

Mul2PI : read_fcs("AES_Mul2_PI.cnf")$
MTHG2 : minimum_transversals_bvs_hg(ghg2hg(subsumption_ghg(Mul2[2], rijnmult_fullcnf_fcs(2)[2])))$
    \endverbatim
    </li>
    <li> For multiplication by 02, the above Maxima function returns 102 
    minimum CNF representations of size 20 in 2190.1490 seconds. </li>
    <li> An example of such a minimum representation is:
    \verbatim
{{-16,-15,-8},{-16,-13,-6},{-16,6,13},{-16,8,15},{-15,1,8},{-14,7},{-13,1,6},
{-12,-5,-1},{-12,5,16},{-11,4},{-10,3},{-9,2},{-8,1,15},{-7,14},{-6,1,13},
{-5,12,16},{-4,11},{-3,10},{-2,9},{-1,5,12}}
    \endverbatim
    </li>
    <li> Most (90) of the minimum representations contain 8 clauses of size 2, 
    and 12 clauses of size 3. There are then a further twelve clause-sets 
    where there are only 8 clause of size 3, but then 4 clauses of size 4. 
    </li>
    <li> MG is currently running experiments with the other field 
    multiplications. </li>
   </ul>
   </li>
   <li> Espresso-ab (see Logic "synthesis" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp)
   <ul>
    <li> Espresso-ab takes as input a truth table in PLA format. </li>
    <li> We can generate a truth table in PLA format for the Sbox in the
    following way:
    \verbatim
generate_full_aes_mul_tt(elem) :=  
  map(
     lambda([ce],
       append(
         int2polyadic_padd(ce[1],2,8),
         int2polyadic_padd(ce[2],2,8),
         if rijn_natmul(elem,ce[1]) = ce[2] then [1] else [0]))
     ,cartesian_product(setmn(0,255),setmn(0,255)))$

elem : 3;
with_stdout(sconcat("Mul",elem,".pla"), block(
  print(".i 16"),
  print(".o 1"),
  for tt_line in generate_full_aes_mul_tt(elem) do
    print(apply(sconcat,rest(tt_line,-1)),1-last(tt_line))
  ))$
    \endverbatim
    where the PLA file will be called "Sbox.pla", and will be represented
    as a DNF representing the negation of the Sbox (as by default Espresso
    minimises DNF formulas.
    </li>
    <li> Using espresso-ab with the "exact" option for 02, espresso
    yields a minimum size clause-set of size 10, however
    for 03, espresso-ab runs out of memory on an 8GB machine. </li>
   </ul>
   </li>
  </ul>


  \todo Finding small representations
  <ul>
   <li> For all but the non-trivial (01 and 02) multiplications, finding the
   minimum representation, whether it be using espresso, or using branch and
   bound methods, very little progresses is made by the solvers. This is
   presumably due to the large number of prime implicates, and sheer
   combinatorial explosion. </li>
   <li> Therefore, we must also consider only small minimal solutions, which
   might not be the minimum but we hope are close. </li>
   <li> Finding small CNF representations via weighted MaxSAT
   <ul>
    <li> To find the minimum representation for the full clause-set
    of a given field multiplication, generated like so
    \verbatim
output_rijnmult_fullcnf_stdname(2);
    \endverbatim
    we can consider the translation of this to the subsumption hypergraph
    for the clause-set and it's prime implicates (see XXX) and then
    translate this subsumption hypergraph to a weighted MaxSAT problem
    where the minimum weight solutions to this problem correspond exactly
    to the minimum sized clause-set representations of the original clause-set.
    </li>
    <li> To do this, we construct the subsumption hypergraph like so
    \verbatim
QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_byte_field_mul_full_2.cnf AES_byte_field_mul_2_pi.cnf > AES_byte_field_mul_2_shg.cnf
    \endverbatim
    where the prime implicates for the "AES_byte_field_mul_full_2.cnf" are 
    placed in "AES_byte_field_mul_2_pi.cnf" and the subsumption hypergraph 
    relating the prime implicates to the original total clauses is placed in
    "AES_byte_field_mul_2_shg.cnf". 
    </li>
    <li> Then the subsumption hypergraph must be converted to a weight MaxSAT
    problem, which can be accomplished using a simple AWK script, like so
    \verbatim
cat AES_byte_field_mul_2_shg.cnf | awk --file ${OKPLATFORM}/OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/shg2partial_maxsat.awk > AES_byte_field_mul_2_shg.wcnf
    \end
    (The above translation needs to be further specified).
    </li>
    <li> Given such a weighted maxsat problem, one can then use ubcsat to
    search for small solutions, like so
    \verbatim
ubcsat-okl  -alg gsat -w -runs 100 -cutoff 1000000 -i AES_byte_field_mul_2_shg.wcnf
    \endverbatim
    </li>
    <li> Given a solution of weight 20 with seed 1402276559 we can then 
    generate the clause set of that weight like so
    \verbatim
new-ubcsat-okl  -alg gsat -w -runs 100 -cutoff 1000000 -wtarget 20 -solve 1 -seed 1402276559 -i AES_byte_field_mul_2_shg.wcnf -r model AES_byte_field_mul_2_m20.result; 
cat AES_byte_field_mul_2_pi.cnf | FilterDimacs AES_byte_field_mul_2_m20.result
    \endverbatim
    </li>
   </ul>
   </li>
  </ul>


*/
