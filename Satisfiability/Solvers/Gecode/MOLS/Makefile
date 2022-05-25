# Oleg Zaikin, 20.5.2021 (Irkutsk)
# Copyright 2021, 2022 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

opt_progs = Euler Euler_BBOpt laMols gcMols rlaMols

test_progs_stem = Parsing Conditions Encoding Constraints Solvers PartialSquares CommandLine LookaheadReduction LookaheadBranching Verification OrthogonalArrays BasicLatinSquares GcVariables Cases
test_progs = $(addprefix Test,$(test_progs_stem))

debug_progs = $(addsuffix _debug,$(opt_progs)) $(addsuffix _debug,$(test_progs))

atps_debug = Euler_debug
atps       = Euler

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

gist ?= 0
gist_option = -DGIST=$(gist)

# -------------------------

Euler : Optimisation_plus = -fwhole-program # needed by Gecode to disable -static 
Euler : Euler.cpp Euler.hpp ../Lookahead.hpp ../Statistics.hpp Options.hpp
Euler : CPPFLAGS += $(gist_option)
Euler : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
ifeq ($(gist),1)
Euler : LDFLAGS += -lgecodegist
endif
Euler_debug : Euler.cpp Euler.hpp ../Lookahead.hpp ../Statistics.hpp Options.hpp
Euler_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Euler_debug : Warning_options += -Wnon-virtual-dtor


Euler_BBOpt : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Euler_BBOpt : Optimisation_plus = -fwhole-program
Euler_BBOpt : Euler_BBOpt.cpp Euler.hpp ../Lookahead.hpp ../Statistics.hpp Options.hpp
Euler_BBOpt_debug : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Euler_BBOpt_debug : Euler_BBOpt.cpp Euler.hpp ../Lookahead.hpp ../Statistics.hpp Options.hpp


gcMols : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
gcMols : Optimisation_plus = -fwhole-program
gcMols : gcMols.cpp Parsing.hpp Conditions.hpp Encoding.hpp Constraints.hpp Solvers.hpp PartialSquares.hpp CommandLine.hpp Options.hpp Verification.hpp
gcMols_debug : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
gcMols_debug : gcMols.cpp Parsing.hpp Conditions.hpp Encoding.hpp Constraints.hpp Solvers.hpp PartialSquares.hpp CommandLine.hpp Options.hpp Verification.hpp


rlaMols : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
rlaMols : Optimisation_plus = -fwhole-program
rlaMols : rlaMols.cpp Parsing.hpp Conditions.hpp Encoding.hpp Constraints.hpp Solvers.hpp PartialSquares.hpp CommandLine.hpp Options.hpp LookaheadBranching.hpp LookaheadReduction.hpp Verification.hpp GcVariables.hpp
rlaMols_debug : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
rlaMols_debug : rlaMols.cpp Parsing.hpp Conditions.hpp Encoding.hpp Constraints.hpp Solvers.hpp PartialSquares.hpp CommandLine.hpp Options.hpp LookaheadBranching.hpp LookaheadReduction.hpp Verification.hpp GcVariables.hpp


laMols : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
laMols : Optimisation_plus = -fwhole-program
laMols : laMols.cpp Parsing.hpp Conditions.hpp Encoding.hpp Constraints.hpp Solvers.hpp PartialSquares.hpp CommandLine.hpp Options.hpp LookaheadBranching.hpp LookaheadReduction.hpp Verification.hpp GcVariables.hpp
laMols_debug : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
laMols_debug : laMols.cpp Parsing.hpp Conditions.hpp Encoding.hpp Constraints.hpp Solvers.hpp PartialSquares.hpp CommandLine.hpp Options.hpp LookaheadBranching.hpp LookaheadReduction.hpp Verification.hpp GcVariables.hpp

# -------------------------

TestParsing_debug : TestParsing.cpp Parsing.hpp Conditions.hpp

TestConditions_debug : TestConditions.cpp Conditions.hpp Parsing.hpp

TestEncoding_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport -lgecodesearch
TestEncoding_debug : TestEncoding.cpp Encoding.hpp Conditions.hpp  PartialSquares.hpp Verification.hpp BasicLatinSquares.hpp

TestConstraints_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport
TestConstraints_debug : TestConstraints.cpp Constraints.hpp Encoding.hpp Conditions.hpp

TestSolvers_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport -lgecodesearch
TestSolvers_debug : Optimisation_plus = -fwhole-program
TestSolvers_debug : TestSolvers.cpp Solvers.hpp Constraints.hpp Encoding.hpp Conditions.hpp LookaheadBranching.hpp LookaheadReduction.hpp BasicLatinSquares.hpp Verification.hpp

TestPartialSquares_debug : TestPartialSquares.cpp PartialSquares.hpp Conditions.hpp

TestCommandLine_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport
TestCommandLine_debug : Optimisation_plus = -fwhole-program
TestCommandLine_debug : TestCommandLine.cpp CommandLine.hpp Conditions.hpp

TestLookaheadReduction_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport -lgecodesearch
TestLookaheadReduction_debug : Optimisation_plus = -fwhole-program
TestLookaheadReduction_debug : TestLookaheadReduction.cpp LookaheadReduction.hpp Cases.hpp Options.hpp Solvers.hpp GcVariables.hpp

TestLookaheadBranching_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport
TestLookaheadBranching_debug : Optimisation_plus = -fwhole-program
TestLookaheadBranching_debug : TestLookaheadBranching.cpp LookaheadBranching.hpp LookaheadReduction.hpp Conditions.hpp Options.hpp

TestGcVariables_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport
TestGcVariables_debug : Optimisation_plus = -fwhole-program
TestGcVariables_debug : TestGcVariables.cpp GcVariables.hpp

TestVerification_debug : TestVerification.cpp Verification.hpp BasicLatinSquares.hpp

TestOrthogonalArrays_debug : TestOrthogonalArrays.cpp OrthogonalArrays.hpp BasicLatinSquares.hpp

TestBasicLatinSquares_debug : TestBasicLatinSquares.cpp BasicLatinSquares.hpp

TestCases_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport
TestCases_debug : TestCases.cpp Options.hpp Cases.hpp

# -------------------------

regtest_targets := $(addprefix Test,$(addsuffix debug, $(test_progs_stem)))

.PHONY : $(regtest_targets)

regtest : $(regtest_targets)

$(regtest_targets) : Test%debug : Test%_debug
	./$<


.PHONY : $(atps) $(atps_debug)

apptest : $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*
