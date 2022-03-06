# Oliver Kullman, 20.2.2022 (Swansea
# Copyright 2022 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..


opt_progs = BCC2SAT GraphGen CNF2cg

popt_progs = $(addprefix p-, $(opt_progs))


test_progs_stem = Graphs Bicliques2SAT Generators ConflictGraphs Bicliques DimacsTools
test_progs = $(addprefix Test,$(test_progs_stem))

debug_progs = $(addsuffix _debug,$(opt_progs)) $(addsuffix _debug,$(test_progs))


atps_debug =
atps       =

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))


include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

BCC2SAT : BCC2SAT.cpp Bicliques2SAT.hpp BCC2SAT.hpp
BCC2SAT_debug : BCC2SAT.cpp Bicliques2SAT.hpp BCC2SAT.hpp

GraphGen : GraphGen.cpp Graphs.hpp Generators.hpp
GraphGen_debug : GraphGen.cpp Graphs.hpp Generators.hpp

CNF2cg : CNF2cg.cpp ConflictGraphs.hpp DimacsTools.hpp
CNF2cg_debug : CNF2cg.cpp ConflictGraphs.hpp DimacsTools.hpp

# -------------------------

TestGraphs_debug : TestGraphs.cpp Graphs.hpp

TestBicliques2SAT_debug : TestBicliques2SAT.cpp Bicliques2SAT.hpp Bicliques.hpp

TestGenerators_debug : TestGenerators.cpp Graphs.hpp Generators.hpp

TestConflictGraphs_debug : TestConflictGraphs.cpp Graphs.hpp ConflictGraphs.hpp DimacsTools.hpp

TestBicliques_debug : TestBicliques.cpp Bicliques.hpp Graphs.hpp

TestDimacsTools_debug : TestDimacsTools.cpp DimacsTools.hpp

# -------------------------

regtest_targets := $(addprefix Test,$(addsuffix debug, $(test_progs_stem)))

.PHONY : $(regtest_targets)

regtest : $(regtest_targets)

$(regtest_targets) : Test%debug : Test%_debug
	./$<


.PHONY : $(atps) $(atps_debug)

regtest :

apptest : $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
