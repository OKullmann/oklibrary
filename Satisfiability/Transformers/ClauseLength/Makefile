# Oliver Kullmann, 20.12.2018 (Swansea)

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all optimised debug clean

CXX = g++

Standard_options = --std=c++17 -pedantic
Warning_options = -Wall -Wextra
Optimisation_options = -Ofast -DNDEBUG
Debug_options = -g -D_GLIBCXX_DEBUG

all : optimised debug

optimised : CNF3CNF

debug : CNF3CNF_debug

CNF3CNF : TransCNF3CNF.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

CNF3CNF_debug : TransCNF3CNF.cpp
	$(CXX) $(Standard_options) $(Warning_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

clean :
	- rm CNF3CNF CNF3CNF_debug
