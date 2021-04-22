# Oliver Kullmann, 6.2.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = Ode1 Ode
debug_progs = Test_debug Ode1_debug Ode_debug

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

numerics_plus = -fno-signed-zeros -fno-math-errno -fno-trapping-math

numerics_minus += -fno-unsafe-math-optimizations -fno-associative-math -fno-reciprocal-math


ifun10 = -DIFUN0=$(idir)Ode1.fun0
ifun11 = -DIFUN1=$(idir)Ode1.fun1
ifun12 = -DIFUN2=$(idir)Ode1.fun2
ifun13 = -DIFUN3=$(idir)Ode1.fun3

difun1 = $(ifun10) $(ifun11) $(ifun12) $(ifun13)

ifun0 = -DIFUN0=$(idir)Ode.fun0
ifun1 = -DIFUN1=$(idir)Ode.fun1
ifun2 = -DIFUN2=$(idir)Ode.fun2
ifun3 = -DIFUN3=$(idir)Ode.fun3

difun = $(ifun0) $(ifun1) $(ifun2) $(ifun3)


Ode1 : LDFLAGS += -lglut -lGLU -lGL -lGLEW -lmpfr -lgmp
Ode1 : Optimisation_plus =
Ode1 : CPPFLAGS += $(difun1)
Ode1 : Ode1.cpp Ode1.fun0 Ode1.fun1 Ode1.fun2 Ode1.fun3 Ode.hpp Stepper.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(oklibloc)/Visualisation/Graphics/Plot.hpp
Ode1_debug : LDFLAGS += -lglut -lGLU -lGL -lGLEW -lmpfr -lgmp
Ode1_debug : CPPFLAGS += $(difun1)
Ode1_debug : Ode1.cpp Ode1.fun0 Ode1.fun1 Ode1.fun2 Ode1.fun3 Ode.hpp Stepper.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(oklibloc)/Visualisation/Graphics/Plot.hpp

Ode : LDFLAGS += -lglut -lGLU -lGL -lGLEW -lmpfr -lgmp
Ode : Optimisation_plus =
Ode : CPPFLAGS += $(difun)
Ode : Ode.cpp Ode.fun0 Ode.fun1 Ode.fun2 Ode.fun3 Ode.hpp Stepper.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(oklibloc)/Visualisation/Graphics/Plot.hpp
Ode_debug : LDFLAGS += -lglut -lGLU -lGL -lGLEW -lmpfr -lgmp
Ode_debug : CPPFLAGS += $(difun)
Ode_debug : Ode.cpp Ode.fun0 Ode.fun1 Ode.fun2 Ode.fun3 Ode.hpp Stepper.hpp $(programmingloc)/Numerics/FloatingPoint.hpp $(oklibloc)/Visualisation/Graphics/Plot.hpp


Test_debug : Test.cpp Ode.hpp $(programmingloc)/Numerics/FloatingPoint.hpp

regtest : Test_debug
	./Test_debug

apptest :
