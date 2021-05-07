# Oliver Kullmann, 29.3.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..
okgecodeloc = $(oklibloc)/Satisfiability/Solvers/Gecode

opt_progs = Send-more-money Send-most-money Trivial

debug_progs = $(addsuffix _debug,$(opt_progs))

atps_debug = Trivial_debug Send-more-money_debug Send-most-money_debug
atps       = Trivial Send-more-money Send-most-money

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------



Send-more-money : Send-more-money.cpp
Send-more-money : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist
Send-more-money : Optimisation_plus = -fwhole-program
Send-more-money : include_options += -I$(okgecodeloc)
Send-more-money_debug : Send-more-money.cpp
Send-more-money_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist
Send-more-money_debug : include_options += -I$(okgecodeloc)

Send-most-money : Send-most-money.cpp
Send-most-money : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist
Send-most-money : Optimisation_plus = -fwhole-program
Send-most-money : include_options += -I$(okgecodeloc)
Send-most-money_debug : Send-most-money.cpp
Send-most-money_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist
Send-most-money_debug : include_options += -I$(okgecodeloc)

Trivial : Trivial.cpp Trivial.hpp
Trivial : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist
Trivial : Optimisation_plus = -fwhole-program
Trivial : include_options += -I$(okgecodeloc)
Trivial_debug : Trivial.cpp Trivial.hpp
Trivial_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist
Trivial_debug : include_options += -I$(okgecodeloc)


.PHONY : $(atps) $(atps_debug)

regtest :

apptest : $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
