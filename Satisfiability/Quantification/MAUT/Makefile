# Oliver Kullman, 28.4.2021 (Swansea
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = RPL_trees
debug_progs =

test_targets = Test

debug_progs := $(addsuffix _debug,$(opt_progs)) $(addsuffix _debug,$(debug_progs)) $(addsuffix _debug,$(test_targets))
test_targets := $(addsuffix debug,$(test_targets))


.PHONY : $(test_targets)

atps       = RPL_trees
atps_debug = $(addsuffix _debug,$(atps))

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

.PHONY : $(atps) $(atps_debug)

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

RPL_trees : RPL_trees.cpp RPL_trees.hpp VarLit.hpp ClauseSets.hpp Parsing.hpp PartialAssignments.hpp
RPL_trees_debug : RPL_trees.cpp RPL_trees.hpp VarLit.hpp ClauseSets.hpp Parsing.hpp PartialAssignments.hpp


Test_debug : Test.cpp RPL_trees.hpp VarLit.hpp ClauseSets.hpp Parsing.hpp PartialAssignments.hpp

regtest : $(test_targets)

$(test_targets) : %debug : %_debug
	./$<


apptest :  $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
