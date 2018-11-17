# Oliver Kullmann, 17.11.2018 (Swansea)
# Copyright 2018 Oliver Kullmann
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

all : AtomicCounter

AtomicCounter : AtomicCounter.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) -pthread $< -o $@

clean :

cleanall :
	- rm AtomicCounter
