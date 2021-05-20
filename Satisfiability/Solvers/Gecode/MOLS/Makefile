# Oleg Zaikin, 20.5.2021 (Irkutsk)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..
okgecodeloc = $(oklibloc)/Satisfiability/Solvers/Gecode

opt_progs = 2mols

debug_progs = $(addsuffix _debug,$(opt_progs))

atps_debug = 2mols_debug
atps       = 2mols

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------



2mols : 2mols.cpp
2mols : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
2mols : Optimisation_plus = -fwhole-program
2mols : include_options += -I$(okgecodeloc)
2mols_debug : 2mols.cpp
2mols_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
2mols_debug : include_options += -I$(okgecodeloc)


.PHONY : $(atps) $(atps_debug)

regtest :

apptest : $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
