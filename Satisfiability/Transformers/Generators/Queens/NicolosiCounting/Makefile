# Oliver Kullmann, 25.5.2018 (Swansea)
# Copyright 2018, 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all programs optimised debug test regtest apptest clean cleanall


git_id = $(shell git rev-parse HEAD)
git_macro = -DGITID="\"$(git_id)\""
machine_name = $(shell hostname)
machine_macro = -DMACHINE="\"$(machine_name)\""
bogomips_value = $(shell grep -m 1 "bogomips" /proc/cpuinfo | cut -f2 | cut -d' ' -f2)
bogomips_macro = -DBOGOMIPS=$(bogomips_value)

special_macros = $(machine_macro) $(bogomips_macro) $(git_macro)


CXX = g++

LStandard_options = --std=c++17 -pedantic
Warning_options = -fmax-errors=5 -Wall -Wextra
Optimisation_core = -Ofast -DNDEBUG -march=native
# "plus" means extensions, "minus" means restriction:
Optimisation_plus = -fwhole-program -funsafe-loop-optimizations
Optimisation_minus =
Optimisation_options = $(Optimisation_core) $(Optimisation_plus) $(Optimisation_minus)
Debug_options = -g -D_GLIBCXX_DEBUG
# Options to reproduce compilation:
repr_macro = -DOPTIMISATION="\"$(LStandard_options) $(Optimisation_options) $(target_specifics)\""


opt_progs = qcount qcount_ct wqcount pqcount
debug_progs = $(addsuffix _debug, $(opt_progs))


all : programs test

programs : optimised debug

optimised : $(opt_progs)

$(opt_progs) :
	$(CXX) $(LStandard_options) $(Warning_options) $(Optimisation_options) $(special_macros) $(repr_macro) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@

debug : $(debug_progs)

$(debug_progs) :
	$(CXX) $(LStandard_options) $(Warning_options) $(Debug_options) $(special_macros) $(repr_macro) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@


qcount : Warning_options += -Wno-parentheses
qcount : NQueens.cpp

qcount_debug : Warning_options += -Wno-parentheses
qcount_debug : NQueens.cpp


SETN=-DNN=16

qcount_ct : CXXFLAGS+=$(SETN)
qcount_ct : target_specifics=$(SETN)
qcount_ct : NQueens_ct.cpp

qcount_ct_debug : CXXFLAGS += $(SETN)
qcount_ct_debug : target_specifics=$(SETN)
qcount_ct_debug : NQueens_ct.cpp


wqcount : Warning_options += -Wno-parentheses
wqcount : NQueens_w.cpp

wqcount_debug : Warning_options += -Wno-parentheses
wqcount_debug : NQueens_w.cpp


pqcount : Warning_options += -Wno-parentheses
pqcount : LDFLAGS += -pthread
pqcount : NQueenspar.cpp

pqcount_debug : Warning_options += -Wno-parentheses
pqcount_debug : LDFLAGS += -pthread
pqcount_debug : NQueenspar.cpp


test : regtest apptest

regtest :

apptest :


clean :
	- rm $(debug_progs)

cleanall : clean
	- rm $(opt_progs)
