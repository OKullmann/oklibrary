# Oleg Zaikin, 20.5.2021 (Irkutsk)
# Copyright 2021, 2022 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

opt_progs = Euler Euler_BBOpt Mols

test_progs_stem = Parsing Conditions Encoding Constraints Solvers PartialSquares
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


Mols : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Mols : Optimisation_plus = -fwhole-program
Mols : Mols.cpp Conditions.hpp Parsing.hpp Solvers.hpp Options.hpp
Mols_debug : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Mols_debug : Optimisation_plus = -fwhole-program
Mols_debug : Mols.cpp Conditions.hpp Parsing.hpp Solvers.hpp Options.hpp

# -------------------------

TestParsing_debug : TestParsing.cpp Parsing.hpp Conditions.hpp

TestConditions_debug : TestConditions.cpp Conditions.hpp Parsing.hpp

TestEncoding_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport -lgecodesearch
TestEncoding_debug : TestEncoding.cpp Encoding.hpp Conditions.hpp

TestConstraints_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport
TestConstraints_debug : TestConstraints.cpp Constraints.hpp Encoding.hpp Conditions.hpp

TestSolvers_debug : Optimisation_plus = -fwhole-program
TestSolvers_debug : LDFLAGS += -lgecodeint -lgecodekernel -lgecodesupport -lgecodesearch
TestSolvers_debug : TestSolvers.cpp Solvers.hpp Constraints.hpp Encoding.hpp Conditions.hpp

TestPartialSquares_debug : TestPartialSquares.cpp PartialSquares.hpp Conditions.hpp

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
