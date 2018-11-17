# Oliver Kullmann, 1.2.2009 (Swansea)
# Copyright 2009, 2018 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

SHELL = /bin/bash
.SUFFIXES :
.PHONY : all clean cleanall

CXX = g++

Standard_options = --std=c++17 -pedantic
Warning_options = -Wall -Wextra
Optimisation_options = -Ofast -DNDEBUG

all : EmptyProgram

EmptyProgram : EmptyProgram.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

clean :

cleanall :
	- rm EmptyProgram
