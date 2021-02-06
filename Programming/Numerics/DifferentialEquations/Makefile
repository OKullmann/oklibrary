# Oliver Kullmann, 6.2.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../..//..

opt_progs = Ode1
debug_progs = Ode1_debug

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math

numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math


Ode1 : Ode1.cpp Ode.hpp ../FloatingPoint.hpp
Ode1_debug : Ode1.cpp Ode.hpp ../FloatingPoint.hpp


regtest :

apptest :

