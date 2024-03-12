# Oliver Kullmann, 19.3.2019 (Swansea)
# Copyright 2019, 2021, 2024 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../..

opt_progs = RExtractColumns

debug_progs = $(addsuffix _debug,$(opt_progs)) Test_debug

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

RExtractColumns : RExtractColumns.cpp Environment.hpp Strings.hpp
RExtractColumns_debug : RExtractColumns.cpp Environment.hpp Strings.hpp

# -------------------------

Test_debug : numerics_minus =
Test_debug : Test.cpp Environment.hpp Strings.hpp

regtest : Test_debug
	./Test_debug
apptest :

