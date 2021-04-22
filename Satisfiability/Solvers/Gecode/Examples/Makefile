# Oliver Kullmann, 29.3.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..
okgecodeloc = $(oklibloc)/Satisfiability/Solvers/Gecode

opt_progs = Send-more-money send-more-money-with-gist send-more-money-with-gist-inspection Send-most-money Send-most-money-with-gist Trivial

debug_progs = $(addsuffix _debug,$(opt_progs))

atps_debug = Trivial_debug Send-more-money_debug Send-most-money_debug
atps       = Trivial Send-more-money Send-most-money

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------



Send-more-money : Send-more-money.cpp
Send-more-money : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Send-more-money : Optimisation_plus = -fwhole-program
Send-more-money : include_options += -I$(okgecodeloc)
Send-more-money_debug : Send-more-money.cpp
Send-more-money_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Send-more-money_debug : include_options += -I$(okgecodeloc)

send-more-money-with-gist : send-more-money-with-gist.cpp
send-more-money-with-gist : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist
send-more-money-with-gist : Optimisation_plus = -fwhole-program
send-more-money-with-gist_debug : send-more-money-with-gist.cpp
send-more-money-with-gist_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist

send-more-money-with-gist-inspection : send-more-money-with-gist-inspection.cpp
send-more-money-with-gist-inspection : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist
send-more-money-with-gist-inspection : Optimisation_plus = -fwhole-program
send-more-money-with-gist-inspection_debug : send-more-money-with-gist-inspection.cpp
send-more-money-with-gist-inspection_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist

Send-most-money : Send-most-money.cpp
Send-most-money : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Send-most-money : Optimisation_plus = -fwhole-program
Send-most-money_debug : Send-most-money.cpp
Send-most-money_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport

Send-most-money-with-gist : Send-most-money-with-gist.cpp
Send-most-money-with-gist : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist
Send-most-money-with-gist : Optimisation_plus = -fwhole-program
Send-most-money-with-gist_debug : Send-most-money-with-gist.cpp
Send-most-money-with-gist_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport -lgecodegist

Trivial : Trivial.cpp Trivial.hpp
Trivial : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Trivial : Optimisation_plus = -fwhole-program
Trivial : include_options += -I$(okgecodeloc)
Trivial_debug : Trivial.cpp Trivial.hpp
Trivial_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Trivial_debug : include_options += -I$(okgecodeloc)


.PHONY : $(atps) $(atps_debug)

regtest :

apptest : $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
