# Oleg Zaikin, 6.4.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs =

test_progs_stem = Lookahead TreeOutput
test_progs = $(addprefix Test,$(test_progs_stem))

debug_progs = $(addsuffix _debug,$(opt_progs)) $(addsuffix _debug,$(test_progs))


atps_debug =

atps_debug := $(addprefix at_,$(atps_debug))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------



TestLookahead_debug : TestLookahead.cpp Lookahead.hpp Statistics.hpp Examples/Trivial.hpp
TestLookahead_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -pthread
TestLookahead_debug : Warning_options += -Wnon-virtual-dtor

TestTreeOutput_debug : TestTreeOutput.cpp TreeOutput.hpp
TestTreeOutput_debug : Warning_options += -Wnon-virtual-dtor


regtest_targets := $(addprefix Test,$(addsuffix debug, $(test_progs_stem)))

.PHONY : $(regtest_targets)

regtest : $(regtest_targets)

$(regtest_targets) : Test%debug : Test%_debug
	./$<



.PHONY : $(atps_debug)

apptest : $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*
