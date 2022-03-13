# Oliver Kullman, 6.4.2019 (Swansea
# Copyright 2019, 2020, 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# TODOS:
# 1. Update to new standard (more automation).

oklibloc = ../../../..

timing_stems = Discard Generation Bernoulli12 Bernoulli2 Bernoulli UniformRange Choose Kolmogorov Initialisation ilogp2 LSRG
timing_progs = $(addprefix Timing, $(timing_stems))

opt_progs = $(timing_progs) Kval KSTval MetaBernoulli MetaDevice BRG QBRG DQBRG LSRG
debug_progs = TestNumbers_debug TestDistributions_debug TestFPDistributions_debug TestAlgorithms_debug TestClauseSets_debug TestQClauseSets_debug TestDQClauseSets_debug TestTests_debug TestSequences_debug TestLatinSquares_debug TestLSRG_debug  TimingBernoulli12_debug TimingBernoulli2_debug TimingBernoulli_debug TimingUniformRange_debug TimingChoose_debug TimingKolmogorov_debug TimingLSRG_debug MetaBernoulli_debug MetaDevice_debug  BRG_debug QBRG_debug DQBRG_debug LSRG_debug # $(addsuffix _debug, $(timing_stems))

popt_progs = $(addprefix p-, $(opt_progs))

atps       = LSRG
atps_debug = $(addsuffix _debug,$(atps))

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

.PHONY : $(atps) $(atps_debug)

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

TimingDiscard : TimingDiscard.cpp Numbers.hpp

TimingGeneration : TimingGeneration.cpp Numbers.hpp

TimingBernoulli12 : numerics_minus += -fno-fast-math
TimingBernoulli12 : TimingBernoulli12.cpp Numbers.hpp Distributions.hpp Tests.hpp
TimingBernoulli12_debug : TimingBernoulli12.cpp Numbers.hpp Distributions.hpp Tests.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TimingBernoulli2 : TimingBernoulli2.cpp Numbers.hpp Distributions.hpp Tests.hpp
TimingBernoulli2_debug : TimingBernoulli2.cpp Numbers.hpp Distributions.hpp Tests.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TimingBernoulli : TimingBernoulli.cpp Numbers.hpp Distributions.hpp Tests.hpp
TimingBernoulli_debug : TimingBernoulli.cpp Numbers.hpp Distributions.hpp Tests.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TimingUniformRange : TimingUniformRange.cpp Numbers.hpp Distributions.hpp Tests.hpp
TimingUniformRange_debug : TimingUniformRange.cpp Numbers.hpp Distributions.hpp Tests.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TimingChoose : TimingChoose.cpp Numbers.hpp Distributions.hpp Algorithms.hpp Tests.hpp
TimingChoose_debug : TimingChoose.cpp Numbers.hpp Distributions.hpp Algorithms.hpp Tests.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TimingKolmogorov : TimingKolmogorov.cpp Numbers.hpp Tests.hpp KolmogorovSmirnov.hpp
TimingKolmogorov_debug : TimingKolmogorov.cpp Numbers.hpp Tests.hpp KolmogorovSmirnov.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TimingInitialisation : TimingInitialisation.cpp Numbers.hpp

Timingilogp2 : Timingilogp2.cpp Numbers.hpp

TimingLSRG : TimingLSRG.cpp LSRG.hpp LatinSquares.hpp Numbers.hpp SeedOrganisation.hpp Distributions.hpp Tests.hpp KolmogorovSmirnov.hpp
TimingLSRG_debug : TimingLSRG.cpp LSRG.hpp LatinSquares.hpp Numbers.hpp SeedOrganisation.hpp Distributions.hpp Tests.hpp KolmogorovSmirnov.hpp

Kval : k.c
KSTval : KolmogorovSmirnovDist.c KolmogorovSmirnovDist.h

MetaBernoulli : Optimisation_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
MetaBernoulli : Optimisation_plus := $(filter-out -static,$(Optimisation_plus))
MetaBernoulli : Optimisation_plus += -fno-signed-zeros -fno-math-errno -fno-trapping-math
MetaBernoulli : LDFLAGS += -pthread
MetaBernoulli : MetaBernoulli.cpp Numbers.hpp Distributions.hpp Tests.hpp KolmogorovSmirnov.hpp
MetaBernoulli_debug : LDFLAGS += -pthread
MetaBernoulli_debug : MetaBernoulli.cpp Numbers.hpp Distributions.hpp Tests.hpp KolmogorovSmirnov.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

MetaDevice : Optimisation_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
MetaDevice : Optimisation_plus += -fno-signed-zeros -fno-math-errno -fno-trapping-math
MetaDevice : MetaDevice.cpp Numbers.hpp Distributions.hpp Tests.hpp
MetaDevice_debug : MetaDevice.cpp Numbers.hpp Distributions.hpp Tests.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

BRG : BRG.cpp ClauseSets.hpp VarLit.hpp Numbers.hpp Distributions.hpp SeedOrganisation.hpp $(programmingloc)/ProgramOptions/Environment.hpp
BRG_debug : BRG.cpp ClauseSets.hpp VarLit.hpp Numbers.hpp Distributions.hpp SeedOrganisation.hpp $(programmingloc)/ProgramOptions/Environment.hpp

QBRG : QBRG.cpp QClauseSets.hpp ClauseSets.hpp VarLit.hpp Numbers.hpp Distributions.hpp SeedOrganisation.hpp $(programmingloc)/ProgramOptions/Environment.hpp
QBRG_debug : QBRG.cpp QClauseSets.hpp ClauseSets.hpp VarLit.hpp Numbers.hpp Distributions.hpp SeedOrganisation.hpp $(programmingloc)/ProgramOptions/Environment.hpp

DQBRG : DQBRG.cpp DQClauseSets.hpp QClauseSets.hpp ClauseSets.hpp VarLit.hpp Numbers.hpp Distributions.hpp Algorithms.hpp SeedOrganisation.hpp $(programmingloc)/ProgramOptions/Environment.hpp
DQBRG_debug : DQBRG.cpp DQClauseSets.hpp QClauseSets.hpp ClauseSets.hpp VarLit.hpp Numbers.hpp Distributions.hpp Algorithms.hpp SeedOrganisation.hpp $(programmingloc)/ProgramOptions/Environment.hpp

LSRG : LSRG.cpp LSRG.hpp Numbers.hpp LatinSquares.hpp Distributions.hpp ClauseSets.hpp VarLit.hpp SeedOrganisation.hpp $(programmingloc)/ProgramOptions/Environment.hpp
LSRG_debug : LSRG.cpp LSRG.hpp Numbers.hpp LatinSquares.hpp Distributions.hpp ClauseSets.hpp VarLit.hpp SeedOrganisation.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TestNumbers_debug : TestNumbers.cpp Numbers.hpp
TestDistributions_debug : TestDistributions.cpp Numbers.hpp Distributions.hpp
TestFPDistributions_debug : TestFPDistributions.cpp Numbers.hpp FPDistributions.hpp
TestAlgorithms_debug : TestAlgorithms.cpp Numbers.hpp Distributions.hpp Algorithms.hpp
TestClauseSets_debug : TestClauseSets.cpp ClauseSets.hpp VarLit.hpp Numbers.hpp Distributions.hpp VarLit.hpp $(programmingloc)/ProgramOptions/Environment.hpp
TestQClauseSets_debug : TestQClauseSets.cpp QClauseSets.hpp
TestDQClauseSets_debug : TestDQClauseSets.cpp DQClauseSets.hpp
TestTests_debug : TestTests.cpp Tests.hpp KolmogorovSmirnov.hpp $(programmingloc)/Numerics/FloatingPoint.hpp
TestSequences_debug : TestSequences.cpp Sequences.hpp
TestLatinSquares_debug : TestLatinSquares.cpp LatinSquares.hpp Numbers.hpp  Algorithms.hpp Distributions.hpp ClauseSets.hpp VarLit.hpp SeedOrganisation.hpp
TestLSRG_debug : TestLSRG.cpp LSRG.cpp LSRG.hpp Numbers.hpp LatinSquares.hpp Distributions.hpp ClauseSets.hpp VarLit.hpp SeedOrganisation.hpp $(programmingloc)/ProgramOptions/Environment.hpp

.PHONY : TestNumbersdebug TestDistributionsdebug TestFPDistributionsdebug TestAlgorithmsdebug TestClauseSetsdebug TestQClauseSetsdebug TestDQClauseSetsdebug TestTestsdebug TestSequencesdebug TestLatinSquaresdebug TestLSRGdebug

regtest : TestNumbersdebug TestDistributionsdebug TestFPDistributionsdebug TestAlgorithmsdebug TestClauseSetsdebug TestQClauseSetsdebug TestDQClauseSetsdebug TestTestsdebug TestSequencesdebug TestLatinSquaresdebug TestLSRGdebug

TestNumbersdebug : TestNumbers_debug
	./$<

TestDistributionsdebug : TestDistributions_debug
	./$<

TestFPDistributionsdebug : TestFPDistributions_debug
	./$<

TestAlgorithmsdebug : TestAlgorithms_debug
	./$<

TestClauseSetsdebug : TestClauseSets_debug
	./$<

TestQClauseSetsdebug : TestQClauseSets_debug
	./$<

TestDQClauseSetsdebug : TestDQClauseSets_debug
	./$<

TestTestsdebug : TestTests_debug
	./$<

TestSequencesdebug : TestSequences_debug
	./$<

TestLatinSquaresdebug : TestLatinSquares_debug
	./$<

TestLSRGdebug : TestLSRG_debug
	./$<


apptest :  $(atps) $(atps_debug) fuzzing


$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*

.PHONY: runBernoulli12 runBernoulli2 runBernoulli runUniformRange runChoose runMetaBernoulli

fuzzing : runBernoulli12 runBernoulli2 runBernoulli runUniformRange runChoose runMetaBernoulli

runBernoulli12 : TimingBernoulli12_debug
	${testtool} ./$< 2,d 1e5 22 1 2 3 > /dev/null
	${testtool} ./$< 2,e 1e5 > /dev/null

runBernoulli2 : TimingBernoulli2_debug
	${testtool} ./$< e 1e5 > /dev/null
	${testtool} ./$< d 1e5 20 10 4 555 88 > /dev/null

runBernoulli : TimingBernoulli_debug
	${testtool} ./$< e 1e5 > /dev/null
	${testtool} ./$< d 1e5 10/1048576 4 555 88 > /dev/null

runUniformRange : TimingUniformRange_debug
	${testtool} ./$< 1e4 10 10 > /dev/null

runChoose : TimingChoose_debug
	${testtool} ./$< 1e3 10 4 3 > /dev/null

runMetaBernoulli : MetaBernoulli_debug
	${testtool} ./$< 1e2 1e1 1e1 1/3 > /dev/null
