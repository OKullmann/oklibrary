# Oliver Kullmann, 16.1.2008 (Swansea)
# Copyright 2008, 2009, 2010, 2011, 2012, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Internet domains:

oklibrary_domain := http://www.ok-sat-library.org
oklibrary_sourceforge := http://oklibrary.sourceforge.net/
oklibrary_mailinglist_users := http://www.freelists.org/list/oklibrary-users
oklibrary_mailinglist_devel := http://lists.sourceforge.net/mailman/listinfo/oklibrary-developers


# The list of links to be established to programs from the OKlibrary:

oksolver2002_okl ?= $(bin_dir)/OKsolver_2002-O3-DNDEBUG
oksolver2002ntp_okl ?= $(bin_dir)/OKsolver_2002_NTP-O3-DNDEBUG
oksolver2002outputxml_okl ?= $(bin_dir)/OKsolver_2002_OUTPUTXML-O3-DNDEBUG
oksolver2002_xml2maxima_okl ?= $(OKlib)/Satisfiability/Solvers/OKsolver/SAT2002/Xml2Maxima
splitviaoksolver_okl ?= $(OKlib)/Satisfiability/Solvers/OKsolver/SAT2002/SplittingViaOKsolver
processsplitviaoksolver_okl ?= $(OKlib)/Satisfiability/Interfaces/DistributedSolving/ProcessSplitViaOKsolver
processicnf_okl ?= $(OKlib)/Satisfiability/Interfaces/DistributedSolving/ProcessiCNF
extracticnf_okl ?= $(OKlib)/Satisfiability/Interfaces/DistributedSolving/ExtractiCNF
extractdecisionsicnf_okl ?= $(OKlib)/Satisfiability/Interfaces/DistributedSolving/ExtractDecisionsiCNF
extractcryptominisat_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractCryptominisat
extractglucose_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractGlucose
extractlingeling_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractLingeling
extractminisat_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat
extractoksolver_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver
extractprecosat570_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractPrecosat570
extractpicosat_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractPicosat
extractsatz_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractSatz
extractmarchpl_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMarchpl
extracttawsolver_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/ExtractTawSolver
tawsolver_okl ?= $(bin_dir)/tawSolver
ttawsolver_okl ?= $(bin_dir)/ttawSolver
ctawsolver_okl ?= $(bin_dir)/ctawSolver
cttawsolver_okl ?= $(bin_dir)/cttawSolver

selectextractiontool_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/SelectExtractionTool

runminisat_okl ?= $(OKlib)/Experimentation/ExperimentSystem/SolverMonitoring/RunMinisat
random_okl ?= $(OKlib)/Experimentation/ExperimentSystem/Random

ucp_okl ?= $(bin_dir)/UnitClausePropagation-O3-DNDEBUG
ucpw_okl ?= $(bin_dir)/UnitClausePropagationW-O3-DNDEBUG
rucpbase_okl ?= $(bin_dir)/RUcpBase-O3-DNDEBUG
rucpgen_okl ?= $(bin_dir)/RUcpGen-O3-DNDEBUG
randomrucpbases_okl ?= $(OKlib)/Satisfiability/Reductions/Bases/RandomRUcpBases

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
sortbyclauselength_okl ?= $(bin_dir)/SortByClauseLength-O3-DNDEBUG
randomshuffledimacs_okl ?= $(bin_dir)/RandomShuffleDimacs-O3-DNDEBUG
appenddimacs_okl ?= $(bin_dir)/AppendDimacs-O3-DNDEBUG
manipparam_okl ?= $(bin_dir)/ManipParam-O3-DNDEBUG

preprocesssplitting_okl ?= $(bin_dir)/PreprocessSplitting-O3-DNDEBUG
collectingpartialassignments_okl ?= $(bin_dir)/CollectingPartialAssignments-O3-DNDEBUG
collectingdecisions_okl ?= $(bin_dir)/CollectingDecisions-O3-DNDEBUG

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
quinemccluskeysubsumptionhypergraphfullstatistics_okl ?= $(bin_dir)/QuineMcCluskeySubsumptionHypergraphFullStatistics-n16-O3-DNDEBUG $(bin_dir)/QuineMcCluskeySubsumptionHypergraphFullStatistics-n4-O3-DNDEBUG

applypass_okl ?= $(bin_dir)/ApplyPass-O3-DNDEBUG
passextends_okl ?= $(bin_dir)/PassExtends-O3-DNDEBUG
passclashes_okl ?= $(bin_dir)/PassClashes-O3-DNDEBUG

runvdw3k_okl ?=  $(OKlib)/Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/RunVdW3k
runvdwk1k2_okl ?=  $(OKlib)/Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/RunVdWk1k2
extractcertificatevdW2_okl ?=  $(OKlib)/Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/ExtractCertificate_vdW2
runpdvdwk1k2_okl ?=  $(OKlib)/Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/RunPdVdWk1k2
crunpdvdwk1k2_okl ?=  $(OKlib)/Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/CRunPdVdWk1k2

crunpdschur_okl ?= $(OKlib)/Experimentation/Investigations/RamseyTheory/SchurProblems/CRunPdSchur
crunpdwschur_okl ?= $(OKlib)/Experimentation/Investigations/RamseyTheory/SchurProblems/CRunPdWSchur
crunpdschurfsb_okl ?= $(OKlib)/Experimentation/Investigations/RamseyTheory/SchurProblems/CRunPdSchurFsb
crunpdwschurfsb_okl ?= $(OKlib)/Experimentation/Investigations/RamseyTheory/SchurProblems/CRunPdWSchurFsb
crunmschur_okl ?= $(OKlib)/Experimentation/Investigations/RamseyTheory/SchurProblems/CRunMSchur
crunwmschur_okl ?= $(OKlib)/Experimentation/Investigations/RamseyTheory/SchurProblems/CRunWMSchur

randomdestotalassignment_okl ?= $(OKlib)/Experimentation/Investigations/Cryptography/DataEncryptionStandard/RandomDESTotalAssignment

extractsolutionubcsat_okl ?= $(OKlib)/Experimentation/ExperimentSystem/ControllingLocalSearch/extract_solution_ubcsat

measurecpuspeed_okl ?= $(bin_dir)/MeasureCPUSpeed-O3-DNDEBUG
measurecpuspeedmem_okl ?= $(bin_dir)/MeasureCPUSpeedMem-O3-DNDEBUG

public_link_list_okl ?= $(oksolver2002_okl) $(oksolver2002ntp_okl) $(oksolver2002outputxml_okl) $(oksolver2002_xml2maxima_okl) $(splitviaoksolver_okl) $(processsplitviaoksolver_okl) $(processicnf_okl) $(extracticnf_okl) $(extractdecisionsicnf_okl) $(extractcryptominisat_okl) $(extractglucose_okl) $(extractlingeling_okl) $(extractminisat_okl) $(extractoksolver_okl) $(extractsatz_okl) $(extractmarchpl_okl) $(extractprecosat570_okl) $(extractpicosat_okl) $(extracttawsolver_okl) $(selectextractiontool_okl) $(runminisat_okl) $(random_okl) $(ucp_okl) $(ucpw_okl) $(rucpbase_okl) $(rucpgen_okl) $(randomrucpbases_okl) $(ramsey_generator_okl) $(vanderwaerden_generator_okl) $(lininequal_trans_okl) $(boundones_okl) $(vdwtransversals_okl) $(vdwtransversalspb_okl) $(vdwtransversalsinc_okl) $(vdwtransversalsincpb_okl) $(gttransversals_okl) $(gttransversalsinc_okl) $(oksolver_2002_m2pp_okl) $(satz215_m2pp_okl) $(extdimacs2dimacs_okl) $(extdimacsstatistics_okl) $(extdimacsfullstatistics_okl) $(sortbyclauselength_okl) $(randomshuffledimacs_okl) $(appenddimacs_okl) $(manipparam_okl) $(preprocesssplitting_okl) $(collectingpartialassignments_okl) $(collectingdecisions_okl) $(boundedtransversalsbv_okl) $(rankprimes_okl) $(grosswaldhagisformula_okl) $(greentao_okl) $(pdvanderwaerden_okl) $(countprogressionsgreentao_okl) $(pdextend_okl) $(solutionshift_okl) $(quinemccluskey_okl) $(quinemccluskeysubsumptionhypergraph_okl) $(quinemccluskeysubsumptionhypergraphwithfullstatistics_okl) $(quinemccluskeysubsumptionhypergraphfullstatistics_okl) $(vdw_minimumtransversals_okl) $(gt_minimumtransversals_okl) $(greentaocnf_okl) $(greentaogcnf_okl) $(greentaogen_okl) $(greentaodgen_okl) $(pdvanderwaerdencnf_okl) $(smusathorn_okl) $(musatd2_okl) $(oklib_call) $(runvdw3k_okl) $(runvdwk1k2_okl) $(runpdvdwk1k2_okl) $(crunpdvdwk1k2_okl) $(randomdestotalassignment_okl) $(applypass_okl) $(passextends_okl) $(passclashes_okl)  $(extractcertificatevdW2_okl) $(extractsolutionubcsat_okl) $(measurecpuspeed_okl) $(measurecpuspeedmem_okl) $(crunpdschur_okl) $(crunpdwschur_okl) $(crunpdschurfsb_okl) $(crunpdwschurfsb_okl) $(crunmschur_okl) $(crunwmschur_okl) $(tawsolver_okl) $(ttawsolver_okl) $(ctawsolver_okl) $(cttawsolver_okl)
