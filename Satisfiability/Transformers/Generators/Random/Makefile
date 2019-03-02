# Oliver Kullmann, 2.3.2019 (Swansea)
# Copyright 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all programs optimised debug test regtest apptest clean cleanall

CXX = g++

Standard_options = --std=c++17 -pedantic
Warning_options = -fmax-errors=5 -Wall -Wextra
Optimisation_options = -Ofast -DNDEBUG -march=native -fwhole-program -static
Debug_options = -g -D_GLIBCXX_DEBUG

all : programs test


programs : optimised debug

opt_progs = 
optimised : $(opt_progs)

debug_progs = Test
debug : $(debug_progs)

Test : Test.cpp Numbers.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(include_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@


test : regtest apptest

regtest : Test
	./Test
apptest :


clean :
	- rm $(debug_progs)

cleanall : clean
	- rm $(opt_progs)
