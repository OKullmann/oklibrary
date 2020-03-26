# Oliver Kullman, 24.3.2020 (Swansea
# Copyright 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = AutarkiesL1
debug_progs = AutarkiesL1_debug TestClauseSets

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

AutarkiesL1 : AutarkiesL1.cpp Generics.hpp VarLit.hpp ClauseSets.hpp Input.hpp Encodings.hpp Translations.hpp InOut.hpp $(programmingloc)/ProgramOptions/Environment.hpp
AutarkiesL1_debug : AutarkiesL1.cpp Generics.hpp VarLit.hpp ClauseSets.hpp Input.hpp Encodings.hpp Translations.hpp InOut.hpp $(programmingloc)/ProgramOptions/Environment.hpp

TestClauseSets : Warning_options += -Wno-missing-field-initializers
TestClauseSets : TestClauseSets.cpp ClauseSets.hpp VarLit.hpp


.PHONY: TestClauseSetsdebug

regtest : TestClauseSetsdebug

TestClauseSetsdebug : TestClauseSets
	./$<


.PHONY: fuzzing fuzzing_qbrg fuzzing_dqbrg runtest

fuzzing : fuzzing_qbrg fuzzing_dqbrg

fuzzing_qbrg : AutarkiesL1_debug
	generator="./QBRG" timeout --signal=SIGINT 5s ./Fuzzer "a30 30" "10*2,4|1,2,0|2-2,3,3; 10*2,4,1/4" "" t || true

fuzzing_dqbrg : AutarkiesL1_debug
	generator="./DQBRG" timeout --signal=SIGINT 5s ./Fuzzer "a30 30" "30" "10*2,4|1,2,0|2-2,3,3; 10*2,4,1/4" "" t || true

runtest : AutarkiesL1_debug
	app_tests/RunTests

apptest : fuzzing runtest
