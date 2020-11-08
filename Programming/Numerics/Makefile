# Oliver Kullmann, 3.3.2019 (Swansea)
# Copyright 2019, 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../..

opt_progs = ExperimentsTau
debug_progs = Test_debug ExperimentsTau_debug

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math

numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math


ExperimentsTau : ExperimentsTau.cpp Tau.hpp
ExperimentsTau_debug : ExperimentsTau.cpp Tau.hpp

Test_debug : Test.cpp FloatingPoint.hpp Tau.hpp

regtest : Test_debug
	./Test_debug
apptest :

