# Oliver Kullmann, 3.6.2013 (Swansea)
# Copyright 2013, 2015, 2016, 2018 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Temporary makefile, as long as the default C++ compiler of the OKlibrary
# is 4.1.2 (which can not be used here). Can also be used on its own.

# When called directly in this directory, that is, using
#   tawSolver> make
# or
#   tawSolver> make all
# produces executables
#   tawSolver, ttawSolver, ctawSolver, cttawSolver
# in this directory, using system-g++,
# plus the debug-versions (with suffix "_debug").

# The macro LIT_TYPE can be used to redefine the literal-type (default is int).

# When called by the recursive makefile of the OKlibrary, then these
# executables are in bin_dir, using the OKlibrary-g++.
# If the Oklibrary-g++ shall be used, but the executable shall be produced in
# this directory (Solvers/TawSolver), then run one level up:
# Solvers> make all sub_make_directories="TawSolver" bin_dir=""

# Three examples for calling from the OKlibrary:
# CXXFLAGS="-DLIT_TYPE=int16_t" oklib all
# oklib all tau_iteration=6
# CXXFLAGS="-DLIT_TYPE=int64_t" oklib all Optimisation_options="-g" sub_make_directories="TawSolver"



# Additional targets:

# Target "clean" removes the debug-versions, and "cleanall" removes also
# the normal executables.

# Target "timing" runs the four default solvers on two instances from
# Ramsey-theory, and creates the log-file "Output_log_tawSolver", while
# the timing results are in "Output_timing_tawSolver".

# Via the target "local" (not included in "all") the executables "Tau"
# and "Tau_debug" are produced (for experimentation with tau-computation).
# Cleaning here is included in "clean" resp. "cleanall".




SHELL = /bin/bash
.SUFFIXES :

.PHONY : all check new_check app_tests timing clean cleanall debug optimised local

ifeq ($(gpp_call_okl),)
  CXX = g++
else
  CXX = $(gpp_call_okl)
endif
# Remark: if using the standard library (in the future), then also
# LFLAGS="$(gcc_linking_okl) needs to be set.

Standard_options = --std=c++11 -pedantic
Warning_options = -Wall -Wextra -Wno-misleading-indentation
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
  side4_target = mtawSolver
  side5_target = mttawSolver
  debug_main = tawSolver_debug
  debug_side = ttawSolver_debug
  debug_side2 = ctawSolver_debug
  debug_side3 = cttawSolver_debug
  debug_side4 = mtawSolver_debug
  debug_side5 = mttawSolver_debug
else
  main_target = $(bin_dir)/tawSolver
  side_target = $(bin_dir)/ttawSolver
  side2_target = $(bin_dir)/ctawSolver
  side3_target = $(bin_dir)/cttawSolver
  side4_target = $(bin_dir)/mtawSolver
  side5_target = $(bin_dir)/mttawSolver
  debug_main = $(bin_dir)/tawSolver_debug
  debug_side = $(bin_dir)/ttawSolver_debug
  debug_side2 = $(bin_dir)/ctawSolver_debug
  debug_side3 = $(bin_dir)/cttawSolver_debug
  debug_side4 = $(bin_dir)/mtawSolver_debug
  debug_side5 = $(bin_dir)/mttawSolver_debug
endif

ifeq ($(log_dir),)
  outputfile = Output_timing_tawSolver
  logfile = Output_log_tawSolver
else
  outputfile = $(log_dir)/Output_timing_tawSolver
  logfile = $(log_dir)/Output_log_tawSolver
endif


all : optimised debug

optimised : $(main_target) $(side_target) $(side2_target) $(side3_target) $(side4_target) $(side5_target)

local : Tau Tau_debug

$(main_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) tawSolver.cpp -o $@ $(LFLAGS)

Tau : Tau.cpp Tau.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) Tau.cpp -o $@ $(LFLAGS)

$(side_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options_tau) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DPURE_LITERALS -DTAU_ITERATION=$(tau_iteration) tawSolver.cpp -o $@ $(LFLAGS)

$(side2_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DALL_SOLUTIONS tawSolver.cpp -o $@ $(LFLAGS)

$(side3_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options_tau) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DTAU_ITERATION=$(tau_iteration) -DALL_SOLUTIONS tawSolver.cpp -o $@ $(LFLAGS)

$(side4_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DVAR_MARGINALS tawSolver.cpp -o $@ $(LFLAGS)

$(side5_target) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options_tau) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DTAU_ITERATION=$(tau_iteration) -DVAR_MARGINALS tawSolver.cpp -o $@ $(LFLAGS)


debug : $(debug_main) $(debug_side) $(debug_side2) $(debug_side3) $(debug_side4) $(debug_side5)

$(debug_main) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) tawSolver.cpp -o $@ $(LFLAGS)

Tau_debug : Tau.cpp Tau.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) Tau.cpp -o $@ $(LFLAGS)

$(debug_side) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DPURE_LITERALS -DTAU_ITERATION=$(tau_iteration) tawSolver.cpp -o $@ $(LFLAGS)

$(debug_side2) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DALL_SOLUTIONS tawSolver.cpp -o $@ $(LFLAGS)

$(debug_side3) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DTAU_ITERATION=$(tau_iteration) -DALL_SOLUTIONS tawSolver.cpp -o $@ $(LFLAGS)

$(debug_side4) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DVAR_MARGINALS tawSolver.cpp -o $@ $(LFLAGS)

$(debug_side5) : tawSolver.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(Other_options) $(CPPFLAGS) $(CXXFLAGS) -DTAU_ITERATION=$(tau_iteration) -DVAR_MARGINALS tawSolver.cpp -o $@ $(LFLAGS)


repeat := 5
time := /usr/bin/time -f "%U %M" -o $(outputfile) -a
test_directory := app_tests/test_cases/full
input1 := $(test_directory)/VanDerWaerden_2-3-12_134.cnf
input2 := $(test_directory)/VanDerWaerden_2-3-12_135.cnf
# Those were created by
# VanderWaerdenCNF-O3-DNDEBUG 3 12 134
# VanderWaerdenCNF-O3-DNDEBUG 3 12 135
ifeq ($(log_dir),)
  solver1 = ./tawSolver
  solver2 = ./ttawSolver
  solver3 = ./ctawSolver
  solver4 = ./cttawSolver
  solver5 = ./mtawSolver
  solver6 = ./mttawSolver
else
  solver1 = tawSolver
  solver2 = ttawSolver
  solver3 = ctawSolver
  solver4 = cttawSolver
  solver5 = mtawSolver
  solver6 = mttawSolver
endif

timing :
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


check : Tau_debug
	./Tau_debug
app_tests :

clean :
	- rm $(debug_main) $(debug_side) $(debug_side2) $(debug_side3) $(debug_side4) $(debug_side5) Tau_debug

cleanall : clean
	- rm $(main_target) $(side_target) $(side2_target) $(side3_target) $(side4_target) $(side5_target) Tau
