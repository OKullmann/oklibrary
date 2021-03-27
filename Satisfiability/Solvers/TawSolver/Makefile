# Oliver Kullmann, 3.3.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = tawSolver ftawSolver tawSolver_alpha ttawSolver ctawSolver ctawSolver_alpha ctawSolver_lambda ctawSolver_lambda_alpha cttawSolver cttawSolver_lambda mtawSolver mttawSolver tawSolver_wa ttawSolver_wa ctawSolver_wa cttawSolver_wa   Tau

debug_progs = $(addsuffix _debug,$(opt_progs))

atps_debug = tawSolver_debug
atps       = tawSolver

atps_debug := $(addprefix at_,$(atps_debug))
atps       := $(addprefix at_,$(atps))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

weights_set1 = -DTWEIGHT_2=4.85 -DTWEIGHT_4=0.354 -DTWEIGHT_5=0.11 -DTWEIGHT_6=0.0694 -DTWEIGHT_BASIS_OPEN=1.46

weights_set2 = -DWEIGHT_2=7.0 -DWEIGHT_4=0.31 -DWEIGHT_5=0.19 -DWEIGHT_6=0.19/1.7 -DWEIGHT_BASIS_OPEN=1.70

lambda_value = 100
lambda_option = -DLAMBDA=$(lambda_value)
alpha_value = 0.5
alpha_option = -DALPHA=$(alpha_value)

firstvar_option = -DFIRST_VARIABLE -DPURE_LITERALS


tawSolver : tawSolver.cpp
tawSolver_debug : tawSolver.cpp

ftawSolver : tawSolver.cpp
ftawSolver : CPPFLAGS += $(firstvar_option)
ftawSolver_debug : tawSolver.cpp
ftawSolver_debug : CPPFLAGS += $(firstvar_option)

tawSolver_alpha : tawSolver.cpp
tawSolver_alpha : CPPFLAGS += $(alpha_option)
tawSolver_alpha_debug : tawSolver.cpp
tawSolver_alpha_debug : CPPFLAGS += $(alpha_option)

tawSolver_wa : tawSolver.cpp
tawSolver_wa : CPPFLAGS += $(weights_set2)
tawSolver_wa_debug : tawSolver.cpp
tawSolver_wa_debug : CPPFLAGS += $(weights_set2)


ttawSolver : tawSolver.cpp
ttawSolver : CPPFLAGS += -DPURE_LITERALS -DTAU_ITERATION
ttawSolver : numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math
ttawSolver : numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
ttawSolver_debug : tawSolver.cpp
ttawSolver_debug : CPPFLAGS += -DPURE_LITERALS -DTAU_ITERATION


ttawSolver_wa : tawSolver.cpp
ttawSolver_wa : CPPFLAGS += -DPURE_LITERALS -DTAU_ITERATION $(weights_set1)
ttawSolver_wa : numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math
ttawSolver_wa : numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
ttawSolver_wa_debug : tawSolver.cpp
ttawSolver_wa_debug : CPPFLAGS += -DPURE_LITERALS -DTAU_ITERATION $(weights_set1)


ctawSolver : tawSolver.cpp
ctawSolver : CPPFLAGS += -DALL_SOLUTIONS
ctawSolver_debug : tawSolver.cpp
ctawSolver_debug : CPPFLAGS += -DALL_SOLUTIONS

ctawSolver_alpha : tawSolver.cpp
ctawSolver_alpha : CPPFLAGS += -DALL_SOLUTIONS $(alpha_option)
ctawSolver_alpha_debug : tawSolver.cpp
ctawSolver_alpha_debug : CPPFLAGS += -DALL_SOLUTIONS $(alpha_option)

ctawSolver_lambda : tawSolver.cpp
ctawSolver_lambda : CPPFLAGS += -DALL_SOLUTIONS $(lambda_option)
ctawSolver_lambda_debug : tawSolver.cpp
ctawSolver_lambda_debug : CPPFLAGS += -DALL_SOLUTIONS $(lambda_option)

ctawSolver_lambda_alpha : tawSolver.cpp
ctawSolver_lambda_alpha : CPPFLAGS += -DALL_SOLUTIONS $(lambda_option) $(alpha_option)
ctawSolver_lambda_alpha_debug : tawSolver.cpp
ctawSolver_lambda_alpha_debug : CPPFLAGS += -DALL_SOLUTIONS $(lambda_option) $(alpha_option)

ctawSolver_wa : tawSolver.cpp
ctawSolver_wa : CPPFLAGS += -DALL_SOLUTIONS $(weights_set2)
ctawSolver_wa_debug : tawSolver.cpp
ctawSolver_wa_debug : CPPFLAGS += -DALL_SOLUTIONS $(weights_set2)


cttawSolver : tawSolver.cpp
cttawSolver : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS
cttawSolver : numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math
cttawSolver : numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
cttawSolver_debug : tawSolver.cpp
cttawSolver_debug : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS

cttawSolver_lambda : tawSolver.cpp
cttawSolver_lambda : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS $(lambda_option)
cttawSolver_lambda : numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math
cttawSolver_lambda : numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
cttawSolver_lambda_debug : tawSolver.cpp
cttawSolver_lambda_debug : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS $(lambda_option)

cttawSolver_wa : tawSolver.cpp
cttawSolver_wa : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS $(weights_set1)
cttawSolver_wa : numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math
cttawSolver_wa : numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
cttawSolver_wa_debug : tawSolver.cpp
cttawSolver_wa_debug : CPPFLAGS += -DTAU_ITERATION -DALL_SOLUTIONS $(weights_set1)


mtawSolver : tawSolver.cpp
mtawSolver : CPPFLAGS += -DVAR_MARGINALS
mtawSolver_debug : tawSolver.cpp
mtawSolver_debug : CPPFLAGS += -DVAR_MARGINALS

mttawSolver : tawSolver.cpp
mttawSolver : CPPFLAGS += -DTAU_ITERATION -DVAR_MARGINALS
mttawSolver : numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math
mttawSolver : numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math
mttawSolver_debug : tawSolver.cpp
mttawSolver_debug : CPPFLAGS += -DTAU_ITERATION -DVAR_MARGINALS



Tau : Tau.cpp Tau.hpp
Tau_debug : Tau.cpp Tau.hpp


.PHONY : $(atps) $(atps_debug)

regtest :

apptest : $(atps) $(atps_debug)

$(atps_debug) : at_%_debug : %_debug
	DirMatching ./$*_debug tests/app_tests/$*

$(atps) : at_% : %
	DirMatching ./$* tests/app_tests/$*
