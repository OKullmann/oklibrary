# Oliver Kullman, 28.4.2021 (Swansea
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = RPL_trees_64 RPL_trees_80 RPL_trees_mpz
debug_progs =

test_targets = Test

debug_progs := $(addsuffix _debug,$(opt_progs)) $(addsuffix _debug,$(debug_progs)) $(addsuffix _debug,$(test_targets))
test_targets := $(addsuffix debug,$(test_targets))


.PHONY : $(test_targets)

atps       = $(opt_progs)
atps_debug = $(addsuffix _debug,$(atps))

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

.PHONY : $(atps) $(atps_debug)

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

dep_RPL_trees := RPL_trees.cpp RPL_trees.hpp VarLit.hpp ClauseSets.hpp Parsing.hpp PartialAssignments.hpp Backtracking.hpp Counting.hpp

RPL_trees_64 : $(dep_RPL_trees)
RPL_trees_64 : LDFLAGS += -lgmp
RPL_trees_64 : CPPFLAGS += -DCOUNTTYPE=Count64
RPL_trees_64_debug : $(dep_RPL_trees)
RPL_trees_64_debug : LDFLAGS += -lgmp
RPL_trees_64_debug : CPPFLAGS += -DCOUNTTYPE=Count64

RPL_trees_80 : $(dep_RPL_trees)
RPL_trees_80 : LDFLAGS += -lgmp
RPL_trees_80 : CPPFLAGS += -DCOUNTTYPE=Count80
RPL_trees_80_debug : $(dep_RPL_trees)
RPL_trees_80_debug : LDFLAGS += -lgmp
RPL_trees_80_debug : CPPFLAGS += -DCOUNTTYPE=Count80

RPL_trees_mpz : $(dep_RPL_trees)
RPL_trees_mpz : LDFLAGS += -lgmp
RPL_trees_mpz : CPPFLAGS += -DCOUNTTYPE=Count_mpz
RPL_trees_mpz_debug : $(dep_RPL_trees)
RPL_trees_mpz_debug : LDFLAGS += -lgmp
RPL_trees_mpz_debug : CPPFLAGS += -DCOUNTTYPE=Count_mpz


Test_debug : Test.cpp RPL_trees.hpp VarLit.hpp ClauseSets.hpp Parsing.hpp PartialAssignments.hpp Backtracking.hpp Counting.hpp
Test_debug : LDFLAGS += -lgmp


regtest : $(test_targets)

$(test_targets) : %debug : %_debug
	./$<


apptest :  $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
