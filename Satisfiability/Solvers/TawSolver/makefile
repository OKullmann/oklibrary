# Oliver Kullmann, 3.6.2013 (Swansea)
# Copyright 2013, 2015 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Temporary makefile, as long as the default C++ compiler is 4.1.2.
# When called directly in this directory, produces executables
#   tawSolver, ttawSolver
# in this directory, using system-g++; when called by the recursive makefile,
# then in bin_dir, using the OKlibrary-g++.

# If the Oklibrary-g++ shall be used, but the executable shall be produced in
# this directory (Solvers/TawSolver), then run one level up:
# Solvers> make all sub_make_directories="TawSolver" bin_dir=""

# The macro LIT_TYPE can be used to redefine the literal-type (default is int).

# When calling from the OKlibrary, three examples:
# CXXFLAGS="-DLIT_TYPE=int16_t" oklib all
# oklib all tau_iteration=6
# CXXFLAGS="-DLIT_TYPE=int64_t" oklib all Optimisation_options="-g" sub_make_directories="TawSolver"


SHELL = /bin/bash
.SUFFIXES :

.PHONY : all check new_check app_tests timing cleanall debug allall

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
Optimisation_options_tau = -Ofast -funroll-loops -funsafe-loop-optimizations -fno-math-errno -funsafe-math-optimizations -fwhole-program -DNDEBUG
debug_options = -g

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
  side2_target = ctawSolver
  side3_target = cttawSolver
  debug_main = tawSolver_debug
  debug_side = ttawSolver_debug
  debug_side2 = ctawSolver_debug
  debug_side3 = cttawSolver_debug
else
  main_target = $(bin_dir)/tawSolver
  side_target = $(bin_dir)/ttawSolver
  side2_target = $(bin_dir)/ctawSolver
  side3_target = $(bin_dir)/cttawSolver
  debug_main = $(bin_dir)/tawSolver_debug
  debug_side = $(bin_dir)/ttawSolver_debug
  debug_side2 = $(bin_dir)/ctawSolver_debug
  debug_side3 = $(bin_dir)/cttawSolver_debug
endif

ifeq ($(log_dir),)
  outputfile = Output_timing_tawSolver
  logfile = Output_log_tawSolver
else
  outputfile = $(log_dir)/Output_timing_tawSolver
  logfile = $(log_dir)/Output_log_tawSolver
endif


all : $(main_target) $(side_target) $(side2_target) $(side3_target)

allall : all debug

$(main_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) tawSolver.cpp -o $@ $(LFLAGS)

$(side_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options_tau) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DPURE_LITERALS -DTAU_ITERATION=$(tau_iteration) tawSolver.cpp -o $@ $(LFLAGS)

$(side2_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DALL_SOLUTIONS tawSolver.cpp -o $@ $(LFLAGS)

$(side3_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options_tau) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DTAU_ITERATION=$(tau_iteration) -DALL_SOLUTIONS tawSolver.cpp -o $@ $(LFLAGS)


debug : $(debug_main) $(debug_side) $(debug_side2) $(debug_side3)

$(debug_main) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) tawSolver.cpp -o $@ $(LFLAGS)

$(debug_side) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DPURE_LITERALS -DTAU_ITERATION=$(tau_iteration) tawSolver.cpp -o $@ $(LFLAGS)

$(debug_side2) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DALL_SOLUTIONS tawSolver.cpp -o $@ $(LFLAGS)

$(debug_side3) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DTAU_ITERATION=$(tau_iteration) -DALL_SOLUTIONS tawSolver.cpp -o $@ $(LFLAGS)


repeat := 5
time := /usr/bin/time -f "%U %M" -o $(outputfile) -a
input1 := VanDerWaerden_2-3-12_134.cnf
input2 := VanDerWaerden_2-3-12_135.cnf
ifeq ($(log_dir),)
  solver1 = ./tawSolver
  solver2 = ./ttawSolver
  solver3 = ./ctawSolver
  solver4 = ./cttawSolver
else
  solver1 = tawSolver
  solver2 = ttawSolver
  solver3 = ctawSolver
  solver4 = cttawSolver
endif

timing :
	VanderWaerdenCNF-O3-DNDEBUG 3 12 134
	VanderWaerdenCNF-O3-DNDEBUG 3 12 135
	date > $(outputfile); echo >> $(outputfile)
	echo >> $(logfile); date >> $(logfile); echo >> $(logfile)
	$(solver1) -v >> $(outputfile); $(solver1) -v >> $(logfile)
	echo "$(solver1) $(input1)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) $(solver1) $(input1) $(logfile) $(logfile); done; echo >> $(outputfile)
	echo "$(solver1) $(input2)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) $(solver1) $(input2) $(logfile) $(logfile); done; echo >> $(outputfile)
	$(solver2) -v >> $(outputfile); $(solver2) -v >> $(logfile)
	echo "$(solver2) $(input1)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) $(solver2) $(input1) $(logfile) $(logfile); done; echo >> $(outputfile)
	echo "$(solver2) $(input2)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) $(solver2) $(input2) $(logfile) $(logfile); done; echo >> $(outputfile)
	$(solver3) -v >> $(outputfile); $(solver3) -v >> $(logfile)
	echo "$(solver3) $(input1)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) $(solver3) $(input1) $(logfile) $(logfile); done; echo >> $(outputfile)
	echo "$(solver3) $(input2)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) $(solver3) $(input2) $(logfile) $(logfile); done; echo >> $(outputfile)
	$(solver4) -v >> $(outputfile); $(solver4) -v >> $(logfile)
	echo "$(solver4) $(input1)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) $(solver4) $(input1) $(logfile) $(logfile); done; echo >> $(outputfile)
	echo "$(solver4) $(input2)" >> $(outputfile)
	for ((i=0; i<$(repeat); ++i)); do $(time) $(solver4) $(input2) $(logfile) $(logfile); done; echo >> $(outputfile)
	rm $(input1) $(input2)


check :
new_check :
app_tests :


cleanall :
	- rm $(main_target) $(side_target) $(debug_main) $(debug_side) $(side2_target) $(debug_side2) $(side3_target) $(debug_side3)
