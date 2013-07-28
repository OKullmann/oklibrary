# Oliver Kullmann, 3.6.2013 (Swansea)
# Copyright 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Temporary makefile, as long as the default C++ compiler is 4.1.2.
# When called directly in this directory, produces executables
#   tawSolver, ttawSolver
# in this directory, using system-g++; when called by the recursive makefile,
# then in bin_dir, using the OKlibrary-g++.

# If the Oklibrar-g++ shall be used, but the executable shall be produced in
# this directory (Solvers/TawSolver), then run one level up:
# Solvers> make all sub_make_directories="TawSolver" bin_dir=""

# The macro LIT_TYPE can be used to redefine the literal-type (default is int).

# When calling from the OKlibrary, e.g.:
# CXXFLAGS="-DLIT_TYPE=int16_t" oklib all
# oklib all tau_iteration=6
# CXXFLAGS="-DLIT_TYPE=int64_t" oklib all Optimisation_options="-g" sub_make_directories="TawSolver"


SHELL = /bin/bash
.SUFFIXES :

.PHONY : all check new_check app_tests timing cleanall

ifeq ($(gpp_call_okl),)
  CXX = g++
else
  CXX = $(gpp_call_okl)
endif
# Remark: if using the standard library, then also LFLAGS="$(gcc_linking_okl)
# needs to be set.

Standard_options = --std=c++11 -pedantic
Warning_options = -Wall
Optimisation_options = -Ofast -funroll-loops -funsafe-loop-optimizations -fno-math-errno -funsafe-math-optimizations -ffinite-math-only -fwhole-program -DNDEBUG
# Run from time to time with -Wunsafe-loop-optimizations to check for the
# assumptions made by -funsafe-loop-optimizations.

ifeq ($(git_id_oklib),)
  Other_options =
else
  Other_options = -DGIT_ID=$(git_id_oklib)
endif
ifneq ($(oklibrary_domain),)
  Other_options += -DOKLIB="$(subst http://,,$(oklibrary_domain))"
endif
tau_iteration = 5

ifeq ($(bin_dir),)
  main_target = tawSolver
  side_target = ttawSolver
else
  main_target = $(bin_dir)/tawSolver
  side_target = $(bin_dir)/ttawSolver
endif

ifeq ($(log_dir),)
  outputfile = Output_timing_tawSolver
else
  outputfile = $(log_dir)/Output_timing_tawSolver
endif

all : $(main_target) $(side_target)

$(main_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) tawSolver.cpp -o $(main_target) $(LFLAGS)

$(side_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DPURE_LITERALS -DTAU_ITERATION=$(tau_iteration) tawSolver.cpp -o $(side_target) $(LFLAGS)


repeat := 5
time := /usr/bin/time -f "%U %M" -o $(outputfile) -a
input1 := VanDerWaerden_2-3-12_134.cnf
input2 := VanDerWaerden_2-3-12_135.cnf
timing :
	VanderWaerdenCNF-O3-DNDEBUG 3 12 134
	tawSolver -v > $(outputfile)
	date >> $(outputfile); echo >> $(outputfile)
	echo "tawSolver $(input1)" >> $(outputfile)
	- for ((i=0; i<$(repeat); ++i)); do $(time) tawSolver $(input1); done; echo >> $(outputfile)
	VanderWaerdenCNF-O3-DNDEBUG 3 12 135
	echo "tawSolver $(input2)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) tawSolver $(input2); done; echo >> $(outputfile)
	ttawSolver -v >> $(outputfile)
	echo "ttawSolver $(input1)" >> $(outputfile)
	- for ((i=0; i<$(repeat); ++i)); do $(time) ttawSolver $(input1); done; echo >> $(outputfile)
	VanderWaerdenCNF-O3-DNDEBUG 3 12 135
	echo "ttawSolver $(input2)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) ttawSolver $(input2); done; echo >> $(outputfile)
	rm $(input1) $(input2)



check :
new_check :
app_tests :


cleanall :
	- rm $(main_target) $(side_target)
