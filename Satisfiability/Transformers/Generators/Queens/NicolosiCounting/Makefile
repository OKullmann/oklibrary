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
Optimisation_options = -Ofast -DNDEBUG -funroll-loops -fno-math-errno -ffinite-math-only -fwhole-program -funsafe-loop-optimizations -funroll-loops
Debug_options = -g

all : qcount qcount_debug

qcount : NQueens.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

qcount_debug : NQueens.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

clean :
	- rm qcount qcount_debug
