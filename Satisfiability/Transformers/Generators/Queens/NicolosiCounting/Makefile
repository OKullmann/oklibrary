# Oliver Kullmann, 25.5.2018 (Swansea)
# Copyright 2018 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all clean

CXX = g++

Standard_options = --std=c++11 -pedantic
Warning_options = -Wall
Optimisation_options = -Ofast -DNDEBUG
Optimisation_options_full = -Ofast -DNDEBUG -ffinite-math-only -funroll-loops -fwhole-program -fno-math-errno -funsafe-loop-optimizations
Debug_options = -g

SETN=-DNN=15

all : qcount qcount_debug qcount_ct qcount_ct_debug wqcount wqcount_debug

qcount : NQueens.cpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

qcount_debug : NQueens.cpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

qcount_ct : NQueens_ct.cpp
	$(CXX) $(SETN) $(Standard_options) $(Warning_options) $(Optimisation_options_full) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

qcount_ct_debug : NQueens_ct.cpp
	$(CXX) $(SETN) $(Standard_options) $(Warning_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

wqcount : NQueens_w.cpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

wqcount_debug : NQueens_w.cpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

clean :
	- rm qcount qcount_debug qcount_ct qcount_ct_debug wqcount wqcount_debug
