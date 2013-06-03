# Oliver Kullmann, 3.6.2013 (Swansea)
# Copyright 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Temporary makefile, as long as the default C++ compiler is 4.1.2.
# When called directly in this directory, produces executable tawSolver
# in this directory; when called by the recursive makefile, then in bin_dir.

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all cleanall

CXX = g++
Standard_options = --std=c++11 -pedantic
Warning_options = -Wall
Optimisation_options = -Ofast -DNDEBUG

ifeq ($(bin_dir),)
  main_target = tawSolver
else
  main_target = $(bin_dir)/tawSolver
endif

all : $(main_target)

$(main_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) tawSolver.cpp -o $(main_target)

cleanall :
	- rm $(main_target)
