# Oliver Kullman, 6.4.2019 (Swansea
# Copyright 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

timing_stems = Discard Generation Bernoulli12 Bernoulli2 Bernoulli UniformRange Choose Kolmogorov Initialisation ilogp2
timing_progs = $(addprefix Timing, $(timing_stems))

opt_progs = $(timing_progs) Kval
debug_progs = TestNumbers_debug TestDistributions_debug TestAlgorithms_debug TestClauseSets_debug TestTests_debug TimingBernoulli12_debug TimingBernoulli2_debug TimingBernoulli_debug TimingUniformRange_debug TimingChoose_debug TimingKolmogorov_debug # $(addsuffix _debug, $(timing_stems))

popt_progs = $(addprefix p-, $(opt_progs))

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

TimingKolmogorov : TimingKolmogorov.cpp Numbers.hpp Tests.hpp
TimingKolmogorov_debug : TimingKolmogorov.cpp Numbers.hpp Tests.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TimingInitialisation : TimingInitialisation.cpp Numbers.hpp

Timingilogp2 : Timingilogp2.cpp Numbers.hpp

Kval : k.c


TestNumbers_debug : TestNumbers.cpp Numbers.hpp
TestDistributions_debug : TestDistributions.cpp Numbers.hpp Distributions.hpp
TestAlgorithms_debug : TestAlgorithms.cpp Numbers.hpp Distributions.hpp Algorithms.hpp
TestClauseSets_debug : TestClauseSets.cpp ClauseSets.hpp
TestTests_debug : TestTests.cpp Tests.hpp $(programmingloc)/Numerics/FloatingPoint.hpp


.PHONY : TestNumbersdebug TestDistributionsdebug TestAlgorithmsdebug TestClauseSetsdebug TestTestsdebug

regtest : TestNumbersdebug TestDistributionsdebug TestAlgorithmsdebug TestClauseSetsdebug TestTestsdebug

TestNumbersdebug : TestNumbers_debug
	./$<

TestDistributionsdebug : TestDistributions_debug
	./$<

TestAlgorithmsdebug : TestAlgorithms_debug
	./$<

TestClauseSetsdebug : TestClauseSets_debug
	./$<

TestTestsdebug : TestTests_debug
	./$<


apptest : fuzzing


.PHONY: runBernoulli12 runBernoulli2 runBernoulli runUniformRange runChoose

fuzzing : runBernoulli12 runBernoulli2 runBernoulli runUniformRange runChoose

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
