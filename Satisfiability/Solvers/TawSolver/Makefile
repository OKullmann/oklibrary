# Oliver Kullmann, 3.3.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = tawSolver ttawSolver ctawSolver cttawSolver mtawSolver mttawSolver tawSolver_wa ttawSolver_wa ctawSolver_wa cttawSolver_wa Tau

debug_progs = $(addsuffix _debug,$(opt_progs))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

weights_set1 = -DTWEIGHT_2=4.85 -DTWEIGHT_4=0.354 -DTWEIGHT_5=0.11 -DTWEIGHT_6=0.0694 -DTWEIGHT_BASIS_OPEN=1.46

weights_set2 = -DWEIGHT_2=7.0 -DWEIGHT_4=0.31 -DWEIGHT_5=0.19 -DWEIGHT_6=0.19/1.7 -DWEIGHT_BASIS_OPEN=1.70


tawSolver : tawSolver.cpp
tawSolver_debug : tawSolver.cpp

tawSolver_wa : tawSolver.cpp
tawSolver_wa : CPPFLAGS += $(weights_set2)
tawSolver_wa_debug : tawSolver.cpp
tawSolver_wa_debug : CPPFLAGS += $(weights_set2)


ttawSolver : tawSolver.cpp
ttawSolver : CPPFLAGS += -DPURE_LITERALS -DTAU_ITERATION
ttawSolver_debug : tawSolver.cpp
ttawSolver_debug : CPPFLAGS += -DPURE_LITERALS -DTAU_ITERATION

ttawSolver_wa : tawSolver.cpp
ttawSolver_wa : CPPFLAGS += -DPURE_LITERALS -DTAU_ITERATION $(weights_set1)
ttawSolver_wa_debug : tawSolver.cpp
ttawSolver_wa_debug : CPPFLAGS += -DPURE_LITERALS -DTAU_ITERATION $(weights_set1)


ctawSolver : tawSolver.cpp
ctawSolver : CPPFLAGS += -DALL_SOLUTIONS
ctawSolver_debug : tawSolver.cpp
ctawSolver_debug : CPPFLAGS += -DALL_SOLUTIONS

ctawSolver_wa : tawSolver.cpp
ctawSolver_wa : CPPFLAGS += -DALL_SOLUTIONS $(weights_set2)
ctawSolver_wa_debug : tawSolver.cpp
ctawSolver_wa_debug : CPPFLAGS += -DALL_SOLUTIONS $(weights_set2)


cttawSolver : tawSolver.cpp
cttawSolver : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS
cttawSolver_debug : tawSolver.cpp
cttawSolver_debug : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS

cttawSolver_wa : tawSolver.cpp
cttawSolver_wa : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS $(weights_set1)
cttawSolver_wa_debug : tawSolver.cpp
cttawSolver_wa_debug : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS $(weights_set1)


mtawSolver : tawSolver.cpp
mtawSolver : CPPFLAGS += -DVAR_MARGINALS
mtawSolver_debug : tawSolver.cpp
mtawSolver_debug : CPPFLAGS += -DVAR_MARGINALS

mttawSolver : tawSolver.cpp
mttawSolver : CPPFLAGS += -DTAU_ITERATION -DVAR_MARGINALS
mttawSolver_debug : tawSolver.cpp
mttawSolver_debug : CPPFLAGS += -DTAU_ITERATION -DVAR_MARGINALS



Tau : Tau.cpp Tau.hpp
Tau_debug : Tau.cpp Tau.hpp


at_list = $(addprefix at_,$(opt_progs)) $(addprefix at_,$(debug_progs))

.PHONY : $(at_list)

regtest :

apptest : $(at_list)

$(at_list) : at_% : %
#	./DirMatching ./$* tests/app_tests/$*
