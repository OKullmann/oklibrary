# Oliver Kullmann, 16.1.2008 (Swansea)
# Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# The list of links to be established to programs from the OKlibrary.

oksolver2002_okl ?= $(bin_dir)/OKsolver_2002-O3-DNDEBUG
oksolver2002outputxml_okl ?= $(bin_dir)/OKsolver_2002_OUTPUTXML-O3-DNDEBUG
oksolver2002_xml2maxima_okl ?= $(OKlib)/Satisfiability/Solvers/OKsolver/SAT2002/Xml2Maxima

ucp_okl ?= $(bin_dir)/UnitClausePropagation-O3-DNDEBUG
ucpw_okl ?= $(bin_dir)/UnitClausePropagationW-O3-DNDEBUG
rucpbase_okl ?= $(bin_dir)/RUcpBase-O3-DNDEBUG
rucpgen_okl ?= $(bin_dir)/RUcpGen-O3-DNDEBUG

ramsey_generator_okl ?= $(bin_dir)/Ramsey-O3-DNDEBUG
vanderwaerden_generator_okl ?= $(bin_dir)/VanderWaerdenCNF-O3-DNDEBUG

vdw_minimumtransversals_okl ?= $(bin_dir)/MinimumTransversals_VanderWaerden-O3-DNDEBUG
gt_minimumtransversals_okl ?= $(bin_dir)/MinimumTransversals_GreenTao-O3-DNDEBUG
vdwtransversals_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/VdWTransversals
vdwtransversalspb_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/VdWTransversalsPB
vdwtransversalsinc_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/VdWTransversalsInc
vdwtransversalsincpb_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/VdWTransversalsIncPB
gttransversals_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/GTTransversals
gttransversalsinc_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/GTTransversalsInc

lininequal_trans_okl ?= $(bin_dir)/LinInequal-O3-DNDEBUG
boundones_okl ?=  $(OKlib)/Satisfiability/Transformers/Generators/BoundOnes
oksolver_2002_m2pp_okl ?= $(OKlib)/Satisfiability/Solvers/OKsolver/SAT2002/OKsolver_2002-m2pp
satz215_m2pp_okl ?= $(OKlib)/Satisfiability/Solvers/Satz/satz215-m2pp

extdimacs2dimacs_okl ?= $(bin_dir)/ExtendedToStrictDimacs-O3-DNDEBUG
extdimacsstatistics_okl ?= $(bin_dir)/ExtendedDimacsStatistics-O3-DNDEBUG
extdimacsfullstatistics_okl ?= $(bin_dir)/ExtendedDimacsFullStatistics-O3-DNDEBUG

boundedtransversalsbv_okl ?= $(bin_dir)/BoundedTransversals_bv-O3-DNDEBUG

rankprimes_okl ?= $(bin_dir)/RankPrimes-O3-DNDEBUG
grosswaldhagisformula_okl ?= $(bin_dir)/GrosswaldHagisFormula-O3-DNDEBUG

greentao_okl ?= $(bin_dir)/GreenTao-O3-DNDEBUG
pdvanderwaerden_okl ?= $(bin_dir)/PdVanderWaerden-O3-DNDEBUG
countprogressionsgreentao_okl ?= $(bin_dir)/CountProgressions_GreenTao-O3-DNDEBUG
pdextend_okl ?= $(bin_dir)/PdExtend-O3-DNDEBUG
solutionshift_okl ?= $(bin_dir)/SolutionShift-O3-DNDEBUG

greentaocnf_okl ?= $(bin_dir)/GreenTaoCNF-O3-DNDEBUG
greentaogcnf_okl ?= $(bin_dir)/GreenTaoGCNF-O3-DNDEBUG
greentaogen_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/GTSat
greentaodgen_okl ?= $(OKlib)/Satisfiability/Transformers/Generators/GTdSat
pdvanderwaerdencnf_okl ?= $(bin_dir)/PdVanderWaerdenCNF-O3-DNDEBUG


smusathorn_okl ?= $(bin_dir)/Smusat_Horn-O3-DNDEBUG
musatd2_okl ?= $(bin_dir)/Musatd2-O3-DNDEBUG

quinemccluskey_okl ?= $(bin_dir)/QuineMcCluskey-n16-O3-DNDEBUG
quinemccluskeysubsumptionhypergraph_okl ?= $(bin_dir)/QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG
quinemccluskeysubsumptionhypergraphwithfullstatistics_okl ?= $(bin_dir)/QuineMcCluskeySubsumptionHypergraphWithFullStatistics-n16-O3-DNDEBUG
quinemccluskeysubsumptionhypergraphfullstatistics_okl ?= $(bin_dir)/QuineMcCluskeySubsumptionHypergraphFullStatistics-n16-O3-DNDEBUG

runvdw3k_okl ?=  $(OKlib)/Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/RunVdW3k
extractcertificatevdW2_okl ?=  $(OKlib)/Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/ExtractCertificate_vdW2
runpdvdw3k_okl ?=  $(OKlib)/Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/RunPdVdW3k

extractsolutionubcsat_okl ?= $(OKlib)/Experimentation/ExperimentSystem/ControllingLocalSearch/extract_solution_ubcsat

public_link_list_okl ?= $(oksolver2002_okl) $(oksolver2002outputxml_okl) $(oksolver2002_xml2maxima_okl) $(ucp_okl) $(ucpw_okl) $(rucpbase_okl) $(rucpgen_okl) $(ramsey_generator_okl) $(vanderwaerden_generator_okl) $(lininequal_trans_okl) $(boundones_okl) $(vdwtransversals_okl) $(vdwtransversalspb_okl) $(vdwtransversalsinc_okl) $(vdwtransversalsincpb_okl) $(gttransversals_okl) $(gttransversalsinc_okl) $(oksolver_2002_m2pp_okl) $(satz215_m2pp_okl) $(extdimacs2dimacs_okl) $(extdimacsstatistics_okl) $(extdimacsfullstatistics_okl) $(boundedtransversalsbv_okl) $(rankprimes_okl) $(grosswaldhagisformula_okl) $(greentao_okl) $(pdvanderwaerden_okl) $(countprogressionsgreentao_okl) $(pdextend_okl) $(solutionshift_okl) $(quinemccluskey_okl) $(quinemccluskeysubsumptionhypergraph_okl) $(quinemccluskeysubsumptionhypergraphwithfullstatistics_okl) $(quinemccluskeysubsumptionhypergraphfullstatistics_okl) $(vdw_minimumtransversals_okl) $(gt_minimumtransversals_okl) $(greentaocnf_okl) $(greentaogcnf_okl) $(greentaogen_okl) $(greentaodgen_okl) $(pdvanderwaerdencnf_okl) $(smusathorn_okl) $(musatd2_okl) $(oklib_call) $(runvdw3k_okl) $(runpdvdw3k_okl) $(extractcertificatevdW2_okl) $(extractsolutionubcsat_okl)

