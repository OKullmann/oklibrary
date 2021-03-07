# Oliver Kullmann, 10.2.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../..

opt_progs = MatchFiles Matching DirMatching Timing DirTiming
test_progs = DirMatching
test_progs := $(addprefix Test,$(test_progs))
debug_progs = $(addsuffix _debug,$(opt_progs)) $(addsuffix _debug,$(test_progs))
unit_targets = $(addsuffix _debug_run,$(test_progs))

.PHONY : $(unit_targets)

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

MatchFiles : MatchFiles.cpp
MatchFiles_debug : MatchFiles.cpp

Matching : Matching.cpp Matching.hpp
Matching_debug : Matching.cpp Matching.hpp

DirMatching : DirMatching.cpp Matching.hpp DirMatching.hpp
DirMatching_debug : DirMatching.cpp Matching.hpp DirMatching.hpp

Timing : Timing.cpp Timing.hpp
Timing_debug : Timing.cpp Timing.hpp

DirTiming : DirTiming.cpp DirMatching.hpp
DirTiming_debug : DirTiming.cpp DirMatching.hpp


# -------------------------

TestDirMatching_debug : TestDirMatching.cpp DirMatching.hpp


# -------------------------

regtest : $(unit_targets)

$(unit_targets) : %_run : %
	./$*


.PHONY : at_Matching at_Matching_debug

apptest : at_Matching at_Matching_debug

# Special dependency on DirMatching and Matching here:
at_Matching at_Matching_debug : at_% : % Matching DirMatching
	./DirMatching ./$* tests/app_tests/Matching
