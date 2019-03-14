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

CXX = g++

Standard_options = --std=c++17 -pedantic
Warning_options = -fmax-errors=5 -Wall -Wextra
Optimisation_options = -Ofast -DNDEBUG -march=native -fwhole-program -static
numerics_options = -fno-finite-math-only
Debug_options = -g -D_GLIBCXX_DEBUG
opt_macro = -DOPTIMISATION="\"$(Optimisation_options) $(numerics_options)\""

all : programs test

programs : optimised debug

opt_progs = TimingDiscard TimingGeneration TimingBernoulli12 TimingBernoulli
optimised : $(opt_progs)

debug_progs = Test
debug : $(debug_progs)

floatingpointloc = ../../../../Programming
include_options = -I $(floatingpointloc)


TimingDiscard : TimingDiscard.cpp Numbers.hpp Environment.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(include_options) $(Optimisation_options) $(numerics_options) $(git_macro) $(opt_macro) $(CPPFLAGS) $(CXXFLAGS) $< -o $@
TimingGeneration : TimingGeneration.cpp Numbers.hpp Environment.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(include_options) $(Optimisation_options) $(numerics_options) $(git_macro) $(opt_macro) $(CPPFLAGS) $(CXXFLAGS) $< -o $@
TimingBernoulli12 : TimingBernoulli12.cpp Numbers.hpp Environment.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(include_options) $(Optimisation_options) $(numerics_options) $(git_macro) $(opt_macro) $(CPPFLAGS) $(CXXFLAGS) $< -o $@
TimingBernoulli : TimingBernoulli.cpp Numbers.hpp Environment.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(include_options) $(Optimisation_options) $(numerics_options) $(git_macro) $(opt_macro) $(CPPFLAGS) $(CXXFLAGS) $< -o $@


Test : Test.cpp Numbers.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(include_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@


test : regtest apptest

regtest : Test
	./Test
apptest :


clean :
	- rm $(debug_progs)

cleanall : clean
	- rm $(opt_progs)
