# Oliver Kullman, 20.2.2022 (Swansea
# Copyright 2022, 2023 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..


opt_progs = BCC2SAT BCCbySAT GraphGen CNF2cg CNF2statscg 2QCNF2aCNF CNFBCC QCNF2gCNF GCGeq Disassemble GSM CNFstats Graph2BCcompGraph MaxIndependentGreedy DirStatistics Graph2Matrix

popt_progs = $(addprefix p-, $(opt_progs))


test_progs_stem = Graphs Bicliques2SAT Generators ConflictGraphs Bicliques DimacsTools GraphTraversal Algorithms
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

BCCbySAT : BCCbySAT.cpp Bicliques2SAT.hpp BCC2SAT.hpp
BCCbySAT_debug : BCCbySAT.cpp Bicliques2SAT.hpp BCC2SAT.hpp

GraphGen : GraphGen.cpp Graphs.hpp Generators.hpp
GraphGen_debug : GraphGen.cpp Graphs.hpp Generators.hpp

CNF2cg : CNF2cg.cpp ConflictGraphs.hpp DimacsTools.hpp
CNF2cg_debug : CNF2cg.cpp ConflictGraphs.hpp DimacsTools.hpp

CNF2statscg : CNF2statscg.cpp ConflictGraphs.hpp DimacsTools.hpp GraphTraversal.hpp
CNF2statscg_debug : CNF2statscg.cpp ConflictGraphs.hpp DimacsTools.hpp GraphTraversal.hpp

2QCNF2aCNF : 2QCNF2aCNF.cpp DimacsTools.hpp
2QCNF2aCNF_debug : 2QCNF2aCNF.cpp DimacsTools.hpp

QCNF2gCNF : QCNF2gCNF.cpp DimacsTools.hpp
QCNF2gCNF_debug : QCNF2gCNF.cpp DimacsTools.hpp

CNFBCC : CNFBCC.cpp Bicliques2SAT.hpp BCC2SAT.hpp
CNFBCC_debug : CNFBCC.cpp Bicliques2SAT.hpp BCC2SAT.hpp

GCGeq : GCGeq.cpp Bicliques2SAT.hpp DimacsTools.hpp
GCGeq_debug : GCGeq.cpp Bicliques2SAT.hpp DimacsTools.hpp

Disassemble : Disassemble.cpp Disassemble.hpp DimacsTools.hpp Bicliques2SAT.hpp
Disassemble_debug : Disassemble.cpp Disassemble.hpp DimacsTools.hpp Bicliques2SAT.hpp

GSM : GSM.cpp Bicliques2SAT.hpp DimacsTools.hpp BCC2SAT.hpp
GSM_debug : GSM.cpp Bicliques2SAT.hpp DimacsTools.hpp BCC2SAT.hpp

CNFstats : CNFstats.cpp DimacsTools.hpp ConflictGraphs.hpp GraphTraversal.hpp
CNFstats_debug : CNFstats.cpp DimacsTools.hpp ConflictGraphs.hpp GraphTraversal.hpp

Graph2BCcompGraph : Graph2BCcompGraph.cpp Graphs.hpp Bicliques.hpp
Graph2BCcompGraph_debug : Graph2BCcompGraph.cpp Graphs.hpp Bicliques.hpp

MaxIndependentGreedy : MaxIndependentGreedy.cpp Graphs.hpp Bicliques2SAT.hpp BCC2SAT.hpp
MaxIndependentGreedy_debug : MaxIndependentGreedy.cpp Graphs.hpp Bicliques2SAT.hpp BCC2SAT.hpp

DirStatistics : DirStatistics.cpp DirStatistics.hpp
DirStatistics_debug : DirStatistics.cpp DirStatistics.hpp

Graph2Matrix : Graph2Matrix.cpp Graphs.hpp
Graph2Matrix_debug : Graph2Matrix.cpp Graphs.hpp

QDimacsSyntax : QDimacsSyntax.cpp QDimacsSyntax.hpp
QDimacsSyntax_debug : QDimacsSyntax.cpp QDimacsSyntax.hpp

# -------------------------

TestGraphs_debug : TestGraphs.cpp Graphs.hpp

TestBicliques2SAT_debug : TestBicliques2SAT.cpp Bicliques2SAT.hpp Bicliques.hpp

TestGenerators_debug : TestGenerators.cpp Graphs.hpp Generators.hpp ConflictGraphs.hpp

TestConflictGraphs_debug : TestConflictGraphs.cpp Graphs.hpp ConflictGraphs.hpp DimacsTools.hpp

TestBicliques_debug : TestBicliques.cpp Bicliques.hpp Graphs.hpp

TestDimacsTools_debug : TestDimacsTools.cpp DimacsTools.hpp

TestGraphTraversal_debug : TestGraphTraversal.cpp GraphTraversal.hpp Graphs.hpp Generators.hpp

TestAlgorithms_debug : TestAlgorithms.cpp Algorithms.hpp

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

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
