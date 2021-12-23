# Oleg Zaikin, 20.5.2021 (Irkutsk)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

opt_progs = Euler Euler_BBOpt

debug_progs = $(addsuffix _debug,$(opt_progs))

atps_debug = Euler_debug
atps       = Euler

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

randgenloc = $(satisfiabilityloc)/Transformers/Generators/Random/
optimisationloc = $(programmingloc)/Numerics/
include_options += -I $(randgenloc)

gist ?= 0
gist_option = -DGIST=$(gist)

# -------------------------

Euler : Euler.cpp ../Lookahead.hpp ../Statistics.hpp $(randgenloc)/LSRG.hpp $(randgenloc)/Numbers.hpp $(randgenloc)/ClauseSets.hpp $(randgenloc)/SeedOrganisation.hpp $(randgenloc)/LatinSquares.hpp $(randgenloc)/Algorithms.hpp $(randgenloc)/Distributions.hpp $(programmingloc)/ProgramOptions/Environment.hpp
Euler : CPPFLAGS += $(gist_option)
Euler : LDFLAGS += -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
ifeq ($(gist),1)
Euler : LDFLAGS += -lgecodegist
endif
Euler : Optimisation_plus = -fwhole-program # ???
Euler_debug : Euler.cpp ../Lookahead.hpp ../Statistics.hpp $(randgenloc)/LSRG.hpp $(randgenloc)/Numbers.hpp $(randgenloc)/ClauseSets.hpp $(randgenloc)/SeedOrganisation.hpp $(randgenloc)/LatinSquares.hpp $(randgenloc)/Algorithms.hpp $(randgenloc)/Distributions.hpp $(programmingloc)/ProgramOptions/Environment.hpp
Euler_debug : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport # -pthread not needed once only part of Optimisation.hpp included

Euler_BBOpt : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Euler_BBOpt : Optimisation_plus = -fwhole-program
Euler_BBOpt : Euler_BBOpt.cpp $(optimisationloc)/Optimisation.hpp ../Lookahead.hpp ../Statistics.hpp $(randgenloc)/LSRG.hpp $(randgenloc)/Numbers.hpp $(randgenloc)/ClauseSets.hpp $(randgenloc)/SeedOrganisation.hpp $(randgenloc)/LatinSquares.hpp $(randgenloc)/Algorithms.hpp $(randgenloc)/Distributions.hpp $(programmingloc)/ProgramOptions/Environment.hpp
Euler_BBOpt_debug : LDFLAGS += -pthread -lgecodesearch -lgecodeint -lgecodekernel -lgecodesupport
Euler_BBOpt_debug : Euler_BBOpt.cpp $(optimisationloc)/Optimisation.hpp ../Lookahead.hpp ../Statistics.hpp $(randgenloc)/LSRG.hpp $(randgenloc)/Numbers.hpp $(randgenloc)/ClauseSets.hpp $(randgenloc)/SeedOrganisation.hpp $(randgenloc)/LatinSquares.hpp $(randgenloc)/Algorithms.hpp $(randgenloc)/Distributions.hpp $(programmingloc)/ProgramOptions/Environment.hpp

# -------------------------

.PHONY : $(atps) $(atps_debug)

regtest :

apptest : $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
