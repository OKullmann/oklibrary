# Oliver Kullmann, 2.3.2019 (Swansea)
# Copyright 2019 Oliver Kullmann
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
Optimisation_plus = -fwhole-program -static
Optimisation_minus =
Optimisation_options = $(Optimisation_core) $(Optimisation_plus) $(Optimisation_minus)
numerics_plus =
numerics_minus = -fno-finite-math-only
numerics_options = $(numerics_plus) $(numerics_minus)
Debug_options = -g -D_GLIBCXX_DEBUG
# Options to reproduce compilation:
repr_macro = -DOPTIMISATION="\"$(LStandard_options) $(Optimisation_options) $(numerics_options)\""

floatingpointloc = ../../../../Programming
include_options = -I $(floatingpointloc)

timing_stems = Discard Generation Bernoulli12 Bernoulli2 Initialisation ilogp2
timing_progs = $(addprefix Timing, $(timing_stems))

opt_progs = $(timing_progs)
debug_progs = Test


all : programs test

programs : optimised debug

optimised : $(opt_progs)

$(opt_progs) :
	$(CXX) $(LStandard_options) $(Warning_options) $(include_options) $(Optimisation_options) $(numerics_options) $(special_macros) $(repr_macro) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

debug : $(debug_progs)


TimingDiscard : TimingDiscard.cpp Numbers.hpp Environment.hpp

TimingGeneration : TimingGeneration.cpp Numbers.hpp Environment.hpp

TimingBernoulli12 : TimingBernoulli12.cpp Numbers.hpp Distributions.hpp Environment.hpp

TimingBernoulli2 : TimingBernoulli2.cpp Numbers.hpp Distributions.hpp Environment.hpp

TimingInitialisation : TimingInitialisation.cpp Numbers.hpp Environment.hpp

Timingilogp2 : Timingilogp2.cpp Numbers.hpp Environment.hpp


Test : Test.cpp Numbers.hpp Distributions.hpp Algorithms.hpp
	$(CXX) $(LStandard_options) $(Warning_options) $(include_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@


test : regtest apptest

regtest : Test
	./Test
apptest :


clean :
	- rm $(debug_progs)

cleanall : clean
	- rm $(opt_progs)
