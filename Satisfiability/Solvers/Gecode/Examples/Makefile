# Oliver Kullmann, 29.3.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

opt_progs = send-more-money send-more-money-with-gist send-more-money-with-gist-inspection send-most-money trivial

debug_progs = $(addsuffix _debug,$(opt_progs))

atps_debug =
atps       =

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------



send-more-money : send-more-money.cpp
send-more-money : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
send-more-money : Optimisation_plus = -fwhole-program
send-more-money_debug : send-more-money.cpp
send-more-money_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport

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

send-most-money : send-most-money.cpp
send-most-money : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
send-most-money : Optimisation_plus = -fwhole-program
send-most-money_debug : send-most-money.cpp
send-most-money_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport

trivial : trivial.cpp trivial.hpp
trivial : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
trivial : Optimisation_plus = -fwhole-program
trivial_debug : trivial.cpp trivial.hpp
trivial_debug : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport


.PHONY : $(atps) $(atps_debug)

regtest :

apptest : $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
