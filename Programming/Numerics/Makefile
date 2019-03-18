# Oliver Kullmann, 3.3.2019 (Swansea)
# Copyright 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all programs optimised debug test regtest apptest clean cleanall

CXX = g++


git_id = $(shell git rev-parse HEAD)
git_macro = -DGITID="\"$(git_id)\""
machine_name = $(shell hostname)
machine_macro = -DMACHINE="\"$(machine_name)\""
bogomips_value = $(shell grep -m 1 "bogomips" /proc/cpuinfo | cut -f2 | cut -d' ' -f2)
bogomips_macro = -DBOGOMIPS=$(bogomips_value)

special_macros = $(machine_macro) $(bogomips_macro) $(git_macro)


Standard_options = --std=c++17 -pedantic
Warning_options = -fmax-errors=5 -Wall -Wextra
Optimisation_options = -Ofast -DNDEBUG -march=native -fwhole-program -static  -fno-finite-math-only -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math  -fno-signed-zeros -fno-math-errno -fno-trapping-math
Debug_options = -g -D_GLIBCXX_DEBUG

all : programs test


programs : optimised debug

opt_progs = 
optimised : $(opt_progs)

debug_progs = Test
debug : $(debug_progs)

Test : Test.cpp FloatingPoint.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(include_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@


test : regtest apptest

regtest : Test
	./Test
apptest :


clean :
	- rm $(debug_progs)

cleanall : clean
	- rm $(opt_progs)
