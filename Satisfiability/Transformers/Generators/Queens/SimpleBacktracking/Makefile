# Oliver Kullmann, 25.5.2018 (Swansea)
# Copyright 2018, 2019 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../../..

opt_progs = qcount qcount_ct wqcount pqcount
debug_progs = $(addsuffix _debug, $(opt_progs))

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

Optimisation_plus = -fwhole-program -funsafe-loop-optimizations

# -------------------------

qcount : Warning_options += -Wno-parentheses
qcount : NQueens.cpp NQueens.hpp

qcount_debug : Warning_options += -Wno-parentheses
qcount_debug : NQueens.cpp NQueens.hpp


SETN=-DNN=16

qcount_ct : CXXFLAGS+=$(SETN)
qcount_ct : NQueens_ct.cpp

qcount_ct_debug : CXXFLAGS += $(SETN)
qcount_ct_debug : NQueens_ct.cpp

wqcount : Warning_options += -Wno-parentheses
wqcount : NQueens_w.cpp

wqcount_debug : Warning_options += -Wno-parentheses
wqcount_debug : NQueens_w.cpp

pqcount : Warning_options += -Wno-parentheses
pqcount : LDFLAGS += -pthread
pqcount : NQueenspar.cpp NQueens.hpp

pqcount_debug : Warning_options += -Wno-parentheses
pqcount_debug : LDFLAGS += -pthread
pqcount_debug : NQueenspar.cpp NQueens.hpp
