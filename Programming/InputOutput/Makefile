# Oliver Kullmann, 10.2.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../..

opt_progs = MatchFiles Matching DirMatching Timing DirTiming
debug_progs = MatchFiles_debug Matching_debug DirMatching_debug Timing_debug DirTiming_debug

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

MatchFiles : MatchFiles.cpp
MatchFiles_debug : MatchFiles.cpp

Matching : Matching.cpp Matching.hpp
Matching_debug : Matching.cpp Matching.hpp

DirMatching : DirMatching.cpp Matching.hpp
DirMatching_debug : DirMatching.cpp Matching.hpp

Timing : Timing.cpp
Timing_debug : Timing.cpp

DirTiming : DirTiming.cpp
DirTiming_debug : DirTiming.cpp


.PHONY : at_Matching at_Matching_debug

regtest :

apptest : at_Matching at_Matching_debug

# Special dependence on DirMatching here:
at_Matching at_Matching_debug : at_% : % DirMatching
	DirMatching ./$* tests/app_tests/Matching
