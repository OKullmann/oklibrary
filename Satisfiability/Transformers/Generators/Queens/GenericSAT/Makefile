# Oliver Kullmann, 6.7.2018 (Swansea)

SHELL = /bin/bash
.SUFFIXES :

.PHONY : all clean

CXX = g++

Standard_options = --std=c++11 -pedantic
Warning_options = -Wall
Optimisation_options = -Ofast -DNDEBUG
Optimisation_options_full = -Ofast -DNDEBUG
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
