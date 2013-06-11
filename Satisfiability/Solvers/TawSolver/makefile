# Oliver Kullmann, 3.6.2013 (Swansea)
# Copyright 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Temporary makefile, as long as the default C++ compiler is 4.1.2.
# When called directly in this directory, produces executable tawSolver
# in this directory, using system-g++; when called by the recursive makefile,
# then in bin_dir, using the OKlibrary-g++.

# If the Oklibrar-g++ shall be used, but the executable shall be produced in
# this directory (Solvers/TawSolver), then run one level up:
# Solvers> make all sub_make_directories="TawSolver" bin_dir=""

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all check new_check app_tests cleanall

ifeq ($(gpp_call_okl),)
  CXX = g++
else
  CXX = $(gpp_call_okl)
endif
# Remark: if using the standard library, then also LFLAGS="$(gcc_linking_okl)
# needs to be set.

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
	$(CXX) -DMAX_CLAUSE_LENGTH=32 $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) tawSolver.cpp -o $(main_target) $(LFLAGS)
	$(CXX) -DMAX_CLAUSE_LENGTH=64 $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) tawSolver.cpp -o $(main_target)64 $(LFLAGS)



check :
new_check :
app_tests :


cleanall :
	- rm $(main_target)
