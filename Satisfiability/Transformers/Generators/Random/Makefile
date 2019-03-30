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
repr_macro = -DOPTIMISATION="\"$(LStandard_options) $(Optimisation_options) $(numerics_options) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)\""
reprdebug_macro = -DOPTIMISATION="\"$(LStandard_options) $(Warning_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)\""


programmingloc = ../../../../Programming
include_options = -I $(programmingloc)

timing_stems = Discard Generation Bernoulli12 Bernoulli2 Initialisation ilogp2
timing_progs = $(addprefix Timing, $(timing_stems))

opt_progs = $(timing_progs)
debug_progs = Test_debug TimingBernoulli12_debug # $(addsuffix _debug, $(timing_stems))

popt_progs = $(addprefix p-, $(opt_progs))


all : programs test

programs : optimised debug

optimised : $(opt_progs)

$(opt_progs) :
	$(CXX) $(LStandard_options) $(Warning_options) $(include_options) $(Optimisation_options) $(numerics_options) $(special_macros) $(repr_macro) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@

debug : $(debug_progs)

$(debug_progs) :
	$(CXX) $(LStandard_options) $(Warning_options) $(include_options) $(Debug_options) $(special_macros) $(reprdebug_macro) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< -o $@

$(popt_progs) : p-% :
	- rm $*
	make CXXFLAGS="-fprofile-generate" $*
	./$* -p
	rm $*
	make CXXFLAGS="-fprofile-use" $*


TimingDiscard : TimingDiscard.cpp Numbers.hpp

TimingGeneration : TimingGeneration.cpp Numbers.hpp

TimingBernoulli12 : TimingBernoulli12.cpp Numbers.hpp Distributions.hpp Tests.hpp
TimingBernoulli12_debug : TimingBernoulli12.cpp Numbers.hpp Distributions.hpp Tests.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TimingBernoulli2 : TimingBernoulli2.cpp Numbers.hpp Distributions.hpp Tests.hpp

TimingInitialisation : TimingInitialisation.cpp Numbers.hpp

Timingilogp2 : Timingilogp2.cpp Numbers.hpp


Test_debug : Test.cpp Numbers.hpp Distributions.hpp Algorithms.hpp Tests.hpp


test : regtest apptest

regtest : Test_debug
	./Test_debug
apptest :


clean :
	- rm $(debug_progs)

cleanall : clean
	- rm $(opt_progs)
