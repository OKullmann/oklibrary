# Oliver Kullmann, 6.7.2018 (Swansea)

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all optimised debug clean

CXX = g++

Standard_options = --std=c++14 -pedantic
Warning_options = -Wall -Wextra
Optimisation_options = -Ofast -DNDEBUG -fstrict-aliasing -funroll-loops -fmodulo-sched -fmodulo-sched-allow-regmoves -ftree-loop-ivcanon -fvariable-expansion-in-unroller -floop-nest-optimize -fgraphite-identity -static
Debug_options = -g

all : optimised debug test

optimised : SimTaw ExpQueens Recursion

debug : program program_debug SimTaw_debug TestSimTaw ExpQueens_debug Recursion Recursion_debug test

program : Program.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp NQueensPartial.hpp RandGen.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

program_debug : Program.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp NQueensPartial.hpp RandGen.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

SimTaw : SimTaw.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp InOut.hpp RandGen.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

SimTaw_debug : SimTaw.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp InOut.hpp RandGen.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

TestSimTaw : TestSimTaw.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp RandGen.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

ExpQueens : ExpQueens.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp InOut.hpp RandGen.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

ExpQueens_debug : ExpQueens.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp InOut.hpp RandGen.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

Recursion : Recursion.cpp Recursion.hpp InOut.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

Recursion_debug : Recursion.cpp Recursion.hpp InOut.hpp
	$(CXX) $(Standard_options) $(Warning_options) $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@



test :
	./TestSimTaw
	./RunTests

clean :
	- rm program program_debug SimTaw SimTaw_debug TestSimTaw ExpQueens ExpQueens_debug Recursion Recursion_debug
