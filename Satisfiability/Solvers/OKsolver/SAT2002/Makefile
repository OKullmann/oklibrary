# Oliver Kullmann, 28.12.2019 (Swansea)
# Copyright 2019, 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../..

opt_progs = OKsolver2002 OKsolver2002_NTP
debug_progs = $(addsuffix _debug, $(opt_progs))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

Optimisation_plus = -fwhole-program # -funsafe-loop-optimizations

# -------------------------

OKsolver2002 : Warning_options += -Wno-return-type -Wno-maybe-uninitialized -Wno-array-bounds -Wno-clobbered
OKsolver2002 : CXXFLAGS+=-DSYSTIME
OKsolver2002 : OKsolver_2002.c Parameter.c VarLitKlm.c BaumRes.c Belegung.c lokalesLernen.c Reduktion.c Filter.c Abstand_nKaZ.c Projektion_Prod.c Speicher.c OKsolver_2002_lnk.c Ausgaben.c Einlesen.c

OKsolver2002_debug : CXXFLAGS+=-DSYSTIME
OKsolver2002_debug : OKsolver_2002.c Parameter.c VarLitKlm.c BaumRes.c Belegung.c lokalesLernen.c Reduktion.c Filter.c Abstand_nKaZ.c Projektion_Prod.c Speicher.c OKsolver_2002_lnk.c Ausgaben.c Einlesen.c

OKsolver2002_NTP : Optimisation_core = -O1 -DNDEBUG -march=native
OKsolver2002_NTP : Warning_options += -Wno-return-type -Wno-maybe-uninitialized -Wno-array-bounds -Wno-clobbered
OKsolver2002_NTP : CXXFLAGS+=-DSYSTIME -DNBAUMRES
OKsolver2002_NTP : OKsolver_2002.c Parameter.c VarLitKlm.c BaumRes.c Belegung.c lokalesLernen.c Reduktion.c Filter.c Abstand_nKaZ.c Projektion_Prod.c Speicher.c OKsolver_2002_lnk.c Ausgaben.c Einlesen.c

OKsolver2002_NTP_debug : CXXFLAGS+=-DSYSTIME -DNBAUMRES
OKsolver2002_NTP_debug : OKsolver_2002.c Parameter.c VarLitKlm.c BaumRes.c Belegung.c lokalesLernen.c Reduktion.c Filter.c Abstand_nKaZ.c Projektion_Prod.c Speicher.c OKsolver_2002_lnk.c Ausgaben.c Einlesen.c
