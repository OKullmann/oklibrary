# Oliver Kullmann, 28.6.2018 (Swansea)
# Copyright 2018, 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all programs optimised debug test regtest apptest RunTests clean cleanall

CXX = g++

Standard_options = --std=c++17 -pedantic
Warning_options = -fmax-errors=5 -Wall -Wextra -Wno-misleading-indentation -Wno-dangling-else
Optimisation_options = -Ofast -DNDEBUG
debug_options = -g -D_GLIBCXX_DEBUG

main_target = autL1
debug_main = autL1_debug
main_deps = AutarkiesL1.cpp Generics.hpp VarLit.hpp ClauseSets.hpp Input.hpp Encodings.hpp Translations.hpp InOut.hpp

regtargets = TestClauseSets
apptargets = RunTests

all : programs test

programs : optimised debug

optimised : $(main_target)

$(main_target) : $(main_deps)
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS)


debug : $(debug_main)

$(debug_main) : $(main_deps)
	$(CXX) $(Standard_options) $(Warning_options) $(debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS)


test : regtest apptest

regtest : $(regtargets)

TestClauseSets : TestClauseSets.cpp ClauseSets.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-missing-field-initializers $(debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS)
	./$@

apptest : $(apptargets)

RunTests :
	app_tests/RunTests

clean :
	- rm $(debug_main) $(regtargets)

cleanall : clean
	- rm $(main_target)
