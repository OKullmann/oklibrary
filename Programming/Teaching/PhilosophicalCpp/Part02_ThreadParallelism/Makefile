# Oliver Kullmann, 17.11.2018 (Swansea)
# Copyright 2018, 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Version: 0.5.1

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
Optimisation_plus = -fwhole-program -fstrict-aliasing -funroll-loops -fvariable-expansion-in-unroller -floop-nest-optimize -fgraphite-identity --param graphite-max-nb-scop-params=0
# Remark: -static yields the compiler emitting crashing code for programs Recursion, Sum with gcc version 7.3.0, 8.3.0
Optimisation_minus =
Optimisation_options = $(Optimisation_core) $(Optimisation_plus) $(Optimisation_minus)
numerics_plus =
numerics_minus = -fno-finite-math-only
numerics_options = $(numerics_plus) $(numerics_minus)
Debug_options = -g -D_GLIBCXX_DEBUG
# Options to reproduce compilation:
repr_macro = -DOPTIMISATION="\"$(LStandard_options) $(Optimisation_options) $(numerics_options) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)\""
reprdebug_macro = -DOPTIMISATION="\"$(LStandard_options) $(Warning_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)\""


programmingloc = ../../..
randgenloc = ../../../../Satisfiability/Transformers/Generators
include_options = -I $(programmingloc) -I $(randgenloc)


LDFLAGS += -pthread


opt_progs = AtomicCounter Recursion Sum
debug_progs =


all : programs test

programs : optimised debug

optimised : $(opt_progs)

$(opt_progs) :
	$(CXX) $(LStandard_options) $(Warning_options) $(include_options) $(Optimisation_options) $(numerics_options) $(special_macros) $(repr_macro) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@

debug : $(debug_progs)

$(debug_progs) :
	$(CXX) $(LStandard_options) $(Warning_options) $(include_options) $(Debug_options) $(special_macros) $(reprdebug_macro) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@


AtomicCounter : AtomicCounter.cpp

Recursion : Recursion.cpp

Sum : Sum.cpp


test : regtest apptest

regtest :

apptest :


clean :
	- rm $(debug_progs)

cleanall : clean
	- rm $(opt_progs)
