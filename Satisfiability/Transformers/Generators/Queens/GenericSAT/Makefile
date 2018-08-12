# Oliver Kullmann, 6.7.2018 (Swansea)

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all clean

CXX = g++

Standard_options = --std=c++14 -pedantic
Warning_options = -Wall -Wextra
Optimisation_options = -Ofast -DNDEBUG -faggressive-loop-optimizations -fstrict-aliasing -fwhole-program -funroll-loops -fno-math-errno -funsafe-math-optimizations -freciprocal-math -ffinite-math-only -fno-signed-zeros -fno-trapping-math -funroll-all-loops -funswitch-loops -fmodulo-sched -fmodulo-sched-allow-regmoves -ftree-loop-ivcanon -fvariable-expansion-in-unroller -floop-nest-optimize -fgraphite-identity -ftree-loop-distribution -static
Debug_options = -g

all : program program_debug SimTaw SimTaw_debug GreedyAmoAlo GreedyAmoAlo_debug Lookahead Lookahead_debug TestSimTaw test

program : Program.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp NQueensPartial.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

program_debug : Program.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp NQueensPartial.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

SimTaw : SimTaw.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

SimTaw_debug : SimTaw.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

GreedyAmoAlo : GreedyAmoAlo.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

GreedyAmoAlo_debug : GreedyAmoAlo.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

Lookahead : Lookahead.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Optimisation_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

Lookahead_debug : Lookahead.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

TestSimTaw : TestSimTaw.cpp Backtracking.hpp ChessBoard.hpp NQueens.hpp
	$(CXX) $(Standard_options) $(Warning_options) -Wno-parentheses $(Debug_options) $(CPPFLAGS) $(CXXFLAGS) $< -o $@


test :
	./TestSimTaw; ./RunTests

clean :
	- rm program program_debug SimTaw SimTaw_debug GreedyAmoAlo GreedyAmoAlo_debug Lookahead Lookahead_debug TestSimTaw
