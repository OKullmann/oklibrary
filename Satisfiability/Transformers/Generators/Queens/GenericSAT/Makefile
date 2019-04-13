# Oliver Kullmann, 6.7.2018 (Swansea)
# Copyright 2018, 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../../..

opt_progs = program SimTaw ExpQueens Recursion ExploreBTs MeasureTau
debug_progs = $(addsuffix _debug, $(opt_progs)) TestSimTaw

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

randgenloc = $(oklibloc)/Satisfiability/Transformers/Generators
include_options += -I $(randgenloc)


program : Program.cpp Backtracking.hpp ChessBoard.hpp Trees.hpp Colour.hpp NQueens.hpp Heuristics.hpp NQueensPartial.hpp
program_debug : Warning_options += -Wno-parentheses
program_debug : Program.cpp Backtracking.hpp ChessBoard.hpp Trees.hpp Colour.hpp NQueens.hpp Heuristics.hpp NQueensPartial.hpp

SimTaw : Warning_options += -Wno-parentheses
SimTaw : SimTaw.cpp Backtracking.hpp ChessBoard.hpp Trees.hpp Colour.hpp NQueens.hpp Heuristics.hpp InOut.hpp
SimTaw_debug : Warning_options += -Wno-parentheses
SimTaw_debug : SimTaw.cpp Backtracking.hpp ChessBoard.hpp Trees.hpp Colour.hpp NQueens.hpp Heuristics.hpp InOut.hpp

TestSimTaw : Warning_options += -Wno-parentheses
TestSimTaw : TestSimTaw.cpp Backtracking.hpp ChessBoard.hpp Trees.hpp Colour.hpp NQueens.hpp

ExpQueens : Warning_options += -Wno-parentheses
ExpQueens : ExpQueens.cpp Backtracking.hpp ChessBoard.hpp Trees.hpp Colour.hpp NQueens.hpp Heuristics.hpp InOut.hpp
ExpQueens_debug : Warning_options += -Wno-parentheses
ExpQueens_debug : ExpQueens.cpp Backtracking.hpp ChessBoard.hpp Trees.hpp Colour.hpp NQueens.hpp Heuristics.hpp InOut.hpp

Recursion : Recursion.cpp Recursion.hpp InOut.hpp BranchingTuples.hpp
Recursion_debug : Recursion.cpp Recursion.hpp InOut.hpp BranchingTuples.hpp

ExploreBTs : Optimisation_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
ExploreBTs : Optimisation_plus += -fno-signed-zeros -fno-math-errno -fno-trapping-math
ExploreBTs : ExploreBTs.cpp BranchingTuples.hpp InOut.hpp
ExploreBTs_debug : ExploreBTs.cpp BranchingTuples.hpp InOut.hpp

MeasureTau : Optimisation_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
MeasureTau : Optimisation_plus += -fno-signed-zeros -fno-math-errno -fno-trapping-math
MeasureTau : MeasureTau.cpp BranchingTuples.hpp
MeasureTau_debug : MeasureTau.cpp BranchingTuples.hpp


regtest : TestSimTaw
	./TestSimTaw
apptest : SimTaw SimTaw_debug ExpQueens ExpQueens_debug
	./RunTests
