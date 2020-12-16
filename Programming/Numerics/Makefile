# Oliver Kullmann, 3.3.2019 (Swansea)
# Copyright 2019, 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../..

opt_progs = ExperimentsTau GMP_MPFR_test
debug_progs = Test_debug ExperimentsTau_debug GMP_MPFR_test_debug

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math

numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math


ExperimentsTau : ExperimentsTau.cpp Tau.hpp FloatingPoint.hpp
ExperimentsTau_debug : ExperimentsTau.cpp Tau.hpp FloatingPoint.hpp

GMP_MPFR_test : LDFLAGS += -lmpfr -lgmp
GMP_MPFR_test : Optimisation_plus =
GMP_MPFR_test : GMP_MPFR_test.cpp
GMP_MPFR_test_debug : LDFLAGS += -lmpfr -lgmp
GMP_MPFR_test_debug : Optimisation_plus =
GMP_MPFR_test_debug : GMP_MPFR_test.cpp


Test_debug : Test.cpp FloatingPoint.hpp Tau.hpp

regtest : Test_debug
	./Test_debug
apptest :

