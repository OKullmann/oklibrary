# Oliver Kullmann, 4.2.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../..

opt_progs = FreeGlut_test
debug_progs = Test_debug FreeGlut_test_debug

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math

numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math


FreeGlut_test : LDFLAGS += -lglut -lGLU -lGL -lGLEW
FreeGlut_test : Optimisation_plus =
FreeGlut_test : FreeGlut_test.cpp
FreeGlut_test_debug : LDFLAGS += -lglut -lGLU -lGL -lGLEW
FreeGlut_test_debug : Optimisation_plus =
FreeGlut_test_debug : FreeGlut_test.cpp

Test_debug : Test.cpp Plot.hpp

regtest : Test_debug
	./Test_debug

apptest :

