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
Optimisation_options = -Ofast -DNDEBUG -march=native -fwhole-program -funsafe-loop-optimizations
Debug_options = -g -D_GLIBCXX_DEBUG

SETN=-DNN=16


opt_progs = qcount qcount_ct wqcount pqcount
debug_progs = $(addsuffix _debug, $(opt_progs))


all : programs test

programs : optimised debug

optimised : $(opt_progs)

debug : $(debug_progs)


qcount : NQueens.cpp
	$(CXX) $(LStandard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

qcount_debug : NQueens.cpp
	$(CXX) $(LStandard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

qcount_ct : NQueens_ct.cpp
	$(CXX) $(SETN) $(LStandard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

qcount_ct_debug : NQueens_ct.cpp
	$(CXX) $(SETN) $(LStandard_options) $(Warning_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

wqcount : NQueens_w.cpp
	$(CXX) $(LStandard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

wqcount_debug : NQueens_w.cpp
	$(CXX) $(LStandard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

pqcount : NQueenspar.cpp
	$(CXX) $(LStandard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -pthread $< -o $@

pqcount_debug : NQueenspar.cpp
	$(CXX) $(LStandard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -pthread $< -o $@


test : regtest apptest

regtest :

apptest :


clean :
	- rm $(debug_progs)

cleanall : clean
	- rm $(opt_progs)
