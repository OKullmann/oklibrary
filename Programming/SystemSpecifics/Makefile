# Oliver Kullman, 27.3.2023 (Swansea
# Copyright 2023 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../


opt_progs =

popt_progs = $(addprefix p-, $(opt_progs))


test_progs_stem = SystemCalls
test_progs = $(addprefix Test,$(test_progs_stem))

debug_progs = $(addsuffix _debug,$(opt_progs)) $(addsuffix _debug,$(test_progs))


atps_debug =
atps       =

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))


include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------


# -------------------------

TestSystemCalls_debug : TestSystemCalls.cpp SystemCalls.hpp

# -------------------------

regtest_targets := $(addprefix Test,$(addsuffix debug, $(test_progs_stem)))

.PHONY : $(regtest_targets)

regtest : $(regtest_targets)

$(regtest_targets) : Test%debug : Test%_debug
	./$<


.PHONY : $(atps) $(atps_debug)

apptest : $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
