# Oliver Kullman, 24.3.2020 (Swansea
# Copyright 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = autL1
debug_progs = autL1_debug TestClauseSets

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

autL1 : AutarkiesL1.cpp Generics.hpp VarLit.hpp ClauseSets.hpp Input.hpp Encodings.hpp Translations.hpp InOut.hpp
autL1_debug : AutarkiesL1.cpp Generics.hpp VarLit.hpp ClauseSets.hpp Input.hpp Encodings.hpp Translations.hpp InOut.hpp

TestClauseSets : Warning_options += -Wno-missing-field-initializers
TestClauseSets : TestClauseSets.cpp ClauseSets.hpp VarLit.hpp


.PHONY: TestClauseSetsdebug

regtest : TestClauseSetsdebug

TestClauseSetsdebug : TestClauseSets
	./$<


apptest : autL1_debug
	app_tests/RunTests
