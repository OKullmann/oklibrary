# Oliver Kullmann, 28.6.2018 (Swansea)
# Copyright 2018 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all clean cleanall debug optimised test

CXX = g++

Standard_options = --std=c++11 -pedantic
Warning_options = -Wall
Optimisation_options = -Ofast -DNDEBUG
debug_options = -g

main_target = autL1
debug_main = autL1_debug

all : optimised debug

optimised : $(main_target)

$(main_target) : AutarkiesL1.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LFLAGS)


debug : $(debug_main)

$(debug_main) : AutarkiesL1.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LFLAGS)


test :
	app_tests/RunTests

clean :
	- rm $(debug_main)

cleanall : clean
	- rm $(main_target)
