# Oliver Kullman, 6.4.2019 (Swansea
# Copyright 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

timing_stems = Discard Generation Bernoulli12 Bernoulli2 Bernoulli Initialisation ilogp2
timing_progs = $(addprefix Timing, $(timing_stems))

opt_progs = $(timing_progs)
debug_progs = Test_debug TestNumbers_debug TestDistributions_debug TestAlgorithms_debug TestClauseSets_debug TimingBernoulli12_debug TimingBernoulli2_debug TimingBernoulli_debug # $(addsuffix _debug, $(timing_stems))

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

TimingInitialisation : TimingInitialisation.cpp Numbers.hpp

Timingilogp2 : Timingilogp2.cpp Numbers.hpp


Test_debug : Test.cpp Numbers.hpp Distributions.hpp Algorithms.hpp Tests.hpp
TestNumbers_debug : TestNumbers.cpp Numbers.hpp
TestDistributions_debug : TestDistributions.cpp Numbers.hpp Distributions.hpp
TestAlgorithms_debug : TestAlgorithms.cpp Numbers.hpp Distributions.hpp Algorithms.hpp
TestClauseSets_debug : TestClauseSets.cpp ClauseSets.hpp


.PHONY : Testdebug TestNumbersdebug TestDistributionsdebug TestAlgorithmsdebug TestClauseSetsdebug

regtest : Testdebug TestNumbersdebug TestDistributionsdebug TestAlgorithmsdebug TestClauseSetsdebug

Testdebug : Test_debug
	./$<

TestNumbersdebug : TestNumbers_debug
	./$<

TestDistributionsdebug : TestDistributions_debug
	./$<

TestAlgorithmsdebug : TestAlgorithms_debug
	./$<

TestClauseSetsdebug : TestClauseSets_debug
	./$<


apptest : fuzzing


.PHONY: runBernoulli12 runBernoulli2 runBernoulli

fuzzing : runBernoulli12 runBernoulli2 runBernoulli

runBernoulli12 : TimingBernoulli12_debug
	${testtool} ./$< 2,d 1e5 22 1 2 3 > /dev/null
	${testtool} ./$< 2,e 1e5 > /dev/null

runBernoulli2 : TimingBernoulli2_debug
	${testtool} ./$< e 1e5 > /dev/null
	${testtool} ./$< d 1e5 20 10 4 555 88 > /dev/null

runBernoulli : TimingBernoulli_debug
	${testtool} ./$< e 1e5 > /dev/null
	${testtool} ./$< d 1e5 10/1048576 4 555 88 > /dev/null
