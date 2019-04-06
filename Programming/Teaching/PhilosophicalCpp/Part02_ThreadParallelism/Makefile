# Oliver Kullmann, 17.11.2018 (Swansea)
# Copyright 2018, 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

opt_progs = AtomicCounter Recursion Sum
debug_progs =

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

Optimisation_plus = -fwhole-program -fstrict-aliasing -funroll-loops -fvariable-expansion-in-unroller -floop-nest-optimize -fgraphite-identity --param graphite-max-nb-scop-params=0
# Remark: -static yields the compiler emitting crashing code for programs Recursion, Sum with gcc version 7.3.0, 8.3.0

randgenloc = $(oklibloc)/Satisfiability/Transformers/Generators
include_options += -I $(randgenloc)

LDFLAGS += -pthread


AtomicCounter : AtomicCounter.cpp

Recursion : Recursion.cpp

Sum : Sum.cpp


regtest :

apptest :

